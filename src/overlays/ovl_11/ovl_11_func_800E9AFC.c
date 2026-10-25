#include "common.h"
#include "master_volume.h"

s32 ovl_11_func_800E9AFC(s16 arg0, s16 arg1, s32 arg2) {
    s32 result;

    result = 1;
    if (arg2 != 0) {
        if (arg0 != 0) {
            BeginMasterVolumeFadeIn((s32) arg1);
        } else {
            BeginMasterVolumeFadeOut((s32) arg1);
        }
    } else {
        result = GetMasterVolumeRampState() == 0;
    }
    return result;
}
