/*
 * Counted collision-query entry points for ovl_11.
 *
 * Both wrappers consume the same counter, even when the unbudgeted path takes
 * it past the limit. The frame driver resets it before updating objects.
 * The collision engine and its record-specific probes remain outside this
 * reconstructed module; the complete original TU boundary is not established.
 * See notes/file-groupings.md for membership evidence.
 */

#include "common.h"
#include "ovl11_collision_query_budget.h"

/* The dispatcher views its packed argument's home slot as a collision tag.
 * The established by-value, halfword-aligned type preserves these wrappers'
 * observed argument homing. Its separate definition spells the packed payload
 * s32; retain this explicit representation boundary rather than changing types. */
s32 ovl_11_func_800BEB28(s32 *position, Ovl11CollisionTag tag, void **hitObject,
                       s32 *hitFlags, s32 context, s32 queryMask);

/* ovl_11:0x800BE208 */
s32 Ovl11QueryCollisionBudgeted(s32 *position, Ovl11CollisionTag tag,
                              u16 unused2, u16 unused3, void **hitObject,
                              s32 *hitFlags, s32 context, s32 queryMask) {
    s32 result;

    if (gOvl11CollisionQueryCount >= 50) {
        /* Exhaustion reports a blocked query, not a successful empty scan. */
        *hitFlags = 0;
        result = 1;
    } else {
        gOvl11CollisionQueryCount += 1;
        result = ovl_11_func_800BEB28(position, tag, hitObject, hitFlags,
                                     context, queryMask);
    }
    return result;
}

/* ovl_11:0x800BE26C */
s32 Ovl11QueryCollisionUnbudgeted(s32 *position, Ovl11CollisionTag tag,
                                s32 unused2, s32 unused3, void **hitObject,
                                s32 *hitFlags, s32 context, s32 queryMask) {
    gOvl11CollisionQueryCount += 1;
    return ovl_11_func_800BEB28(position, tag, hitObject, hitFlags,
                               context, queryMask);
}

/* ovl_11:0x800BE2B8 */
void Ovl11ResetCollisionQueryCount(void) {
    gOvl11CollisionQueryCount = 0;
}
