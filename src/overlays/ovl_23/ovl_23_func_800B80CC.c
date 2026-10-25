#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"

s32 ovl_23_func_800B80CC(void) {
    s32 *base;
    s32 v;

    BeginScreenFadeFeedback(10);
    BeginMasterVolumeFadeOut(10);
    base = (s32 *)&D_8006C838;
    v = base[0x448C >> 2] + 1;
    base[0x448C >> 2] = v;
    return v;
}
