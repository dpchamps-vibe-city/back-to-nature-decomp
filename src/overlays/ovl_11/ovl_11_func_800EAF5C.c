#include "common.h"
#include "game_flags.h"

u16 ovl_11_func_800EAF5C(s16 arg0, s32 arg1, s32 arg2, s32 arg3) {
    switch (arg0) {
    case 0:
        SetGameFlag(4, 0);
        SetGameFlag(5, 1);
        SetGameFlag(0xB0, 1);
        break;
    case 1:
        SetGameFlag(4, 1);
        SetGameFlag(5, 0);
        SetGameFlag(0xB0, 0);
        break;
    case 2:
    default:
        SetGameFlag(4, 0);
        SetGameFlag(5, 0);
        SetGameFlag(0xB0, 0);
        break;
    }
    return 1;
}
