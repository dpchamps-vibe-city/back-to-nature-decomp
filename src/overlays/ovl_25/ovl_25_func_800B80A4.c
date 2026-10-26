#include "common.h"
#include "screen_fade.h"
#include "gte_scene_state.h"
#include "master_volume.h"
#include "ovl25_background_color_cycle.h"
#include "ovl25_scene_display_pipeline.h"

void ovl_25_func_800B8478(void);
void ovl_25_func_800B93E4(void);

void ovl_25_func_800B80A4(void) {
    u8 var_s0;

    Ovl25UpdateSceneCameraOffset();
    Ovl25ApplySceneCamera();
    ApplyGteSceneState();
    Ovl25DrawSceneGeometry();
    var_s0 = IsScreenFadeComplete() == 1;
    if (GetMasterVolumeRampState() == 0) {
        var_s0++;
    }
    ovl_25_func_800B8478();
    ovl_25_func_800B93E4();
    if (var_s0 == 2) {
        s32 *base;

        base = (s32 *)&D_8006C838;
        base[0x448C >> 2]++;
    }
    Ovl25SetBackgroundColor(0, 0xA5, 0xD5);
}
