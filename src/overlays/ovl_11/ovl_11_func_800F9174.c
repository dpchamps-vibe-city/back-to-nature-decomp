#include "common.h"

s32 ovl_11_func_800F8404(s32 arg0);
void ovl_11_func_800F9210(s32 arg0, s32 arg1, s32 arg2);

void ovl_11_func_800F9174(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3, s32 arg4) {
    s32 temp_s0;

    temp_s0 = ovl_11_func_800F8404(arg3);
    if (ovl_11_func_800F8404(arg1) == 2) {
        if (temp_s0 != 2) {
            ovl_11_func_800F9210((s32) *arg2, (s32) *arg0, arg4);
        }
    } else if (temp_s0 == 2) {
        ovl_11_func_800F9210((s32) *arg0, (s32) *arg2, arg4);
    }
}
