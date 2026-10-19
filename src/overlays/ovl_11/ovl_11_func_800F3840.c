#include "common.h"

s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
/* This call supplies a fourth, unused argument in the original code. */
s32 ovl_11_func_800E6F34();

void ovl_11_func_800F3840(void) {
    ovl_11_func_800E6EE0(0x103, 1, 3, 1);
    ovl_11_func_800E6F34(0x103, 0, 0, 0);
    ((struct Ovl11F3840State *)D_8006C838)->field_E6CB =
        ((struct Ovl11F3840State *)D_8006C838)->field_44B8 + 1;
}
