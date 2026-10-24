#include "common.h"
s32 ovl_11_func_800E5B84(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800E5704(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800E5870(s16 arg0, s16 arg1, s32 arg2, s16 arg3) {
    ovl_11_func_800E5B84(arg0, arg3, 0, 0);
    return ovl_11_func_800E5704(arg0, arg1, arg2, 12);
}
