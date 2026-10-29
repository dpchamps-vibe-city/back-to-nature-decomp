/*
 * pad — both controller ports, from startup to the per-frame input record.
 *
 * InitPads starts PadInitDirect communication with two receive buffers and
 * two actuator tables. Every frame PollPadInputs reads each port's state,
 * aligns actuators once a pad stabilizes, and has UpdatePadInputRecord turn
 * the receive buffer into a PadInputState: held and newly pressed buttons,
 * two independent auto-repeat streams, and an analog-stick magnitude. An
 * analog pad whose d-pad is idle reports its left stick as d-pad directions
 * (ReadPadDirectionByte) and as a magnitude (ReadPadStickMagnitude).
 *
 * WritePadActuatorRequests formats the per-port output table; a disabled
 * actuator gate clears both ports' tables on the next output update.
 * Public setup, polling and gate interfaces are in pad_input_setup.h,
 * pad_input_update.h and pad_actuator_gate.h respectively.
 *
 * One unit, [0x800139FC, 0x80014554): the actuator gate word and extended
 * mode IDs are declared here (members reach them through $gp) and reached
 * absolutely from outside. The following function is cd_loader's filename
 * bootstrap helper. See notes/file-groupings.md for membership evidence and
 * plans/flag-review-report.md for the reviewed whole-unit compiler setting.
 */

#include "common.h"
#include "psyq/libapi.h"
#include "pad_input_setup.h"
#include "psyq/memory.h"
#include "pad_input_update.h"
#include "psyq/libpad.h"
#include "game_types.h"
#include "pad_actuator_gate.h"
#include "update_timing.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void StartPadCommunication(void);

void InitPads(void) {
    StartPadCommunication();
    ChangeClearPAD(0);
}

/*
 * Pad-input reset and the two independent auto-repeat timing pairs.
 */

void ResetPadInputState(void) {
    memset(gPad0Input, 0, 0x38);
    memset(gPad1Input, 0, 0x38);
    memset(&gPadReadBuffers, 0, 0x44);
    memset(&gPadActuatorBuffers, 0, 0x10);
    SetPadRepeatTiming(20, 7);
    SetPadSecondaryRepeatTiming(20, 7);
}

/* game_loop declares the two input pointers and reaches them through $gp;
 * this unit reaches them absolutely, so it leaves them extern. */
void SetPadRepeatTiming(s32 delay, s32 interval) {
    PadInputState *pad1;
    PadInputState *pad0;

    pad1 = gPad1Input;
    pad0 = gPad0Input;
    pad1->repeatDelay = delay;
    pad0->repeatDelay = delay;
    pad1->repeatInterval = interval;
    pad0->repeatInterval = interval;
}

void SetPadSecondaryRepeatTiming(s32 delay, s32 interval) {
    PadInputState *pad1;
    PadInputState *pad0;

    pad1 = gPad1Input;
    pad0 = gPad0Input;
    pad1->secondaryRepeatDelay = delay;
    pad0->secondaryRepeatDelay = delay;
    pad1->secondaryRepeatInterval = interval;
    pad0->secondaryRepeatInterval = interval;
}

/*
 * Software enable gate shared by both pads' buffered actuator output, read by
 * request scheduling and the output service. See pad_actuator_gate.h for its
 * deferred effects.
 */

/* Declared here for the members' GP-relative accesses. */
s32 gPadActuatorsEnabled;

void SetPadActuatorsEnabled(s32 enabled) {
    gPadActuatorsEnabled = enabled;
}

s32 GetPadActuatorsEnabled(void) {
    return gPadActuatorsEnabled;
}

/*
 * Poll both controller ports and maintain held, newly pressed, and two
 * independent auto-repeat button streams. Connection transitions clear
 * transient input while preserving repeat timings and actuator requests.
 */

/* Polling, startup and output share this unit's GP-relative state. */
s32 gPadAlignmentPending[2];
s16 gPadExtendedModeIds[2];

/* Helpers keep external linkage: nothing shows the original made them
 * static. Only polling is published to callers. */
