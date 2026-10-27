#include "common.h"
#include "game_types.h"
#include "ovl11_surface_height.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/memory.h"
#include "surface_collision.h"

s32 ovl_11_func_800CE550(Vec3 *arg0) {
    Vec3 position;
    s32 result;
    u32 i;

    result = 1;
    i = 0;
    position = *arg0;
    for (; i < 5; i++) {
        SetSurfaceHeightTolerance(D_801232C8[i]);
        if (Ovl11ResolveSurfaceHeight(&position.x, 0) == 0) {
            arg0->y = position.y;
            result = 0;
            break;
        }
    }
    SetSurfaceHeightTolerance(0xC8);
    return result;
}
