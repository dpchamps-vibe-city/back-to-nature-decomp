#ifndef OVL11_ITEM_RECORD_SEARCH_H
#define OVL11_ITEM_RECORD_SEARCH_H

#include "common.h"

/* ovl_11:0x800EEC1C. Count records whose id equals itemId; stack quantities
 * are ignored. Return the signed-halfword match count widened to s32,
 * including matches found after the removal quota is exhausted.
 *
 * bankSelector must be 0 or 1. Scan groups 0..lastGroup (lastGroup 0..3):
 *  0: one held record selected by bankSelector;
 *  1: the corresponding primary list, 2^(work exponent+1) records;
 *  2: the corresponding 64-record overflow bank;
 *  3: a further 64-record bank for selector 0 only (empty for selector 1).
 * lastGroup == -1 instead scans group 1 from index 1, skipping index 0.
 * Caller must ensure the exponent describes valid allocated lists.
 *
 * removeCount == 0 only counts. A positive value clears up to that many
 * matching RECORDS, not quantity units; a negative value clears all matches
 * while decrementing the quota. Group 0/1 removal releases sprite state;
 * selector-1 held removal also resets the primary request and held-item
 * flags/state. Group 0 deliberately falls through the group-1 removal path.
 * Other groups only clear the three halfwords. No compaction or bounds checks.
 */
s32 Ovl11CountOrRemoveItemRecords(s16 itemId, s16 bankSelector,
                                 s16 lastGroup, s32 removeCount);

#endif
