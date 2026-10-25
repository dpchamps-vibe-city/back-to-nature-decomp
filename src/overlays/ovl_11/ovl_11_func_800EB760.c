#include "common.h"
#include "game_types.h"

void ovl_11_func_800EFABC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800EB760(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    ovl_11_func_800EFABC(arg0, gOvl11ValueSlots[arg1], arg2, arg3);
    return 1;
}
