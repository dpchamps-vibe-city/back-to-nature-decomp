#ifndef OVL11_SEASONAL_RECORD_IDS_H
#define OVL11_SEASONAL_RECORD_IDS_H

#include "common.h"
#include "game_types.h"

/* Require active bit 0 plus the current season's bit (0..3 => 2/4/8/0x10;
 * other seasons use mask 0x2). Return 0 if unavailable, otherwise signed kind,
 * except kind 0x40 becomes 0x13A in season 2. No state changes. A returned
 * zero is ambiguous with a valid zero kind. The view is only a record prefix. */
s16 Ovl11GetSeasonalRecordId(Ovl11SeasonalRecordView *record);

/* Require the same season/active bits plus bit 0x80. Return 0 without changes
 * on failure, including kind 0x139 already marked with bit 0x4000. On success
 * set that bit for kind 0x139; otherwise clear active bit 0. Copy kind to
 * out->id and set out->valid=1, preserving out+2. Map 0x40 to 0x13A in season 2,
 * and 0x3A/0x3E to 0x13B/0x13C in season 3. Return 1.
 *
 * out must be valid on a successful path: although the first stores test it
 * for NULL, the original subsequently reads it without a guard. NULL is safe
 * only on a rejection path. This behavior is retained, not repaired. */
s32 Ovl11ConsumeSeasonalRecord(Ovl11SeasonalRecordView *record, Ovl11RecordPresentation *out);

#endif