void ClearPadInputTransientState(PadInputState *input);
void UpdatePadInputRecord(void *record, s32 portIndex);
s32 AlignPadActuators(s32 portIndex);
s32 ReadPadRawButtons(s32 portIndex);
s32 ReadPadStickMagnitude(s32 portIndex);
void WritePadActuatorRequests(s32 request0, s32 request1, s32 portIndex);

/* Preserve the inner retry loop: a successful actuator alignment clears
 * input and re-polls this port before advancing. The nesting also keeps
 * the invariant port/record addresses in the matching loop preheader. */
void PollPadInputs(void) {
    PadPortPair ports;
    s32 port;
    s32 retry;
    u32 state;

    ports = gPadPortIds[0];
    for (port = 0; port < 2; port++) {
        retry = 1;
        do {
            state = PadGetState(((u8 *)&ports)[port]);
            if (state < 7) {
                switch (state) {
                case PadStateDiscon:
                case PadStateFindPad:
                    if (!(D_8006C844 & 0x10000)) {
                        gPadExtendedModeIds[port] = 0;
                        gPadAlignmentPending[port] = 1;
                        ClearPadInputTransientState((PadInputState *)
                            ((char *)&gPadInputStateStorage + (((port << 3) - port) << 3)));
                        retry = 0;
                        continue;
                    }
                    break;
                case PadStateReqInfo:
                case PadStateExecCmd:
                    ClearPadInputTransientState((PadInputState *)
                        ((char *)&gPadInputStateStorage + (((port << 3) - port) << 3)));
                    retry = 0;
                    continue;
                case PadStateStable:
                    if (gPadAlignmentPending[port] == 1) {
                        gPadExtendedModeIds[port] = PadInfoMode(((u8 *)&ports)[port], InfoModeCurExID, 0);
                        if (AlignPadActuators(port) != 0) {
                            gPadAlignmentPending[port] = 0;
                            ClearPadInputTransientState((PadInputState *)
                                ((char *)&gPadInputStateStorage + (((port << 3) - port) << 3)));
                            continue;
                        }
                    }
                    break;
                }
            }
            UpdatePadInputRecord((char *)&gPadInputStateStorage +
                (((port << 3) - port) << 3), port & 0xFF);
            retry = 0;
        } while (retry == 1);
    }
}

/* record points to one 0x38-byte PadInputState; portIndex selects the SDK
 * read/output buffers (0 or 1, truncated to a byte). Keep the existing
 * void-pointer interface and all signed/unsigned repeat arithmetic. */
