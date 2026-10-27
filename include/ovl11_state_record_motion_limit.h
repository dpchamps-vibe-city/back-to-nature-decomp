#ifndef OVL11_STATE_RECORD_MOTION_LIMIT_H
#define OVL11_STATE_RECORD_MOTION_LIMIT_H

#include "common.h"

/* ovl_11:0x800E5704. Resolve position field 0x22 and increment field 0x23.
 * Directions 0/1/-1 reach a limit when Z/X/Y <= limit; 2/3/-2 use >=.
 * On reaching (equality included), clamp that position component to limit,
 * clear ALL increment components and return 1. Otherwise leave the position
 * unchanged, replace the increment with the selected direction at twice
 * magnitude, and return 0. Other directions clear the increment and return 0.
 * Units are the record's raw signed words, with no fixed-point conversion.
 * No null/ID checks: both resolver results must be valid writable triplets.
 * magnitude * 2 must fit a signed word. This configures, not integrates, motion.
 */
s32 Ovl11LimitStateRecordMotion(s16 recordId, s16 direction,
                               s32 limit, s32 magnitude);

/* ovl_11:0x800E5870. Select animation first, then call the core at magnitude
 * 12 (increment magnitude 24). Animation flags are zero; return core result.
 */
s32 Ovl11AnimateStateRecordMotion12(s16 recordId, s16 direction,
                                   s32 limit, s16 animation);

/* ovl_11:0x800EADB8. Same animation-first composition at magnitude 20
 * (increment magnitude 40), but the existing void ABI discards both results.
 * This does not wait for animation or integrate the position. All resolver,
 * direction and raw-unit constraints of the core above still apply. */
void Ovl11AnimateStateRecordMotion20(s16 recordId, s16 direction,
                                    s32 limit, s16 animation);

#endif
