#include "common.h"
#include "screen_fade.h"
#include "gte_scene_state.h"
#include "master_volume.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl21_scene_camera_geometry.h"

void ovl_21_func_800B8320(void);
void ovl_21_func_800B9798(s16 arg0, s16 arg1);
void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B7FC8(void) {
    u8 var_s0;

    Ovl21UpdateSceneCameraOffset();
    Ovl21ApplySceneCamera();
    ApplyGteSceneState();
    Ovl21DrawSceneGeometry();
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
