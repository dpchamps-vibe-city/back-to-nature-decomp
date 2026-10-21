#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800BE118(VECTOR position, s16 arg4, VECTOR *arg5, u16 *arg6, u32 arg7) {
    u32 i;
    u32 index;
    Ovl11TransitionBounds *bounds;

    index = arg7;
    if (index >= 4U) {
        index = 0;
    }
    bounds = D_80122D8C[index];
    for (i = 0; i < 2U; i++, bounds++) {
        if (bounds->source == arg4 &&
            bounds->minX < position.vx && position.vx < bounds->maxX &&
            bounds->minZ < position.vz && position.vz < bounds->maxZ) {
            arg5->vx = bounds->exitX;
            arg5->vy = 0;
            arg5->vz = bounds->exitZ;
            *arg6 = bounds->destination;
            return 1;
        }
    }
    return 0;
}
