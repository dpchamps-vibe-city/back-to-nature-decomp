#include "common.h"
#include "screen_fade.h"
#include "gte_scene_state.h"
#include "master_volume.h"

void ovl_21_func_800BB138(void);
void ovl_21_func_800BB250(void);
void ovl_21_func_800BB2B4(void);
void ovl_21_func_800B8320(void);
void ovl_21_func_800B9798(s16 arg0, s16 arg1);
void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B7FC8(void) {
    u8 var_s0;

    ovl_21_func_800BB138();
    ovl_21_func_800BB250();
    ApplyGteSceneState();
    ovl_21_func_800BB2B4();
    ovl_21_func_800B8320();
    ovl_21_func_800B9798(0, 0);
    ovl_21_func_800B9798(1, 0);
    ovl_21_func_800BA4C0();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        ((Ovl21MainStateView *) D_8006C838)->state++;
    }
}