void UpdatePadInputRecord(void *record, s32 portIndex) {
    PadInputState *input = (PadInputState *)record;
    s32 newPresses;
    s32 port;
    s32 previousButtons;
    s32 heldButtons;
    s32 repeatInterval;
    s32 secondaryInterval;
    s32 timingDivisor;
    s32 swappedButtons;
    s32 retainedBitsMask;
    u32 repeatDelay;
    u32 secondaryDelay;
    u32 overrideButtons;
    u32 nextDelayCounter;
    u32 nextRepeatCounter;
    u32 nextSecondaryDelayCounter;
    u32 nextSecondaryRepeatCounter;
    u8 timingMode;

    port = portIndex & 0xFF;
    WritePadActuatorRequests(input->actuatorRequest0, input->actuatorRequest1, port);
    input->heldButtons = ~ReadPadRawButtons(port) & 0xFFFF;
    input->stickMagnitude = ReadPadStickMagnitude(port);

    /* The first pad can use an external button source. Start is reported
     * to that source instead; otherwise swap its 0x20/0x40 button bits and
     * suppress the stick magnitude. This is not a general input remap. */
    if ((((D8006C838View *)&D_8006C838)->field_0C & 0x10000) && (record == gPad0Input)) {
        if (input->heldButtons & 0x800) {
            ((D8006C838View *)&D_8006C838)->field_1C->field_0 = 0x800;
        } else {
            overrideButtons = ((D8006C838View *)&D_8006C838)->field_1C->field_4;
            swappedButtons = (overrideButtons >> 1) & 0x20;
            if (overrideButtons & 0x20) {
                swappedButtons |= 0x40;
                retainedBitsMask = ~0x60;
            } else {
                retainedBitsMask = ~0x60;
            }
            overrideButtons &= retainedBitsMask;
            overrideButtons |= swappedButtons;
            input->heldButtons = overrideButtons;
            input->stickMagnitude = 0;
        }
    }

    previousButtons = input->previousHeldButtons;
    heldButtons = input->heldButtons;
    newPresses = (previousButtons ^ heldButtons) & heldButtons;
    input->pressedButtons = newPresses;

    if (heldButtons == previousButtons) {
        timingMode = GetUpdateTimingMode();
        timingDivisor = 1;
        if ((u32)((timingMode - 2) & 0xFFFF) < 2U) {
            timingDivisor = timingMode;
        }
        repeatDelay = (u32)((s32)input->repeatDelay / (s32)timingDivisor);
        repeatInterval = (s32)input->repeatInterval / (s32)timingDivisor;
        if (repeatInterval <= 0) {
            repeatInterval = 1;
        }
        /* The delay must be EXCEEDED, not merely reached. Entering repeat
         * mode resets its counter but emits no button on that update. */
        if (input->repeatActive == 0) {
            input->repeatButtons = 0;
            nextDelayCounter = input->repeatCounter + 1;
            input->repeatCounter = nextDelayCounter;
            if ((u32)repeatDelay < nextDelayCounter) {
                input->repeatCounter = 0U;
                input->repeatActive = 1;
            }
        } else {
            nextRepeatCounter = input->repeatCounter + 1;
            input->repeatCounter = nextRepeatCounter;
            if ((nextRepeatCounter % (u32)repeatInterval) == 0) {
                input->repeatButtons = input->heldButtons;
            } else {
                input->repeatButtons = 0;
            }
        }
        secondaryDelay = (u32)((s32)input->secondaryRepeatDelay / (s32)timingDivisor);
        secondaryInterval = (s32)input->secondaryRepeatInterval / (s32)timingDivisor;
        if (secondaryInterval <= 0) {
            secondaryInterval = 1;
        }
        if (input->secondaryRepeatActive == 0) {
            input->secondaryRepeatButtons = 0;
            nextSecondaryDelayCounter = input->secondaryRepeatCounter + 1;
            input->secondaryRepeatCounter = nextSecondaryDelayCounter;
            if ((u32)secondaryDelay < nextSecondaryDelayCounter) {
                input->secondaryRepeatCounter = 0U;
                input->secondaryRepeatActive = 1;
            }
        } else {
            nextSecondaryRepeatCounter = input->secondaryRepeatCounter + 1;
            input->secondaryRepeatCounter = nextSecondaryRepeatCounter;
            if ((nextSecondaryRepeatCounter % (u32)secondaryInterval) == 0) {
                input->secondaryRepeatButtons = input->heldButtons;
            } else {
                input->secondaryRepeatButtons = 0;
            }
        }
    } else {
        /* ANY held-mask change restarts BOTH streams, including a release
         * that leaves other buttons held; only new presses are emitted. */
        input->repeatButtons = newPresses;
        input->secondaryRepeatButtons = newPresses;
        input->repeatCounter = 0U;
        input->repeatActive = 0;
        input->secondaryRepeatCounter = 0U;
        input->secondaryRepeatActive = 0;
    }
    input->previousHeldButtons = input->heldButtons;
}

/* Unlike ResetPadInputState, preserve both timing pairs and actuator
 * request bytes. -1 distinguishes unavailable stick input from zero. */
void ClearPadInputTransientState(PadInputState *input) {
    input->heldButtons = 0;
    input->pressedButtons = 0;
    input->stickMagnitude = -1;
    input->previousHeldButtons = 0;
    input->repeatCounter = 0;
    input->repeatActive = 0;
    input->secondaryRepeatCounter = 0;
    input->secondaryRepeatActive = 0;
    input->repeatButtons = 0;
    input->secondaryRepeatButtons = 0;
}

