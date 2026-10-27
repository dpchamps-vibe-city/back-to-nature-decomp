#ifndef OVL11_ITEM_ELIGIBILITY_COMMAND_H
#define OVL11_ITEM_ELIGIBILITY_COMMAND_H

#include "common.h"

/* ovl_11 read-only item predicate. fromSlot != 0 resolves the SIGNED LOW
 * HALFWORD of gOvl11ValueSlots[itemOrSlot]; otherwise use the literal ID.
 * ID 0xA4 always returns 1. Other items require flags & 0x1C00 != 0 and
 * the unsigned low halfword of packed property 3 < 3; otherwise return 0.
 * Item and slot indices are unchecked; item tables must be initialized.
 * No slot writes or item/state changes. Original identity: ovl_11:800E93CC. */
s32 Ovl11TestItemEligibilityCommand(s16 itemOrSlot, s16 fromSlot);

#endif
