/* Shared season-to-record-bit mapping, ovl_11:800F581C–800F5868.
 * Query/consume and projected-record clients combine this with their own
 * required flags. No state changes. Membership: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_seasonal_record_ids.h"

s32 Ovl11GetSeasonRecordMask(void) {
    switch (gCalendarSeason) {
    case 0:
    default:
        return 2;
    case 1:
        return 4;
    case 2:
        return 8;
    case 3:
        return 0x10;
    }
}
