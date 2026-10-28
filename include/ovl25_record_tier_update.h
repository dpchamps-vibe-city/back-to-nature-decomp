#ifndef OVL25_RECORD_TIER_UPDATE_H
#define OVL25_RECORD_TIER_UPDATE_H

#include "common.h"

/* ovl_25:800B97E4. Update the caller's record using the loaded scene's tier
 * parameters. The existing raw record ABI has signed kind at +0, state at +2,
 * timer at +4, Q12 value at +C and signed accumulated amount at +10.
 * Kind and state are DIFFERENT selectors. No record or range validation.
 *
 * Add a divisor-scaled tier value to +10, narrowing to s16. Kind 5 instead
 * uses its configured amount/30, repeatedly decayed by a percentage for
 * scene field +C / 30 iterations, with s16 truncation each time and a minimum
 * of 1. Kind 4 uses its configured amount/30. Other nonzero kinds apply their
 * configured percentage. Kinds 4/5 set timer to 1; otherwise a nonpositive
 * timer requests state 2 through Ovl25SetRecordState.
 *
 * Except for kinds 4/5, also add the tier delta (converted to Q12) divided by
 * the scene divisor to +C. No final value clamp is performed here; the outer
 * scene updater does that. Divisor must be nonzero. Uses no RNG, allocates
 * nothing, and returns no value. Arithmetic/narrowing order is significant.
 */
void Ovl25AccumulateRecordTierValue(s16 *record);

#endif
