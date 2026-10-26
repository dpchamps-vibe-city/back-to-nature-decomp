#include "common.h"
#include "gte_scene_state.h"
#include "cd_audio.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl11_scene_camera_lighting.h"
#include "ovl11_area_mode_sequence.h"
#include "map_texture_camera.h"

void ovl_11_func_800C188C(void);
void ovl_11_func_800C162C(void);
void ovl_11_func_800E3AA4(void);
void ovl_11_func_800DCC5C(void);

void ovl_11_func_800C14A0(void) {
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x18000) {
        gRandomState = 1;
    }
    switch (((struct Ovl11FrameWork *) &D_8007AFF0)->dispatch) {
    case 0:
        ovl_11_func_800C188C();
        if ((((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 5) && (((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 0x18) && (((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 0x19)) {
            ovl_11_func_800C162C();
        }
        Ovl11UpdateAreaMode();
        ovl_11_func_800E3AA4();
        PollCdAudio();
        break;
    case 1:
    case 2:
        break;
    case 3:
        Ovl11UpdateMapCameraMode();
        Ovl11UpdateSceneCameraOffset();
        Ovl11ApplySceneCamera();
        ApplyGteSceneState();
        if (!(((struct Ovl11FrameState *) D_8006C838)->flags & 0x800000)) {
            ovl_11_func_800DCC5C();
            Ovl11DrawSceneGeometry();
        }
        break;
    }
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x40000) {
        ((struct Ovl11FrameState *) D_8006C838)->flags &= 0xFF7BFFFF;
    }
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x20000) {
        ((struct Ovl11FrameState *) D_8006C838)->flags = (((struct Ovl11FrameState *) D_8006C838)->flags & 0xFFFDFFFF) | 0x800000;
    }
}
