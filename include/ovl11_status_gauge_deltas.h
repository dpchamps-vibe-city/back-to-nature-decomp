#ifndef OVL11_STATUS_GAUGE_DELTAS_H
#define OVL11_STATUS_GAUGE_DELTAS_H

#include "common.h"

/* Original ovl_11:800D622C. Add signed deltas to primary-object status:
 * +12 current gauge clamps to 0..the signed limit at +14;
 * +16 secondary gauge clamps to 0..100 (99 at hour 6), and bit 3 at +36
 * suppresses positive secondary deltas only;
 * +58 signed word counter clamps to 0..65535.
 * Zero deltas leave their respective fields untouched (no reclamping).
 * Gauge sums are narrowed to signed halfwords before comparison. All state
 * must be initialized; no overflow/range validation precedes the addition.
 * Always returns 1. The fourth argument is unused but remains in the ABI. */
s32 Ovl11ApplyStatusDelta(s8 gaugeDelta, s8 secondaryDelta,
                        s16 counterDelta, s32 unused);

#endif
