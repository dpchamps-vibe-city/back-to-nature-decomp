#ifndef OVL11_ITEM_SELECTION_VALUE_H
#define OVL11_ITEM_SELECTION_VALUE_H

#include "common.h"

/* Fixed selection value, used for balance-threshold/debit and display.
 * This is NOT the separate item table's buy/sell price. Exact hex mapping:
 * 0067:03E9, 0185:0061, 00F7:002A, 00F8:0023, 00F9:0021,
 * 009E:0015, 009C:000F, 005C:000C, 0097:0007, 0060:0003.
 * All other u16 IDs return zero. No state changes or item-table dependency.
 * The s32 result is not narrowed here; callers retain their own s16 casts. */
s32 Ovl11LookupItemSelectionValue(u16 itemId);

#endif
