#include "common.h"

void ovl_15_func_80136C70(s16 slot, s16 mode);
s32 func_8001FABC(s16 arg0);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);

s8 ovl_15_func_80136B18(s16 arg0) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 3; var_s0++) {
        ovl_15_func_80136C70(var_s0, arg0);
    }
    temp_v0 = *(s32 *) ((u8 *) D_8005E3A8 + 0);
    if (temp_v0 & 0x4000) {
        func_8001FABC(5);
        D_80137587++;
    } else if (temp_v0 & 0x1000) {
        func_8001FABC(5);
        D_80137587--;
    }
    if (D_80137587 < 0) {
        D_80137587 = 2;
    }
    if (D_80137587 >= 3) {
        D_80137587 = 0;
    }
    func_800248B0(gActiveRenderContext->field_D8 + 4, 0x39, (D_80137587 * 0x31) + 0x18);
    temp_v0_2 = *(s32 *) ((u8 *) D_8005E3A8 + 8);
    if (temp_v0_2 & 0x40) {
        func_8001FABC(0);
        return D_80137587;
    }
    if (temp_v0_2 & 0x20) {
        func_8001FABC(1);
        return -1;
    }
    return 3;
}
