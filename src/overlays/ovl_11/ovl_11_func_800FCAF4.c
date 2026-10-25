#include "common.h"
#include "game_types.h"

extern s16 gOvl11NumberTextScratch;

s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800FCAF4(s16 arg0, s16 arg1, s16 arg2) {
    s16 var_a0;

    var_a0 = arg0;
    if (var_a0 >= 0x3E8) {
        var_a0 = 0x3E7;
    }
    *((u16 *)func_8001A970((s32) var_a0, &gOvl11NumberTextScratch, 3)) = 0xFFFF;
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32) &gOvl11NumberTextScratch, (s32) arg1, (s32) arg2);
}
