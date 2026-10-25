/*
 * Pad-input reset and the two independent auto-repeat timing pairs.
 * Reconstructed setup slice of the pad family, not a proven original TU
 * boundary; see notes/file-groupings.md.
 */

#include "common.h"
#include "pad_input_setup.h"
#include "psyq/memory.h"
#include "game_types.h"

void ResetPadInputState(void) {
    memset(gPad0Input, 0, 0x38);
    memset(gPad1Input, 0, 0x38);
    memset(&gPadReadBuffers, 0, 0x44);
    memset(&gPadActuatorBuffers, 0, 0x10);
    SetPadRepeatTiming(20, 7);
    SetPadSecondaryRepeatTiming(20, 7);
}

/* Boot owns the two input pointers. Leaving them extern here retains this
 * slice's absolute addressing rather than boot's GP-relative accesses. */
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
