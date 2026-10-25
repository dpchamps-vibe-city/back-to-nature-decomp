#include "common.h"
#include "screen_fade.h"
#include "gte_scene_state.h"
#include "master_volume.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_scene_camera_geometry.h"
#include "ovl19_value_table_display.h"

void ovl_19_func_800B8064(void) {
    u8 var_s0;

    Ovl19UpdateSceneCameraOffset();
    Ovl19ApplySceneCamera();
    ApplyGteSceneState();
    Ovl19DrawSceneGeometry();
    Ovl19DrawValueTable();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    if (var_s0 == 2) {
        ((Ovl19CompletionCounterView *) D_8006C838)->counter++;
    }
}
