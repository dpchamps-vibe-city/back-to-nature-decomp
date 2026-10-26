#ifndef OVL11_ITEM_TABLE_QUERIES_H
#define OVL11_ITEM_TABLE_QUERIES_H

#include "common.h"

/* Original ovl_11:800D5750-800D5960. All indices are signed halfwords into
 * gItemDataTable (0x28-byte ItemData records). The table must be initialized;
 * there are no range or null checks. Queries do not mutate table or RNG. */

/* Flags: 0x4000 -> 0; otherwise any bit in 0x8100 -> 1; otherwise 2.
 * Earlier tests have priority when several flags are present. */
s32 Ovl11ClassifyItemFlags(s16 itemIndex);
/* Status +0x18: bit 28 -> 0, else bit 29 -> 1, else bit 30 -> 2, else 3. */
s32 Ovl11ClassifyItemStatus(s16 itemIndex);
s32 Ovl11IsItemStatusNegative(s16 itemIndex);
/* Preserve the signed-byte result ABI even though the table stores u8. */
s8 Ovl11GetItemType(s16 itemIndex);
s32 Ovl11ItemHasFlag40(s16 itemIndex);
/* True when either bit 0x8000 or 0x100 is set, not necessarily both. */
s32 Ovl11ItemHasFlag8100(s16 itemIndex);

/* itemRecord supplies an s16 index at +0 and an s8 argument at +2.
 * Flag 0x800 chooses that argument; otherwise use action's default byte.
 * Calls the table callback as (s8 first, s8 action+5, s16 action+6, 0),
 * returning its result, or -1 if no callback. Callback effects are unrestricted.
 * The action is embedded at item+0x1C, not a separately loaded pointer. */
s32 Ovl11InvokeItemAction(void *itemRecord);

#endif
