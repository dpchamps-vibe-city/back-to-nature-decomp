#include "common.h"

s32 ovl_11_func_800F06D8(s32 *arg0, s32 *arg1);
s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
/* Both original calls explicitly clear all trailing argument slots. */
s32 ovl_11_func_800E9284(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800E5C60(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800EDD54(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 var_a3;
    s16 var_s0;
    s32 *var_a0;
    s32 *var_a1;
    s32 temp_a2;

    if (arg1 & 1) {
        var_s0 = (s16)gOvl11ValueSlots[arg0];
    } else {
        var_s0 = arg0;
    }
    if (arg1 & 2) {
        var_a3 = (s16)gOvl11ValueSlots[arg3];
    } else {
        var_a3 = (s16)arg3;
    }
    if (var_s0 == 0x29) {
        var_a1 = D_80071B00;
    } else {
        var_a1 = (s32 *) ((u8 *) D_80076280 + (var_s0 * 0x1D4));
    }
    if (var_a3 == 0x29) {
        var_a0 = D_80071B00;
    } else {
        var_a0 = (s32 *) ((u8 *) D_80076280 + (var_a3 * 0x1D4));
    }
    temp_a2 = ovl_11_func_800F06D8(var_a0, var_a1);
    if (var_s0 == 0x29) {
        ovl_11_func_800E9284((s16) temp_a2, 0, 0, 0);
    } else {
        ovl_11_func_800E5B84(var_s0, (s16)arg2, temp_a2, 1);
        ovl_11_func_800E5C60(var_s0, 0, 0, 0);
    }
    return 1;
}
