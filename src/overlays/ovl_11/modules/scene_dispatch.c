/* Loaded-scene setup and effect dispatch (ovl_11:800DBFC4-800DC388).
 * The kind/id choices pair effect initialization with its per-frame update.
 * Includes the common render-color writer and collision-tolerance hook.
 * Interface/constraints: ovl11_scene_setup_install.h.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */

#include "common.h"
#include "ovl11_scene_setup_install.h"
#include "game_types.h"
#include "projected_primitives.h"
#include "cd_audio.h"
#include "ovl11_background_color_cycle.h"
#include "ovl11_coordinate_clamp.h"
#include "ovl11_scene_record_visibility.h"
#include "ovl11_scene_rotation_ramps.h"
#include "ovl11_scene_animation_pair.h"
#include "ovl11_scene_animation_cycle.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/memory.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_time_of_day_fade.h"
#include "ovl11_cell_sprite_dispatch.h"
#include "ovl11_scene_triplet_interpolation.h"
#include "surface_collision.h"

void Ovl11SetSceneCollisionTolerance(void) {
    SetSurfaceHeightTolerance(0xC8);
}

void Ovl11SceneSetupNoOp(void) {
}

void Ovl11InstallSceneSetup(u16 resourceIndex, u16 unused, s32 alreadyLoaded) {
    u8 *base = (u8 *)&D_8007AFF0;
    struct Ovl11SceneHeader *header;

    *(Ovl25SetupStatePair *)(base + 0x25378) =
        *(Ovl25SetupStatePair *)(base + 0x2479C);
    *(s32 *)(base + 0x2537C) = *(s32 *)(base + 0x247A0);
    *(u16 *)(base + 0x25380) = *(u16 *)(base + 0x247A4);
    *(u16 *)(base + 0x25382) = *(u16 *)(base + 0x247A6);
    *(u16 *)(base + 0x25384) = *(u16 *)(base + 0x247A8);
    *(u16 *)(base + 0x25386) = *(u16 *)(base + 0x247AA);
    Ovl11LoadAreaResourceSet(resourceIndex, alreadyLoaded & 0xFFFF);
    InitProjectedPrimitiveSource(gOvl11SceneGeometry, (u8 *)D_8007AFF0 + 0x2000);
    Ovl11SetCoordinateClamp(&gLoadedAreaClampFirstCorner, &gLoadedAreaClampSecondCorner);
    EnableSurfaceCells((Ovl11SceneGrid **)gOvl11SceneGeometry);
    Ovl11SetRenderContextColor(0, 0, 0);
    header = *(struct Ovl11SceneHeader **)(base + 0x25388);
    switch (header->kind) {
    case 15:
        if (header->id == 0xE5) {
            Ovl11ResetSceneTripletInterpolation();
            return;
        }
        Ovl11InitTimeOfDayFade();
        return;
    case 44:
        Ovl11ResetBackgroundColorCycle();
        return;
    }
}

void Ovl11SetRenderContextColor(s32 red, s32 green, s32 blue) {
    u8 *first = (u8 *)&gRenderContexts[0];
    u8 *second = (u8 *)&gRenderContexts[1];

    first[0x19] = red;
    first[0x1A] = green;
    first[0x1B] = blue;
    second[0x19] = red;
    second[0x1A] = green;
    second[0x1B] = blue;
    first[0x18] = 1;
    gRenderContexts[1].unk18 = 1;
}

#define STATE ((struct Ovl11EventDispatchState *)D_8006C838)
#define HEADER ((struct Ovl11SceneHeader *)((Ovl11FarInitView *)&D_8007AFF0)->data)

void Ovl11UpdateSceneEffects(void) {
    s32 track;
    s32 recordIndex;

    /* Keep the original unsigned subtraction followed by signed narrowing. */
    switch ((s16)((u16)HEADER->kind - 1)) {
    case 0:
        if (STATE->flags & 0x4000) {
            track = -1;
            STATE->flags &= ~0x4000;
            for (recordIndex = 0; recordIndex < 10; recordIndex++) {
                /* Three legacy 0x3C-byte base elements make one 0xB4 record.
                 * Preserve this address origin; rebasing through records[]
                 * changes the compiler's induction-address construction. */
                if (((struct Ovl11EventDispatchState *)&D_8006C838[recordIndex * 3])->records[0].type == 0x109 &&
                    ((struct Ovl11EventDispatchState *)&D_8006C838[recordIndex * 3])->records[0].active == 0) {
                    track = 0x13;
                    break;
                }
            }
            if (STATE->flags & 0x8000) {
                track = 0x14;
            }
            if (track != -1) {
                StartCdAudioTrack(track);
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

#undef HEADER
#undef STATE
