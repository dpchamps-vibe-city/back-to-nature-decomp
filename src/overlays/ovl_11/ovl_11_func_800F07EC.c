#include "common.h"

s32 ovl_11_func_800C3548(s32 arg0);
s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800F07EC(s16 arg0, s32 arg1, s32 arg2, u32 *arg3) {
    s16 var_a1;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 var_a1_2;
    s32 var_s2;
    u32 temp_v1;
    s32 *temp_s1;

    temp_s1 = (s32 *) ((u8 *) D_80076280 + (arg0 * 0x1D4));
    temp_a2 = ovl_11_func_800C3548((s32) arg0);
    temp_v1 = *arg3;
    var_s2 = 0;
    switch (temp_v1) {
    case 0:
    case 1:
    case 2:
    case 3:
        var_s2 = ovl_11_func_800E5B84(arg0, 5, 0, 0);
        break;
    case 4:
        ovl_11_func_800E5B84(arg0, 6, 0, 0);
        temp_a2_2 = arg2 + 0x258;
        temp_s1[2] += 0xA;
        if (temp_a2_2 < temp_s1[2]) {
            temp_s1[2] = temp_a2_2;
            var_s2 = 1;
        }
        break;
    case 5:
        ovl_11_func_800E5B84(arg0, 6, 0, 0);
        temp_s1[2] -= 0xA;
        if (temp_s1[2] < arg2) {
            temp_s1[2] = arg2;
            var_s2 = 1;
        }
        break;
    case 6:
        if (temp_a2 == 0) {
            ovl_11_func_800E5B84(arg0, 4, 0, 0);
            var_a1_2 = arg1 + 0x1F4;
            temp_s1[0] += 0xA;
            if (var_a1_2 < temp_s1[0]) {
                temp_s1[0] = var_a1_2;
                var_s2 = 1;
            }
        } else {
            ovl_11_func_800E5B84(arg0, 2, 0, 0);
            var_a1_2 = arg1 - 0x1F4;
            temp_s1[0] -= 0xA;
            if (temp_s1[0] < var_a1_2) {
                temp_s1[0] = var_a1_2;
                var_s2 = 1;
            }
        }
        break;
    case 7:
        var_s2 = ovl_11_func_800E5B84(arg0, 8, 0, 0);
        break;
    case 8:
        if (temp_a2 == 0) {
            ovl_11_func_800E5B84(arg0, 2, 0, 0);
            temp_s1[0] -= 0xA;
            if (temp_s1[0] < arg1) {
                temp_s1[0] = arg1;
                var_s2 = 1;
            }
        } else {
            ovl_11_func_800E5B84(arg0, 4, 0, 0);
            temp_s1[0] += 0xA;
            if (arg1 < temp_s1[0]) {
                temp_s1[0] = arg1;
                var_s2 = 1;
            }
        }
        break;
    case 9:
        var_a1 = 7;
        if (temp_a2 == 0) {
            var_a1 = 8;
        }
        var_s2 = ovl_11_func_800E5B84(arg0, var_a1, 0, 0);
        break;
    }
    if (var_s2 == 1) {
        if (*arg3 == 9) {
            *arg3 = 0;
        } else {
            *arg3 += 1;
            var_s2 = 0;
        }
    }
    return var_s2;
}