s32 ReadPadDirectionByte(s32 port);

/* The two active-low button bytes of a digital (type 4) or analog (type 7)
 * pad, d-pad byte high; 0xFF00FFFF (nothing pressed) when the port reported
 * an error or another pad type. */
s32 ReadPadRawButtons(s32 port) {
    s32 type;

    if (gPadReadBuffers[port][0] != 0) {
        return 0xFF00FFFF;
    }

    type = gPadReadBuffers[port][1] >> 4;
    if (type != 4 && type != 7) {
        return 0xFF00FFFF;
    }

    return (ReadPadDirectionByte(port) << 8) | gPadReadBuffers[port][3];
}

void StartPadCommunication(void) {
    PadInitDirect((unsigned char *)&gPadReadBuffers, (unsigned char *)&gPadReadBuffers + 0x22);
    PadSetAct(0, (unsigned char *)&gPadActuatorBuffers, 8);
    PadSetAct(0x10, (unsigned char *)&gPadActuatorBuffers + 8, 8);
    PadStartCom();
    gPadActuatorsEnabled = 1;
}

/* Nonzero once PadSetActAlign accepts the actuator alignment for a stable pad. */
s32 AlignPadActuators(s32 portIndex) {
    PadPortPair ports;
    u8 *port;
    s32 result;

    ports = gPadPortIds[0];
    port = (u8 *)&ports + portIndex;
    result = 0;
    if (PadGetState(*port) == PadStateStable) {
        result = PadSetActAlign(*port, D_80048B14);
    }
    return result;
}

s16 ComputeStickAngle(s32 stickX, s32 stickY);

/* The active-low d-pad byte. An analog pad whose d-pad is idle reports its
 * left stick instead: the stick angle's 90-degree sector clears one direction
 * bit (up 0x10, right 0x20, down 0x40, left 0x80). */
s32 ReadPadDirectionByte(s32 port) {
    s32 type;
    s32 digital_state;
    s32 analog_check;

    digital_state = gPadReadBuffers[port][2];
    type = gPadReadBuffers[port][1] >> 4;

    if (type == 4) {
        return digital_state;
    }

    if (type != 7) {
        return digital_state;
    }

    if ((digital_state & 0xF0) != 0xF0) {
        return digital_state;
    }

    analog_check = ComputeStickAngle(gPadReadBuffers[port][6], gPadReadBuffers[port][7]);

    if (analog_check == -1) {
        digital_state = gPadReadBuffers[port][2];
    } else if ((u16)(analog_check - 0x2D) >= 0x10F) {
        digital_state = gPadReadBuffers[port][2] & 0xEF;
    } else if ((u16)(analog_check - 0x2E) < 0x59) {
        digital_state = gPadReadBuffers[port][2] & 0xDF;
    } else if ((u16)(analog_check - 0x88) < 0x59) {
        digital_state = gPadReadBuffers[port][2] & 0xBF;
    } else if (analog_check >= 226) {
        digital_state = gPadReadBuffers[port][2] & 0x7F;
    }

    return digital_state;
}

s32 ComputeStickMagnitude(s32, s32);

/* The left stick's magnitude for an analog pad whose d-pad is idle; -1 for
 * any other pad or state. */
s32 ReadPadStickMagnitude(s32 port) {
    if ((gPadReadBuffers[port][1] >> 4) != 4) {
        if ((gPadReadBuffers[port][1] >> 4) == 7) {
            if ((gPadReadBuffers[port][2] & 0xF0) == 0xF0) {
                return ComputeStickMagnitude(gPadReadBuffers[port][6], gPadReadBuffers[port][7]);
            }
        }
        return -1;
    }
    return -1;
}

