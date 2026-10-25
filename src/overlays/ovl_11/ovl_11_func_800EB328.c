#include "common.h"
#include "psyq/libmath.h"
#include "psyq/abs.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"

s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800EB328(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 *position;
    s32 x;
    s32 y;
    s32 z;
    s32 random;
    s32 offset;
    s32 zOffset;
    s32 index;
    double dx;
    double dz;
    s32 result;
    s32 direction;

    index = arg3;
    position = (s32 *)Ovl11ResolveStateField(0x22, arg0, 0);
    random = Rand(51);
    offset = gOvl11ValueSlots[index] - 25;
    x = random + offset;
    y = (s32)Rand(51) - 700;
    random = Rand(51);
    zOffset = gOvl11ValueSlots[++index] - 25;
    z = random + zOffset;
    result = 0;
    if (y < position[1] - 25) {
        y = position[1] - 25;
    }
    dz = 3.14159 / 24;
    if (gOvl11ValueSlots[arg1] < 0) {
        dx = 157.0795;
        dz = 0.0;
        position[1] = -700;
        position[0] += 157;
    } else if (gOvl11ValueSlots[arg2] >= gOvl11ValueSlots[arg1]) {
        register double radius = 1200.0;

        dz *= gOvl11ValueSlots[arg1];
        dx = radius * sin(dz);
        dz = radius * cos(dz);
        position[1] = y;
        position[0] = x + dx;
        position[2] = z + dz;
    } else {
        register double radius = 157.0;

        dz *= gOvl11ValueSlots[arg2];
        dx = radius * sin(dz + 1.570795);
        dz = radius * cos(dz + 1.570795);
        position[1] = -700;
        position[0] += (s32)dx;
        position[2] += (s32)dz;
        result = 1;
    }
    if (abs((s32)dx) >= abs((s32)dz)) {
        if (dx < 0.0) {
            direction = 9;
        } else {
            direction = 7;
        }
    } else {
        if (dz < 0.0) {
            direction = 8;
        } else {
            direction = 10;
        }
    }
    ovl_11_func_800E5B84(arg0, direction, 0, 0);
    return result;
}
