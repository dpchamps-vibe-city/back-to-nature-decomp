#ifndef OVL11_ITEM_RECORD_LISTS_H
#define OVL11_ITEM_RECORD_LISTS_H

#include "common.h"

/* Operate on the primary object's two eight-entry, six-byte item lists.
 * Compaction preserves nonempty order and clears moved-from records.
 * No allocation or object selection is performed. */
void Ovl11CompactSecondaryItemList(void);
void Ovl11CompactItemStackList(void);

/* Exchange with the corresponding selected six-byte record. reverse == 0
 * removes the first list entry and inserts selected at the tail; any nonzero
 * value removes the last nonempty entry and prepends selected. The removed
 * entry becomes selected. Return 1 unconditionally, NOT the generic helper's
 * -1. If reverse is nonzero and selected is nonempty, the list must contain
 * a nonempty entry; otherwise the generic helper accesses before the list.
 * Both first/selected keys zero makes no change. No validation is added. */
s32 Ovl11ExchangeSecondaryItemList(s32 reverse);
s32 Ovl11ExchangeItemStackList(s32 reverse);

#endif
