#ifndef OVL11_PENDING_STAGE_DISPATCH_H
#define OVL11_PENDING_STAGE_DISPATCH_H

#include "common.h"

/* If a mode is pending and the readiness predicate succeeds, dispatch it once
 * and reset gOvl11PendingStageMode to -1. Otherwise leave it pending.
 * Modes 0..2 select stage-matched object handlers; a zero handler result
 * clears the corresponding dispatch counter. Other values do no handler work
 * but are still consumed. This does not advance the counters or retry a mode.
 * The pending value is a signed halfword; callers install it separately. */
void Ovl11ProcessPendingStageMode(void);

#endif
