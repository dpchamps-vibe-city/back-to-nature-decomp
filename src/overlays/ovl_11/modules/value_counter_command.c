/*
 * Object-program compare/increment command (ovl_11:800E5A1C).
 * The shared value-slot bank is not private TU ownership; this command has
 * no state-record helper dependency. See ovl11_value_counter_command.h.
 */
#include "common.h"
#include "ovl11_value_counter_command.h"

s32 Ovl11StepValueCounter(s16 thresholdOrSlot, s16 counterSlot, s32 thresholdIsSlot, s32 obeyFreezeFlag) {
    s32 counter;
    s32 pastThreshold;

    pastThreshold = 0;
    counter = gOvl11ValueSlots[counterSlot];
    if (thresholdIsSlot != 0 ? gOvl11ValueSlots[thresholdOrSlot] / 2 < counter : thresholdOrSlot / 2 < counter) {
        pastThreshold = 1;
    }
    if (obeyFreezeFlag == 0 || (D_8006C844 & 0x8000000) == 0) {
        gOvl11ValueSlots[counterSlot] += 1;
    }
    return pastThreshold;
}
