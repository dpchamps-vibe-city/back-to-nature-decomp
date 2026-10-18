#include "common.h"

void ovl_11_func_800F889C(s32 *arg0, s32 arg1, s16 *arg2);

void ovl_11_func_800F882C(s32 *arg0, s32 arg1, s32 arg2, s16 *arg3) {
    s32 i;

    for (i = arg1; i < arg1 + arg2; i++) {
        ovl_11_func_800F889C(arg0, i, arg3);
        arg3 += 3;
    }
}
