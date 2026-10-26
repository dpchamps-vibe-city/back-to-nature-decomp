#include "common.h"
#include "cd_audio.h"
#include "ovl11_background_color_cycle.h"
#include "ovl11_scene_record_visibility.h"
#include "ovl11_scene_rotation_ramps.h"
#include "ovl11_scene_animation_pair.h"
#include "ovl11_scene_animation_cycle.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_cell_sprite_dispatch.h"
#include "ovl11_scene_triplet_interpolation.h"
#include "psyq/libgs.h"
#include "psyq/memory.h"
#include "psyq/libcd.h"
#include "ovl11_time_of_day_fade.h"

#define STATE ((struct Ovl11EventDispatchState *)D_8006C838)
#define HEADER ((struct Ovl11SceneHeader *)((Ovl11FarInitView *)&D_8007AFF0)->data)

void ovl_11_func_800DC148(void) {
    s32 sound;
    s32 i;

    switch ((s16)((u16)HEADER->kind - 1)) {
    case 0:
        if (STATE->flags & 0x4000) {
            sound = -1;
            STATE->flags &= ~0x4000;
            for (i = 0; i < 10; i++) {
                if (((struct Ovl11EventDispatchState *)&D_8006C838[i * 3])->records[0].type == 0x109 &&
                    ((struct Ovl11EventDispatchState *)&D_8006C838[i * 3])->records[0].active == 0) {
                    sound = 0x13;
                    break;
                }
            }
            if (STATE->flags & 0x8000) {
                sound = 0x14;
            }
            if (sound != -1) {
                StartCdAudioTrack(sound);
            }
        }
        Ovl11DrawVisibleCellSprites();
        break;
    case 1:
        if (!(STATE->flags & 0x10000)) {
            STATE->flags |= 0x10000;
        }
        break;
    case 14:
        if (HEADER->id == 0xE5) {
            Ovl11StepSceneTripletInterpolation();
        } else {
            Ovl11UpdateTimeOfDayFade();
        }
        break;
    case 5:
        Ovl11DrawFixedCellSpriteGrid();
        break;
    case 43:
        Ovl11StepBackgroundColorCycle();
        break;
    }
    if (STATE->mode == 0xC && !(STATE->flags & 0x40000)) {
        STATE->flags |= 0x40000;
        StartCdAudioTrack(0x15);
    }
    if (STATE->mode == 0x12 && !(STATE->flags & 0x80000)) {
        STATE->flags |= 0x80000;
        StartCdAudioTrack(0x15);
    }
    Ovl11StepSceneAnimationCycle();
    Ovl11StepSceneAnimationPair();
    Ovl11StepSceneZRotationRamp();
    Ovl11StepSceneYRotationRamp();
    Ovl11UpdateSceneRecordVisibilityPair();
}
