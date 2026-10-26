#ifndef OVL11_PENDING_STAGE_DISPATCH_H
#define OVL11_PENDING_STAGE_DISPATCH_H

#include "common.h"

/* Replace the single pending mode; this is not a queue. -1 cancels it.
 * Values 0..2 select handlers; other values are accepted but do no handler
 * work when consumed. Does not reset counters or dispatch immediately. */
void Ovl11SetPendingStageMode(s16 mode);

/* If a mode is pending and the readiness predicate succeeds, dispatch it once
 * and reset gOvl11PendingStageMode to -1. Otherwise leave it pending.
 * Modes 0..2 select stage-matched object handlers; a zero handler result
 * clears the corresponding dispatch counter. Other values do no handler work
 * but are still consumed. This does not advance the counters or retry a mode.
 * The pending value is a signed halfword installed by the setter above. */
void Ovl11ProcessPendingStageMode(void);

#endif
