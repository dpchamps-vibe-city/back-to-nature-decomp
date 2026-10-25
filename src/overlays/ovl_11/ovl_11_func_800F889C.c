#include "common.h"

void ovl_11_func_800F7010(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800D6F78(s16 arg0, s16 arg1, s32 *arg2, s16 arg3, s16 arg4, s32 arg5);
void ovl_11_func_800F6D14(s32 *arg0, u16 arg1, s16 arg2, s16 arg3);
s8 ovl_11_func_800D583C(s16 arg0);
void ovl_11_func_800F7094(s32 arg0, s16 arg1, s16 arg2, s32 arg3);
s32 ovl_11_func_800F8404(s32 arg0);

void ovl_11_func_800F889C(s32 *arg0, s32 arg1, s16 *arg2) {
    if (arg2[0] == 0) {
        ovl_11_func_800F7010((s32) (arg0 + 1), D_80126E54[arg1].x, D_80126E54[arg1].y);
        return;
    }
    if (arg1 < 9) {
        ovl_11_func_800D6F78(arg2[0], 0, arg0, D_80126E54[arg1].x, D_80126E54[arg1].y, 8);
    } else {
        ovl_11_func_800F6D14(arg0, (u16) arg2[0], D_80126E54[arg1].x, D_80126E54[arg1].y);
    }
    switch (ovl_11_func_800D583C(arg2[0])) {
    case 2:
        ovl_11_func_800F7094(gActiveRenderContext->field_D8 + 0x58, D_80126E54[arg1].x, D_80126E54[arg1].y, arg2[2]);
        return;
    case 0:
        if (ovl_11_func_800F8404(arg1) == 2) {
            ovl_11_func_800F7094(gActiveRenderContext->field_D8 + 0x58, D_80126E54[arg1].x, D_80126E54[arg1].y, arg2[2]);
        }
        /* fallthrough */
    case 1:
        return;
    }
}
