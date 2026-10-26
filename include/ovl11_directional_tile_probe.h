#ifndef OVL11_DIRECTIONAL_TILE_PROBE_H
#define OVL11_DIRECTIONAL_TILE_PROBE_H

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/* Probe the initialized tile map with a by-value position and collision tag.
 * No caller coordinates are modified. Direction 0/1/2/3 means -Z/-X/+Z/+X.
 * With secondSample==0, result 2 triggers one more probe 400 world-coordinate
 * units in that direction: retain 2 only if that probe returns 0, else use 1.
 * Nonzero secondSample suppresses recursion. Other probe results pass through;
 * column 44 with direction 3 returns 1 before probing. The caller at 800D24FC
 * treats 2 specially but supplies no stronger general meaning for the codes.
 * The original query's unused fields/output indirection are retained. */
s32 Ovl11ProbeDirectionalTile(VECTOR position, Ovl11CollisionTag tag,
                             s16 direction, s32 secondSample);

#endif
