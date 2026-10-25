#include "common.h"
#include "gte_scene_state.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_scene_camera_geometry.h"
#include "message_window.h"

void ovl_19_func_800B80E8(void) {
    UpdateMessageWindow();
    Ovl19UpdateSceneCameraOffset();
    Ovl19ApplySceneCamera();
    ApplyGteSceneState();
    Ovl19DrawSceneGeometry();
    D_800BCEA8[D_800BF4C4]();
}
