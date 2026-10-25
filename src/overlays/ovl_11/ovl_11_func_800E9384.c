#include "common.h"
#include "game_flags.h"

s32 ovl_11_func_800E9384(s16 arg0, s16 arg1, s32 arg2) {
    if (arg0 == 0) {
        SetGameFlag(arg2 & 0xFFFF, arg1 != 0);
        return 1;
    }
    return GetGameFlag(arg2 & 0xFFFF);
}
