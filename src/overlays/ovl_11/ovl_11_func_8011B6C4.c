#include "common.h"

/* This caller explicitly passes a fourth zero in $a3 at 8011B72C. */
s32 ovl_11_func_800E6F34(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_8011B6C4(s32 *arg0, s32 arg1, s16 arg2) {
    s32 result;
    u32 state;

    result = 0;
    state = *arg0 - arg1;
    if (state >= 2U) {
        return 0;
    }
    switch (state) {
    case 0:
        ovl_11_func_800E6F34(arg2, 0, 0, 0);
        state++;
        break;
    case 1:
        if ((gPrimaryObjectFlags & 0x34008000) == 0x14008000) {
            result = 1;
        }
        break;
    }
    *arg0 = arg1 + state;
    return result;
}
