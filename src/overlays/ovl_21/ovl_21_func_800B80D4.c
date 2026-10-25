#include "common.h"
#include "screen_fade.h"
#include "gte_scene_state.h"
#include "master_volume.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl21_scene_camera_geometry.h"

void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B80D4(void) {
    Ovl21UpdateSceneCameraOffset();
    Ovl21ApplySceneCamera();
    ApplyGteSceneState();
    Ovl21DrawSceneGeometry();
    ovl_21_func_800BA4C0();
    BeginScreenFadeFeedback(0xA);
    BeginMasterVolumeFadeOut(0xA);
    ((Ovl21MainStateView *) D_8006C838)->state++;
}
