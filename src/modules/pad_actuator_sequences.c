/*
 * Actuator sequence requests and stepping, exe:0x8001B258-0x8001B530.
 * Four private GP-relative tables connect all six members. The two-entry
 * dispatch table is { NULL, StepPadActuatorSequence }; eleven presets hold
 * a priority and an address into three-byte step streams.
 *
 * Both sequence slots affect the FIRST pad record, not separate ports.
 * Preserve the slot order and partial output writes; see the public header
 * for arbitration, timing, and output ownership. Grouping: file-groupings.md.
 */
#include "common.h"
#include "pad_actuator_sequences.h"
#include "pad_actuator_gate.h"

/* Sizes and signedness follow the two-slot dispatcher and original loads.
 * Each definition remains <= -G8, preserving original GP ownership. */
u16 gPadActuatorSequenceTicks[2];
s16 gPadActuatorSequenceModes[2];
s32 gPadActuatorSequenceAddresses[2];
s16 gPadActuatorSequencePriorities[2];

void SetPadActuatorSequenceAddress(s32 slot, s32 address);
void ResetPadActuatorSequence(s32 slot);

void UpdatePadActuatorSequences(void) {
    s32 slot = 0;
    s16 *mode;
    void (*handler)(s32);
    void (**dispatch)(s32) = gPadActuatorSequenceDispatch;

    mode = gPadActuatorSequenceModes;
    do {
        handler = dispatch[*mode];
        if (handler != 0) {
            handler(slot);
        }
        slot += 1;
        mode += 1;
    } while ((u32)slot < 2U);
}

void RequestPadActuatorSequence(s32 slot, s32 preset) {
    if ((D_8006C844 & 0x10000) || GetPadActuatorsEnabled() == 0) {
        return;
    }
    {
        s32 halfwordOffset = slot * 2;
        s16 *mode = (s16 *)((char *)&gPadActuatorSequenceModes + halfwordOffset);
        if (*mode == 0 ||
            ((s32)*(s16 *)((char *)&gPadActuatorSequencePriorities + halfwordOffset) <=
                 (s32)gPadActuatorSequencePresets[preset].priority &&
             *(s32 *)((char *)&gPadActuatorSequenceAddresses + slot * 4) !=
                 gPadActuatorSequencePresets[preset].stepsAddress)) {
            SetPadActuatorSequenceAddress(slot, gPadActuatorSequencePresets[preset].stepsAddress);
            *mode = 1;
            *(s16 *)((char *)&gPadActuatorSequencePriorities + halfwordOffset) =
                (s16)gPadActuatorSequencePresets[preset].priority;
        }
    }
}

void StopPadActuatorSequence(s32 slot) {
    ResetPadActuatorSequence(slot);
}

/* Reached through the dispatch table's mode-1 callback, not a direct call. */
void StepPadActuatorSequence(s32 slot) {
    u8 *inputBytes;
    u8 *step;
    s32 ticks;
    s32 duration;

    inputBytes = (u8 *)&gPadInputStateStorage;
    step = (u8 *)gPadActuatorSequenceAddresses[slot];
    if (step) {
        ticks = gPadActuatorSequenceTicks[slot];
        duration = step[2];
        ticks++;
        duration >>= 1;
        gPadActuatorSequenceTicks[slot] = (u16)ticks;
        if (duration < (s16)ticks) {
            /* Retain the original separate read/modify/write of the cursor.
             * The local step pointer advances independently before emission. */
            *((volatile s32 *)&gPadActuatorSequenceAddresses[slot]) += 3;
            step += 3;
            gPadActuatorSequenceTicks[slot] = 0;
        }
        if (step[0] == 0xFF) {
            ResetPadActuatorSequence(slot);
            return;
        }
        inputBytes[0x36] = 0;
        inputBytes[0x37] = 0;
        if (step[0] & 1) {
            inputBytes[0x36] = 1;
        }
        if (step[0] & 2) {
            inputBytes[0x37] = step[1];
        }
    }
}

void SetPadActuatorSequenceAddress(s32 slot, s32 address) {
    slot <<= 2;
    *(s32 *)((char *)&gPadActuatorSequenceAddresses + slot) = address;
}

void ResetPadActuatorSequence(s32 slot) {
    PadInputActuatorView *input;
    s32 *address;
    s16 *mode;
    s16 *priority;
    s16 *counter;
    s32 wordOffset;

    input = &gPadInputStateStorage;
    input->request0 = 0;
    input->request1 = 0;
    wordOffset = slot << 2;
    address = (s32 *)((char *)&gPadActuatorSequenceAddresses + wordOffset);
    *address = 0;
    mode = (s16 *)((char *)&gPadActuatorSequenceModes + (slot << 1));
    priority = (s16 *)((char *)&gPadActuatorSequencePriorities + (slot << 1));
    *mode = 0;
    counter = (s16 *)((char *)&gPadActuatorSequenceTicks + (slot << 1));
    *priority = 0;
    *counter = 0;
}
