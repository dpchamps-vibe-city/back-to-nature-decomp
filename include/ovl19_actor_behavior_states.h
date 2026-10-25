#ifndef OVL19_ACTOR_BEHAVIOR_STATES_H
#define OVL19_ACTOR_BEHAVIOR_STATES_H

#include "common.h"

/* Update both 0x48-byte actor slots in overlay-19 scene work.
 * States 0..2 have no behavior here; states 3..11 dispatch their original
 * handlers. After each handler, decrement that record's positive signed
 * countdown once, even if the handler changed its state/countdown.
 *
 * May choose directions with Rand, change state through Ovl19SetActorControl,
 * update the low halfwords of the two planar positions, and play sound cues.
 * Timers are update counts, not milliseconds; position units remain those of
 * the scene records. Requires initialized scene rate/threshold tables and a
 * nonzero direction-choice interval. No drawing or allocation is performed. */
void Ovl19UpdateActorBehaviors(void);

#endif
