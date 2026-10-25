#include "common.h"
#include "gte_scene_state.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl21_scene_camera_geometry.h"
#include "message_window.h"

void ovl_21_func_800B806C(void) {
    UpdateMessageWindow();
    Ovl21UpdateSceneCameraOffset();
    Ovl21ApplySceneCamera();
    ApplyGteSceneState();
    Ovl21DrawSceneGeometry();
    D_800BCB6C[(s16) *D_800C0448]();
}
