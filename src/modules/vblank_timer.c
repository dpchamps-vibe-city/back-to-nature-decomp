/*
 * vblank_timer — reconstructed translation unit.
 *
 * Caller-owned VSync-count interval predicate. It optionally rearms the
 * caller timestamp without waiting for a vertical blank.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "vblank_timer.h"
#include "psyq/stddef.h"
#include "psyq/libetc.h"

s32 HasVSyncIntervalElapsed(s32 interval, s32 *stamp, s32 rearm) {
    s32 now;

    now = VSync(-1);
    if (*stamp == 0) {
        *stamp = now;
    }
    if (now - *stamp < interval) {
        return 0;
    }
    if (rearm == 1) {
        *stamp = now;
    }
    return 1;
}
