/*
 * seasonal_record_ids — reconstructed translation unit.
 *
 * Season-gated record kind lookup and consumption with seasonal
 * presentation-ID mapping.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_seasonal_record_ids.h"

s32 Ovl11ConsumeSeasonalRecord(Ovl11SeasonalRecordView *record, Ovl11RecordPresentation *out) {
    s32 requiredFlags;
    u16 flags;

    requiredFlags = (Ovl11GetSeasonRecordMask() | 0x81) & 0xFFFF;
    flags = record->flags;
    if (requiredFlags != (flags & requiredFlags)) {
        return 0;
    }
    if ((u16) record->kind == 0x139) {
        if (flags & 0x4000) {
            return 0;
        }
        record->flags = flags | 0x4000;
    } else {
        record->flags = flags & 0xFFFE;
    }
    if (out != NULL) {
        out->id = (u16) record->kind;
        out->valid = 1;
    }
    /* The original guards the initial stores, not the subsequent reads.
     * A successful path still requires a valid output record. */
    if (((struct Ovl11CalendarDisplayStateView *) D_8006C838)->season == 2) {
        if ((s16) out->id == 0x40) {
            out->id = 0x13AU;
        }
    }
    if (((s16) out->id == 0x3A) && (((struct Ovl11CalendarDisplayStateView *) D_8006C838)->season == 3)) {
        out->id = 0x13BU;
    }
    if ((s16) out->id == 0x3E) {
        if (((struct Ovl11CalendarDisplayStateView *) D_8006C838)->season == 3) {
            out->id = 0x13CU;
            return 1;
        }
    }
    return 1;
}

s16 Ovl11GetSeasonalRecordId(Ovl11SeasonalRecordView *record) {
    s16 id;
    u16 requiredFlags;

    requiredFlags = (Ovl11GetSeasonRecordMask() | 1) & 0xFFFF;
    if (requiredFlags != (record->flags & requiredFlags)) {
        return 0;
    }
    id = record->kind;
    if (gCalendarSeason != 2) {
        return id;
    }
    if (id == 0x40) {
        id = 0x13A;
    }
    return id;
}
