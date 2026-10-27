#ifndef OVL11_ITEM_USE_RESPONSE_H
#define OVL11_ITEM_USE_RESPONSE_H

#include "common.h"

/* Dereference the signed item-table index, then classify with this priority:
 * flags 0400 -> 4, flags 1800 -> 3, flag 0040 -> 1, flag 0100 -> 0;
 * otherwise IDs 65/3F/3A/3E/64 -> 5, all others -> 2 (IDs/masks in hex).
 * The item-action gate interprets these response codes; they are not a
 * general item type or price. No side effects. itemIndex must be readable
 * and its value must index the loaded gItemDataTable; there is no bounds check. */
s32 Ovl11ClassifyItemUseResponse(s16 *itemIndex);

#endif
