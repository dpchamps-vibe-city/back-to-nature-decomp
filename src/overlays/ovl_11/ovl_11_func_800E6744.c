#include "common.h"

s32 func_8001AF44(u32 arg0);
s32 ovl_11_func_800EFDA0(s32 arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800EFC68(s32, s32);

s32 ovl_11_func_800E6744(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 temp_s1;

    temp_s1 = ovl_11_func_800EFC68((s32) arg0, (s32) arg1);
    if (arg0 == 0x14 && func_8001AF44(0x1BU) == 1 && arg3 != -1) {
        return 0;
    }
    return ovl_11_func_800EFDA0(temp_s1, arg3, arg2);
}
