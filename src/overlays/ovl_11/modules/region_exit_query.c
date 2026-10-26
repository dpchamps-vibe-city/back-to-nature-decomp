/*
 * Region-exit lookup over four pairs of strict X/Z bounds. This service only
 * reads its table; it does not commit the caller's region or position change.
 * Contract: ovl11_region_exit_query.h. Original ovl_11:800BE118-800BE208.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_region_exit_query.h"

s32 Ovl11FindRegionExit(VECTOR position, s16 sourceRegion,
                       VECTOR *destinationPosition, u16 *destinationRegion,
                       u32 variant) {
    u32 i;
    u32 index;
    Ovl11TransitionBounds *bounds;

    index = variant;
    if (index >= 4U) {
        index = 0;
    }
    bounds = gOvl11RegionExitBounds[index];
    for (i = 0; i < 2U; i++, bounds++) {
        if (bounds->source == sourceRegion &&
            bounds->minX < position.vx && position.vx < bounds->maxX &&
            bounds->minZ < position.vz && position.vz < bounds->maxZ) {
            destinationPosition->vx = bounds->exitX;
            destinationPosition->vy = 0;
            destinationPosition->vz = bounds->exitZ;
            *destinationRegion = bounds->destination;
            return 1;
        }
    }
    return 0;
}
