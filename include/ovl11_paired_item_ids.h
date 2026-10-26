#ifndef OVL11_PAIRED_ITEM_IDS_H
#define OVL11_PAIRED_ITEM_IDS_H

#include "common.h"

/* Pure signed-halfword item-ID operations; no state changes.
 * The two predicates return 0 or 1. These ranges are paired numerically;
 * their gameplay categories are not established. */
s32 Ovl11IsUpperPairedItemId(s16 itemId); /* 0xED..0xF2 inclusive */
s32 Ovl11IsLowerPairedItemId(s16 itemId); /* 0x7D..0x82 inclusive */

/* Subtract 0x70 with signed-halfword wrap, without validating the input.
 * CC7CC guards this conversion with IsUpperPairedItemId before using the
 * result in a halfword request. Upper-range inputs map to the lower range. */
s16 Ovl11MapToLowerPairedItemId(s16 itemId);

#endif
