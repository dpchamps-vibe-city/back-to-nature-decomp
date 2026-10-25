#include "common.h"
#include "master_volume.h"
#include "sound_commands.h"

s32 ovl_11_func_800E6CAC(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_s1;

    var_s1 = 0;
    if (arg1 == 0) {
        BeginMasterVolumeFadeOut(arg0 ? arg0 : 10);
    } else if (GetMasterVolumeRampState() == 0 || arg1 == 2) {
        StopSoundSequence(0);
        var_s1 = 1;
    }
    return var_s1;
}
