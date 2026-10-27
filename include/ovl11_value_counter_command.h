#ifndef OVL11_VALUE_COUNTER_COMMAND_H
#define OVL11_VALUE_COUNTER_COMMAND_H

#include "common.h"

/* Compare BEFORE incrementing gOvl11ValueSlots[counterSlot]. Return 1 iff
 * that counter exceeds threshold/2 (signed division, truncating toward zero).
 * thresholdOrSlot is a signed literal, or a value-slot index when
 * thresholdIsSlot != 0. The increment is skipped only when obeyFreezeFlag
 * is nonzero AND work flags at +C contain 0x08000000. Comparison still runs.
 * Slots, arithmetic overflow and negative values are unchecked; the return
 * is not a saturating/completion latch, and the counter is never reset here.
 */
s32 Ovl11StepValueCounter(s16 thresholdOrSlot, s16 counterSlot,
                         s32 thresholdIsSlot, s32 obeyFreezeFlag);

#endif
