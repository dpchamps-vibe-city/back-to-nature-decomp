/*
 * scene_setup_install — reconstructed translation unit.
 *
 * Install the loaded area scene geometry, initialize scene-specific controls
 * and publish both render-context colors.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_scene_setup_install.h"
#include "game_types.h"
#include "projected_primitives.h"
#include "ovl11_background_color_cycle.h"
#include "ovl11_coordinate_clamp.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/memory.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_time_of_day_fade.h"
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