/*
 * ComputeStickMagnitude: 0 inside the dead zone (both axes within
 * 0x5C..0xA4); otherwise the distance from center (0x80, 0x80) less 0x25,
 * clamped to 0..0x5A.
 *
 * MATCHES, WITH AN EXPLICIT POLICY EXCEPTION (embedded assembly).
 * Granted by the user on 2026-08-09 after the clean-C space was exhausted.
 * Full write-up: notes/retros/2026-08-09-func_800142D8-retro.md
 *
 * WHY THE ASM IS HERE. Clean C reaches 41/45 words. The whole residual is one
 * delay-slot thread choice inside cc1's `dbr` on the second guard branch:
 *   target     beq $v0,$0,$L3 / slt   $v0,$a2,92    (fall-through insn MOVED)
 *   clean C    beq $v0,$0,$L9 / addiu $v0,$v1,-128  (target insn COPIED and the
 *                                                    branch redirected -> +1)
 * Registers, schedule, webs and value provenance are otherwise byte-exact.
 * All 1,057,675 legal variable assignments over this statement skeleton were
 * enumerated (build/bruteForce/func_800142D8/): ZERO reproduce the target.
 * 13,368 reproduce the guard but then lose the calculation block's allocation.
 * The two requirements are mutually exclusive under any renaming, and the
 * skeleton is pinned from both ends (see the load-bearing facts below), so
 * there is no clean-C statement order left to try. C has no way to name a
 * delay slot, so the branch itself has to be written out.
 *
 * WHAT THE ASM DOES. It is guard branch 2 and nothing else: three instructions
 * of 44. The `slt` in the delay slot is the fall-through instruction the target
 * moves there; writing the branch by hand is the only way to place it.
 * Register allocation is still the compiler's — the operands are plain "r"
 * constraints, no hard-register pinning — and cc1 picks $v0/$a2 on its own.
 *
 * The asm's branch target is cc1's own label for the join point after this
 * `if`, i.e. the entry to the calculation block. cc1 numbers labels across
 * the whole file, so the number depends on every earlier function in this
 * module ($L3 when the function stood alone). foldTranslationUnit.ts
 * renumbers it when a fold moves the function; an edit to an earlier member
 * that adds or removes labels moves it too, and the link then fails on an
 * undefined label or `make check` names this function.
 *
 * THE TABS IN THE `.set` LINES ARE LOAD-BEARING. maspsx recognises `.set`
 * state by exact spelling (`.set\tnoreorder`) and consumes those lines itself.
 * Written with spaces they reach `gas` instead, `gas` re-enables reordering for
 * the rest of the function, and every later delay slot silently refills — 50
 * instructions, 36/50 words, no error anywhere. Do not reformat this string.
 *
 * DO NOT REDO — premises eliminated with evidence:
 * - signature: arity 2/3/4 (unused extras) give byte-identical output.
 * - flags: flagProbe finds no target fingerprint; the only 44-instruction
 *   column, -fno-rerun-cse-after-loop, scores 34/44 (it kills `move $a2,$a1`).
 * - assembler: real ASPSX 2.86 (psyq_sdk, under wine) emits `nop` for an
 *   unfilled branch — identical to maspsx — and reproduces cc1's output
 *   byte-for-byte. The delay-slot fill is cc1's, not the assembler's.
 * - tail spelling: 576 variants; rounding form, mask split, both clamp
 *   spellings, return form and declaration order are all inert.
 * - scheduler: analyzeTargetSchedule/searchSchedulerState replay .sched and
 *   .sched2 EXACTLY in all 12 blocks. Not a scheduler problem.
 *
 * Two source facts that are load-bearing; changing them regresses the file:
 * - `x` must be assigned exactly once (sched.c adjust_priority only boosts a
 *   destination with REG_N_SETS == 1); that is why `r` exists.
 * - `t = y` must not directly follow `y = stickY & 0xFF` (cse.c rewrites and
 *   deletes a copy whose source is defined by the preceding insn).
 */
