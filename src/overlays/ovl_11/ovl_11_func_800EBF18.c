#include "common.h"

s32 ovl_11_func_800C3548(s32 arg0);
void ovl_11_func_800EFABC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800EBF18(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 i;

    for (i = 1; i < 0x25; i++) {
        if (i != 0x1B && i != 0x21) {
            if ((u32)(arg0 - 0x1F) < 2U) {
                if (ovl_11_func_800C3548(i) == 1) {
                    ovl_11_func_800EFABC(0x1F, i, arg3, arg1);
                } else {
                    ovl_11_func_800EFABC(0x20, i, arg2, arg1);
                }
            } else {
                ovl_11_func_800EFABC(arg0, i, arg2, arg1);
            }
        }
    }
    if ((u16)(arg0 - 0x1F) < 2U) {
        for (i = 0; i < 7; i++) {
            ovl_11_func_800EFABC(0x2B, i, arg2, arg1);
        }
    }
    return 1;
}
