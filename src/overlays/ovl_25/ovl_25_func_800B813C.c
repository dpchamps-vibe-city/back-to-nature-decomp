#include "common.h"
#include "gte_scene_state.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl25_vram_animation_slots.h"
#include "ovl25_background_color_cycle.h"
#include "ovl25_scene_display_pipeline.h"

void ovl_25_func_800B813C(void) {
    UpdateMessageWindow();
    Ovl25UpdateSceneCameraOffset();
    Ovl25ApplySceneCamera();
    ApplyGteSceneState();
    Ovl25DrawSceneGeometry();
    Ovl25UpdateVramAnimations();
    D_800BCBEC[D_800BFE44_view.selector]();
    Ovl25StepBackgroundColorCycle();
}
