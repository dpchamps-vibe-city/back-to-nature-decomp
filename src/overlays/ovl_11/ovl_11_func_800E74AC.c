#include "common.h"

/* Test the work-area position against signed halfword distance limits. */

s32 ovl_11_func_800E74AC(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 result;
    s16 *p;
    s32 delta;
    s32 x;
    s32 y;
    s32 dy;
    s32 xlimit;
    s32 ylimit;

    result = 0;
    xlimit = (s16)arg0;
    ylimit = (s16)arg1;
    p = (s16 *)&D_8006C838;
    x = p[0x2964];
    y = p[0x2968];
    delta = __builtin_abs(x - arg2);
    if (delta < xlimit) {
        dy = __builtin_abs(y - arg3);
        result = dy < ylimit;
    }
    return result;
}
