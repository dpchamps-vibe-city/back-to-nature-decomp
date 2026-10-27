#include "common.h"

extern s32 gObjectWorkPosition[];
extern s32 gObjectPositionRecordCoordinates[];

s32 ovl_11_func_800EDEB8(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 *p;

    if (arg0 == 0x29) {
        p = gObjectWorkPosition;
    } else {
        p = (s32 *)((char *)gObjectPositionRecordCoordinates + arg0 * 0x1D4);
    }
    p[1] += arg1 * 2;
    p[0] += arg2 * 2;
    p[2] += arg3 * 2;
    return 1;
}
