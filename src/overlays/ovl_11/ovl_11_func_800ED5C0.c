#include "common.h"

s32 ovl_11_func_80116980(s16 arg0, s16 arg1, s16 arg2);

s32 ovl_11_func_800ED5C0(s16 arg0, s16 arg1, s32 arg2) {
    s16 var_v0;
    s32 temp_v0;

    var_v0 = 0;
    if (arg1 == 0) {
        var_v0 = -1;
    }
    temp_v0 = ovl_11_func_80116980(arg0, 1, var_v0);
    D_80129560[arg2] = temp_v0;
    if (temp_v0 == -2) {
        return 1;
    }
    return temp_v0 != -1;
}
