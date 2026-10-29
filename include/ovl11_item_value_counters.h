#ifndef OVL11_ITEM_VALUE_COUNTERS_H
#define OVL11_ITEM_VALUE_COUNTERS_H

#include "common.h"

/* Current counts and a pending bank of at most 30 accepted item entries.
 * Values use Ovl11GetItemSellValue's integer units. Item IDs passed to the
 * update/staging calls must be valid for that shared value-table service.
 * State must be initialized; these calls neither allocate nor validate it. */

/* Map a 16-bit item ID to one of 25 counter categories, or -1 if unmapped.
 * Several IDs intentionally share a category. No state is changed. */
s32 Ovl11ItemCounterIndex(u16 itemId);

/* Add the item's value even if it has no counter category. Return 1 after
 * incrementing a mapped category, 0 otherwise. Counter addition is 16-bit;
 * a resulting value >=1000 is clamped to 999. IDs A1..A3 also increment the
 * separate signed-threshold counter and can set game flag 5F. */
s32 Ovl11RecordItemValue(u16 itemId);

/* Current category count (0..65535), or -1 for an unmapped item ID. */
s32 Ovl11GetItemCounter(s16 itemId);

/* Return 1 without changes when the pending entry count is already >=30.
 * Otherwise return 0, add value and consume one entry even for an unmapped
 * category; a mapped pending halfword count is incremented without a cap. */
s32 Ovl11StageItemValue(s16 itemId);

/* Return 0 for an empty pending bank. Otherwise merge all 25 counts with the
 * same 16-bit-add/999-clamp rule, add the pending value, clear pending counts,
 * entry count and value, and return 1. This does not credit the balance yet. */
s32 Ovl11CommitPendingItemCounts(void);

/* Always return 1. If flag A0 is clear, set it BEFORE checking the calendar.
 * Only a date not found by Ovl11FindCalendarDate credits the current value to
 * the spendable balance and clears that value. Counts are never cleared here.
 * Another service must clear A0 before this can apply a later value. */
s32 Ovl11ApplyItemValueOnce(void);

#endif
