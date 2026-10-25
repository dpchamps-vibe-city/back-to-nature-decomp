#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"

s32 ovl_27_func_800B8C6C(void) {
    s32 *base;
    s32 v;

    BeginScreenFadeIncrease(10, 0, 2);
    BeginMasterVolumeFadeOut(10);
    base = (s32 *)&D_8006C838;
    v = base[0x4488 >> 2] + 1;
    base[0x4488 >> 2] = v;
    return v;
}
