#ifndef OVL11_SLOT_COMPARISON_CODE_H
#define OVL11_SLOT_COMPARISON_CODE_H

#include "common.h"

/* Read signed low halfwords from categorySlot, leftSlot and rightSlot in
 * gOvl11ValueSlots; write a sign-extended code to outputSlot. All four indices
 * are unchecked; inputs may alias the output because all reads precede it.
 *
 * Category IDs 19,20,16,5,21/22,34,9,11,6,7,25 map to indices 0..10;
 * others use index 11. right==-1 selects the 0x70D/0x70E family; otherwise
 * left==right selects 0x548/0x549 + 2*index, and unequal values select
 * 0x526..0x528 + 3*index. The category's reference value determines the
 * variant, with reversed ordering for IDs 5/11/16/25. Returns 1 always.
 * This mapping does not establish what the resulting codes represent. */
s32 Ovl11WriteSlotComparisonCode(s16 categorySlot, s16 leftSlot,
                                 s32 rightSlot, s32 outputSlot);

#endif
