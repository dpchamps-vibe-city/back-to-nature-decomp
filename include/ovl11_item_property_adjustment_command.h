#ifndef OVL11_ITEM_PROPERTY_ADJUSTMENT_COMMAND_H
#define OVL11_ITEM_PROPERTY_ADJUSTMENT_COMMAND_H

#include "common.h"

/* ovl_11 command over seven 0xF8-byte records and value-bank status slots
 * 0xB..0x11. Read the record index from recordSlot's SIGNED LOW HALFWORD,
 * set record+0x34 bit 8, then request second-tagged-item property 0x1B into
 * progressSlot. The property service publishes a code, not the raw property.
 *
 * No item/code zero: if this record's status is not 2, set it to 1, flagSlot
 * to 1 and progressSlot to 0xB; otherwise publish 0xC. Return 1.
 * Code 1..3 with flagSlot nonzero: add the table adjustment to signed +0x16,
 * using row 0 initially or row 2 after +0xB8 bit 0x10 (which this sets). Cap at
 * 255, without a lower clamp. Mark status 2; publish 0xD unless all seven
 * statuses are 2. If all are 2, publish 0xE and add 10 (cap 255) to ALL seven
 * records, even on repeated calls. Return 1.
 * Other code/zero flag: select record mode 6 and return 0.
 *
 * All indices are unchecked. Keep record index in 0..6 and command slots
 * disjoint from status slots when using this progress protocol: accesses
 * retain their original sequential aliasing effects. No rollback on failure.
 * Original identity: ovl_11:800EDAA4. */
s32 Ovl11ApplyItemPropertyAdjustmentCommand(s16 progressSlot, s16 recordSlot, s32 flagSlot);

#endif
