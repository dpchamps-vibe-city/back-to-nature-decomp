#include "common.h"

s32 ovl_11_func_800EEC1C(s16 id, s16 which, s16 last, s32 amount);

s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 var_a0;

    var_a0 = arg0;
    if (arg3 & 2) {
        var_a0 = *((s16 *)&D_80129560[var_a0]);
    }
    return ovl_11_func_800EEC1C(var_a0, arg1, arg2, arg3 & 1) != 0;
}
