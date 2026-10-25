#include "common.h"

void ovl_11_func_800F7010(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800D6F78(s16 arg0, s16 arg1, s32 *arg2, s16 arg3, s16 arg4, s32 arg5);
s8 ovl_11_func_800D583C(s16 arg0);
void ovl_11_func_800F7094(s32 arg0, s16 arg1, s16 arg2, s32 arg3);

void ovl_11_func_800FB510(s32 arg0, s32 arg1, s16 *arg2) {
    s16 temp_a0;

    temp_a0 = arg2[0];
    if (temp_a0 == 0) {
        ovl_11_func_800F7010(arg0, D_80126FF0[arg1][0], D_80126FF0[arg1][1]);
        return;
    }
    ovl_11_func_800D6F78(temp_a0, 0, (s32 *)arg0, D_80126FF0[arg1][0], D_80126FF0[arg1][1], 8);
    if (ovl_11_func_800D583C(arg2[0]) == 2) {
        ovl_11_func_800F7094(gActiveRenderContext->field_D8 + 0x58, D_80126FF0[arg1][0], D_80126FF0[arg1][1], arg2[2]);
    }
}
