/*
 * point_projection — reconstructed translation unit.
 *
 * Single-point GTE projection and its long-vector adapter/depth-only
 * companion, exe:8001DFD4..8001E0B8. The adapter calls the first helper, and
 * the companion repeats the same RTPS/SXY2/depth protocol. Existing unused
 * adapters remain extracted placeholders.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "point_projection.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/inline_c.h"
#include "psyq/inline_c_gas.h"
#include "include_asm.h"

s32 ProjectPointToScreen(s32 *screen, SVECTOR *position) {
    long flagOrDepth;
    SVECTOR projected;

    gte_ldv0(position);
    gte_rtps();
    gte_stflg(&flagOrDepth);
    if (flagOrDepth < 0) {
        return 0;
    }
    gte_stsxy2(&projected);
    if (screen != 0) {
        screen[0] = projected.vx;
        screen[1] = projected.vy;
    }
    gte_stszotz(&flagOrDepth);
    return flagOrDepth;
}

/* Unused long-vector adapter: narrows x/y/z to halfwords before calling
 * the checked projector, then narrows its returned depth to a byte. */
INCLUDE_ASM("build/asm/nonmatchings/modules/point_projection", func_8001E04C);

/* Unused RTPS companion returning OTZ without the FLAG guard. */
INCLUDE_ASM("build/asm/nonmatchings/modules/point_projection", func_8001E088);
