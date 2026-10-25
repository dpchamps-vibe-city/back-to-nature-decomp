/*
 * Poll both controller ports and maintain held, newly pressed, and two
 * independent auto-repeat button streams. Connection transitions clear
 * transient input while preserving repeat timings and actuator requests.
 *
 * This slice of the pad family is a reconstructed TU hypothesis, not a
 * proven original boundary; see notes/file-groupings.md.
 */

#include "common.h"
#include "pad_input_update.h"
#include "psyq/libpad.h"
#include "game_types.h"
#include "update_timing.h"

/* Keep the existing small-data ownership; the output formatter also owns
 * the mode-ID pair for its GP-relative reads. Neither array is private. */
s32 gPadAlignmentPending[2];
s16 gPadExtendedModeIds[2];

/* Module helpers keep external linkage; co-location is not evidence that
 * the original symbols were private. Only polling is published to callers. */
void ClearPadInputTransientState(PadInputState *input);
void UpdatePadInputRecord(void *record, s32 portIndex);
s32 func_800140C8(s32 portIndex);
s32 func_80013FC0(s32 portIndex);
s32 func_80014250(s32 portIndex);
void func_80014494(s32 request0, s32 request1, s32 portIndex);

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
                        if (func_800140C8(port) != 0) {
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
    func_80014494(input->actuatorRequest0, input->actuatorRequest1, port);
    input->heldButtons = ~func_80013FC0(port) & 0xFFFF;
    input->stickMagnitude = func_80014250(port);

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
