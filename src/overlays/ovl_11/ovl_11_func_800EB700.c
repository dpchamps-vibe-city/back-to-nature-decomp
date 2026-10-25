#include "common.h"

s32 ovl_11_func_800EFDA0(s32 arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_800EFC68(s32 arg0, s32 arg1);

s32 ovl_11_func_800EB700(s16 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return ovl_11_func_800EFDA0(ovl_11_func_800EFC68(arg0, gOvl11ValueSlots[(s16)arg1]), arg3, arg2);
}
