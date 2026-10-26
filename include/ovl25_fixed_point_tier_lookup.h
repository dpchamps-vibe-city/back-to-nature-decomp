#ifndef OVL25_FIXED_POINT_TIER_LOOKUP_H
#define OVL25_FIXED_POINT_TIER_LOOKUP_H

#include "common.h"

/* Convert Q12 input toward zero, retaining its signed 16-bit integer part.
 * The original shift pair deliberately sign-extends bit 15 of that part;
 * inputs outside its range wrap rather than clamp. Returns a sign-extended
 * halfword selected at integer thresholds 40, 30 and 20 (10 for delta).
 * Value has a below-20 entry; delta is zero below 10. Neither mutates state.
 * The caller treats value as a total amount and delta as an increment before
 * duration scaling. Names do not imply a gameplay meaning for either table. */
s32 Ovl25LookupTierValue(s32 inputQ12);
s32 Ovl25LookupTierDelta(s32 inputQ12);

#endif
