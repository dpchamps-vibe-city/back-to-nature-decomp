#include "common.h"

extern s16 D_800C4A1A;
extern s32 D_800C4A1C;

void func_80015F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8);

void ovl_27_func_800B9FF4(void) {
    s32 var_a1;
    s32 var_a2;

    var_a2 = 0x68;
    if ((D_800C4A1C % 30) < 0xF) {
        var_a2 = 0x64;
    }
    var_a1 = 0xB3;
    if (D_800C4A1A == 0) {
        var_a1 = 0x9B;
    }
    func_80015F80(gActiveRenderContext->field_D8 + 0x88, (s32) D_800C4BD0, 0x24, 0, (s16) var_a2, (s16) var_a1, 0x1000, 0, 0U);
}
