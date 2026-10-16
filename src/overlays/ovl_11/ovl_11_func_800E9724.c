#include "common.h"

s32 ovl_11_func_800E60A0(s16 arg0, s32 height, s32 arg2, s32 arg3);

s32 ovl_11_func_800E9724(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    return ovl_11_func_800E60A0(arg0, *(s16 *)&D_80129560[arg1],
                              D_80129560[arg2], D_80129560[arg3]);
}
