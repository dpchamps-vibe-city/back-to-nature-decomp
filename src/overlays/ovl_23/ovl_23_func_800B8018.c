#include "common.h"
#include "screen_fade.h"
#include "master_volume.h"

void ovl_23_func_800B823C(void);
void ovl_23_func_800B9310(void);

void ovl_23_func_800B8018(void) {
    s32 *base;
    u8 var_s0;

    ovl_23_func_800B823C();
    ovl_23_func_800B9310();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        base = (s32 *)&D_8006C838;
        base[0x448C >> 2] += 1;
    }
}
