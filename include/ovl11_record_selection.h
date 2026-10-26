#ifndef OVL11_RECORD_SELECTION_H
#define OVL11_RECORD_SELECTION_H

#include "common.h"
#include "game_types.h"

/* Return a random eligible index, or -1 without calling Rand when none exist.
 * family == 0 selects twenty 0xB8-byte records; ANY other value selects ten
 * 0xB4-byte records. Eligibility: nonzero u16 ID, signed +0x30 key == 1,
 * and neither 0x400 nor 0x20000 set in the +0x34 flags. No record is changed.
 * A successful scan advances the shared random generator once. */
s32 Ovl11ChooseEligibleTaggedRecord(u16 family);

/* Return the first matching index, scanning all large records before small
 * ones. position is a FOUR-word by-value object; its X/Z words at +0/+8 and
 * radius use the records' signed coordinate units. Y and the fourth word are ignored.
 * Bounds are strict: X/Z must lie INSIDE position +/- radius. Arithmetic must
 * fit s32; zero/negative radius cannot select a record. Nonzero ID, matching
 * signed +0x30 key and clear flag 0x20000 are required.
 * Success writes *outRecord and *category (large: flag 0x10000 -> 0, else 1;
 * small: 2). Failure returns -1 and leaves BOTH outputs untouched. The index
 * is local to its family, so retain the category/pointer to disambiguate it.
 * This is an X/Z square query, not a radial-distance or 3D collision test. */
s32 Ovl11FindTaggedRecordInBox(Ovl11PaddedVec3 position, s16 key,
                              s32 radius, s32 *category, void **outRecord);

#endif
