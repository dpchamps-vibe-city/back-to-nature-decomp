#include "common.h"
#include "cd_audio.h"

s32 ovl_11_func_800E6AB0(s32 arg0, s32 arg1, s32 arg2) {
    s32 var_s0;

    arg0 = (s16)arg0;
    if (D_8006C904 != 0) {
        return 1;
    }
    var_s0 = 0;
    if (arg2 != 0) {
        StartCdAudioTrack(arg0);
    } else if (GetCdAudioState() == 0) {
        var_s0 = 1;
    }
    return var_s0;
}
