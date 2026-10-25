#ifndef OVL11_COLLISION_QUERY_BUDGET_H
#define OVL11_COLLISION_QUERY_BUDGET_H

#include "common.h"
#include "game_types.h"

/*
 * Counted entry points into the collision engine; ovl_11 must be resident.
 * position is the engine's position-word vector, passed through without unit
 * conversion. tag is the existing four-byte collision descriptor, BY VALUE.
 * queryMask selects the engine's probes; context is passed through unchanged.
 * hitObject and hitFlags receive probe-specific results where the selected
 * engine path writes them. They are not initialized here on a delegated query;
 * supply valid output pointers for every selected path that writes them.
 *
 * unused2/unused3 are retained ABI slots and are neither read nor forwarded.
 * Their established types intentionally differ between the two entry points.
 * The common signed counter counts calls delegated by EITHER wrapper, not
 * hits. It does not saturate: callers must avoid signed overflow.
 */

/* Delegate only while the shared count is below 50, incrementing before the
 * call. Otherwise return 1 (blocked), write zero to *hitFlags, leave *hitObject
 * untouched, and do not increment. hitFlags MUST be non-null on this exhausted
 * path, even if queryMask would not require it during an actual scan.
 * Delegated calls return the engine's collision result (0 clear, 1 blocked).
 * Original identity: ovl_11:0x800BE208. */
s32 Ovl11QueryCollisionBudgeted(s32 *position, Ovl11CollisionTag tag,
                              u16 unused2, u16 unused3, void **hitObject,
                              s32 *hitFlags, s32 context, s32 queryMask);

/* Always increment and delegate, even at/above 50. Return the engine's result.
 * These calls still consume the budget seen by Ovl11QueryCollisionBudgeted.
 * Original identity: ovl_11:0x800BE26C. */
s32 Ovl11QueryCollisionUnbudgeted(s32 *position, Ovl11CollisionTag tag,
                                s32 unused2, s32 unused3, void **hitObject,
                                s32 *hitFlags, s32 context, s32 queryMask);

/* Zero the count; the frame-update driver calls this before object updates.
 * This does not reset any collision-engine state or output records.
 * Original identity: ovl_11:0x800BE2B8. */
void Ovl11ResetCollisionQueryCount(void);

#endif
