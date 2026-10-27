#ifndef OVL11_PENDING_ITEM_COMMAND_H
#define OVL11_PENDING_ITEM_COMMAND_H

#include "common.h"

/* ovl_11 command over the signed pending-item latch at work+0x51F0.
 * mode 0: return 1 iff the nonzero pending item's packed property at
 * propertyIndex is zero. Independently snapshot the pending value (even zero)
 * into gOvl11ValueSlots[resultSlot] unless resultSlot == -1, then clear the
 * latch even on a failed test. Property/slot indices are unchecked; resultSlot
 * retains its full signed word width rather than narrowing to a halfword.
 * mode 1: return whether shared status word 80070C88 has bit 0x20, without
 * consuming the latch or writing a slot. Other modes return 0 unchanged.
 * No waiting/retry occurs here; the interpreter chooses the next command from
 * the boolean result. unused retains operand 4. Original: ovl_11:800E9914. */
s32 Ovl11ConsumePendingItemCommand(s16 mode, s16 propertyIndex, s32 resultSlot, s32 unused);

#endif
