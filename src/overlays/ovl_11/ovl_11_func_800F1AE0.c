#include "common.h"
#include "game_flags.h"

s32 ovl_11_func_800F13D8(u16 arg0);

s32 ovl_11_func_800F1AE0(s32 arg0) {
    s32 var_s0;
    s32 callRet2;

    var_s0 = arg0 & 0x3FF;
    callRet2 = GetGameFlag(var_s0 + 0xFB);
    if (callRet2 == 1) {
        return 0;
    }
    return ovl_11_func_800F13D8(var_s0) == 0;
}