s32 ComputeStickMagnitude(s32 stickX, s32 stickY) {
    s32 x;
    s32 y;
    s32 t;
    s32 xsq;
    s32 value;
    s32 original;
    s32 r;
    s32 cond;

    y = stickY & 0xFF;
    x = stickX & 0xFF;
    t = y;

    if ((u32)(x - 0x5C) < 0x49) {
        /* if (t >= 0xA5) goto calc; cond = (t < 0x5C); */
        __asm__ __volatile__(".set\tnoreorder\n"
                             ".set\tnomacro\n"
                             "\tslt\t%0,%1,165\n"
                             "\tbeq\t%0,$0,$L87\n"
                             "\tslt\t%0,%1,92\n"
                             ".set\tmacro\n"
                             ".set\treorder"
                             : "=&r"(cond)
                             : "r"(t));
        if (cond == 0) {
            return 0;
        }
    }

    value = x - 0x80;
    xsq = value * value;
    t = y - 0x80;
    t = t * t + xsq;
    value = csqrt(t << 12);

    original = value;
    value >>= 12;
    if (original < 0) {
        value = (original + 0xFFF) >> 12;
    }

    r = value & 0xFF;
    r -= 0x25;
    if (r < 0) {
        r = 0;
    }
    if (r >= 0x5B) {
        r = 0x5A;
    }

    return r & 0xFF;
}

/* The stick's direction in degrees, 0..359, clockwise from up (90 is right,
 * 180 down), from axis bytes centered on 0x80; -1 when centered, which
 * includes the dead zone where both axes lie within 0x5C..0xA4. */
s16 ComputeStickAngle(s32 stickX, s32 stickY) {
    u8 s1;
    u8 s0;
    s32 dx;
    s32 dy;
    s32 ax;
    s32 ay;
    s32 angle;
    s32 v;
    s32 c;

    s1 = stickX & 0xFF;
    s0 = stickY & 0xFF;
    if ((u32) (s1 - 0x5C) < 0x49U && s0 < 0xA5U) {
        if (s0 >= 0x5CU) {
            s1 = 0x80;
            s0 = 0x80;
        }
    }
    if (s1 == 0x80) {
        if (s0 == 0x80) {
            return -1;
        }
    }
    dx = s1 - 0x80;
    ax = dx < 0 ? -dx : dx;
    dy = s0 - 0x80;
    ay = dy < 0 ? -dy : dy;
    if (ay != 0) {
        if (ax != 0) {
            angle = ratan2(ax, ay) * 3;
            v = angle * 0x78;
            if (v < 0) {
                angle = (v + 0xFFF) >> 0xC;
            } else {
                angle = v >> 0xC;
            }
        } else {
            angle = 0;
        }
    } else {
        angle = 0x5A;
    }
    if (s1 >= 0x80) {
        if (s0 < 0x81) {
            goto done;
        } else {
            c = 0xB4;
        }
    } else {
        if (s0 < 0x81) {
            c = 0x168;
        } else {
            angle += 0xB4;
            goto done;
        }
    }
    angle = c - angle;
done: ;
    if (angle == 0x168) {
        angle = 0;
    }
    return (s16) angle;
}

/* portIndex is 0 or 1 (unchecked); only each request's low byte is used.
 * Mode ID zero selects a 0x40 header and collapses either nonzero request to
 * byte 1, clearing byte 2. Otherwise byte 1 is boolean request0 and byte 2
 * carries request1. No SDK call occurs here: PadSetAct already points at
 * these tables. Disabling the gate clears BOTH eight-byte port tables. */
void WritePadActuatorRequests(s32 request0, s32 request1, s32 portIndex) {
    u8 act0;
    u8 act1;

    act0 = request0 & 0xFF;
    act1 = request1 & 0xFF;

    if (gPadExtendedModeIds[portIndex] == 0) {
        gPadActuatorBuffers[portIndex][0] = 0x40;
        gPadActuatorBuffers[portIndex][1] = (act0 > 0);
        if (act1 != 0) {
            gPadActuatorBuffers[portIndex][1] = 1;
        }
        gPadActuatorBuffers[portIndex][2] = 0;
    } else {
        gPadActuatorBuffers[portIndex][1] = (act0 > 0);
        gPadActuatorBuffers[portIndex][2] = act1;
    }

    if (gPadActuatorsEnabled == 0) {
        memset(gPadActuatorBuffers, 0, 0x10);
    }
}
