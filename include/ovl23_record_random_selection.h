#ifndef OVL23_RECORD_RANDOM_SELECTION_H
#define OVL23_RECORD_RANDOM_SELECTION_H

#include "common.h"

/* Overlay 23's record-controller decisions. All indices are unchecked;
 * the caller uses record indices 0..5, trial rows 0..3 and weight sets 0..1.
 * Both calls consume one Rand(100) result and leave the record state intact.
 */

/* ovl_23:800BA3B4. Truncate record+30 minus record+32 to s16, bucket it
 * against the four descending signed thresholds at state+3A4..3AA, then
 * return whether the selected signed trial-table entry exceeds Rand(100).
 * Equality with a threshold selects the next bucket, not the earlier one. */
s32 Ovl23TestRecordRandomThreshold(s16 trialRow, s16 recordIndex);

/* ovl_23:800BA494. Choose one of two three-weight rows within weightSet.
 * Set 0 tests the record's +28 category for 1 or 2. Nonzero sets compare
 * truncated Q12 positions and the +2E distance threshold (margin <101).
 * Accumulate u16 weights with s16 narrowing after each addition; the first
 * cumulative sum strictly greater than Rand(100) returns 3, 2 or 1. Return
 * -1 if none crosses the draw. Weights need not sum to 100. */
s32 Ovl23ChooseRecordWeightedResult(s16 recordIndex, s16 weightSet);

#endif
