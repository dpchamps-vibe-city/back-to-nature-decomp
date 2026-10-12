#include "common.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800FD3CC(s16 arg0, s16 arg1);

void ovl_11_func_800FD33C(void) {
    s32 var_s0;
    s16 var_a1;

    func_80017B3C(D_8005E3C0->field_D8 + 0x54, (s32) (D_8005360E + D_80054BBC[0]), 0xBA, 0x1A);
    var_a1 = 0x30;
    for (var_s0 = 0; var_s0 < 7; var_s0++) {
        ovl_11_func_800FD3CC(var_s0, var_a1);
        var_a1 += 0x18;
    }
}
