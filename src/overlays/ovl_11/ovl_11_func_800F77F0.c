#include "common.h"

s32 ovl_11_func_800F71DC(s32 arg0);
s32 ovl_11_func_800F7230(s16 arg0, s16 arg1);

s32 ovl_11_func_800F77F0(s32 arg0) {
    s32 result;

    if (D_80126E2C < 0x400) {
        ovl_11_func_800F71DC(D_80126E2C);
        D_80126E2C += 0x400;
        return 1;
    }
    if (ovl_11_func_800F7230(D_80126E2C - 0x400, arg0) == 0) {
        D_80126E2C -= 0x3B0;
        result = D_80126E2C < 0x13F;
    } else {
        result = 1;
    }
    return result;
}
