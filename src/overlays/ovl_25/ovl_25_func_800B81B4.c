#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"

void ovl_25_func_800B93E4(void);

s32 ovl_25_func_800B81B4(void) {
    s32 *base;
    s32 v;

    ovl_25_func_800B93E4();
    BeginScreenFadeFeedback(10);
    BeginMasterVolumeFadeOut(10);
    base = (s32 *)&D_8006C838;
    v = base[0x448C >> 2] + 1;
    base[0x448C >> 2] = v;
    return v;
}
