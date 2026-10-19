#include "common.h"

s32 ovl_11_func_800F1BD0(s32 arg0);
s32 ovl_11_func_800F1C48(s32 arg0);
s32 ovl_11_func_800F1CC4(s32 arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800F1B28(s32 arg0) {
    if (ovl_11_func_800F1BD0(arg0) == 0) {
        return 0;
    }
    if (ovl_11_func_800F1C48(arg0) == 0) {
        return 0;
    }
    if (ovl_11_func_800F1CC4(arg0) == 0) {
        return 0;
    }
    if (arg0 & 0x20000) {
        if (ovl_11_func_800E6EE0(0, 0, 1, 0) == 0) {
            return 0;
        }
    }
    if (arg0 & 0x10000) {
        if (ovl_11_func_800E6EE0(0, 1, 0, 0) == 0) {
            return 0;
        }
    }
    return 1;
}
