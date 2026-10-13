#include "common.h"

s32 ovl_11_func_800F07EC(s16 arg0, s32 arg1, s32 arg2, u32 *arg3);

s32 ovl_11_func_800EC304(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    return ovl_11_func_800F07EC(arg1, D_80129560[arg2], D_80129560[arg3],
                              (u32 *) &D_80129560[(s16) arg0]);
}
