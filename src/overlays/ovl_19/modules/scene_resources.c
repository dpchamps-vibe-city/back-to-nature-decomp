/*
 * Overlay-19 scene resources, original 800BB318-800BBD14.
 * Sprite/archive and queued sound setup precede the camera/render lifecycle.
 * The geometry loader, camera-driven draw path and surface queries share
 * one descriptor; they now live together rather than in separate modules.
 * See ovl19_scene_resources.h and ovl19_scene_camera_geometry.h for sequencing,
 * state changes, units and loading prerequisites.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "gte_scene_state.h"
#include "projected_primitives.h"
#include "ovl19_scene_camera_geometry.h"
#include "ovl19_scene_resources.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "sound_commands.h"

/* Offsets from D_8007AFF0, not independent globals. The distance, height and
 * yaw slots are reset as words but consumed as low halfwords. Keep those
 * access widths: the signed/unsigned views deliberately preserve wraparound.
 * The words at +2539C/+253A4 are only reset here; their roles remain unknown. */
enum {
    CAMERA_DISTANCE = 0x25394,
    CAMERA_HEIGHT = 0x25398,
    CAMERA_YAW = 0x253A0,
    CAMERA_OFFSET_X = 0x253AC,
    CAMERA_EYE_Y = 0x253AE,
    CAMERA_OFFSET_Z = 0x253B0,
    CAMERA_TARGET_X = 0x253B4,
    CAMERA_TARGET_Y = 0x253B6,
    CAMERA_TARGET_Z = 0x253B8
};

/* Collision engine contracts; no linkage changes are inferred by folding. */
void SetVal8005E2E8(s32 tolerance);
s32 func_8001E4C0(void *geometry, s32 *position);
s32 func_8001E38C(void);

void Ovl19InitSceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    *(s32 *)(camera + CAMERA_DISTANCE) = -0x2328;
    *(s32 *)(camera + CAMERA_HEIGHT) = -0x1194;
    *(s32 *)(camera + 0x2539C) = 0;
    *(s32 *)(camera + CAMERA_YAW) = -0x200;
    *(s32 *)(camera + 0x253A4) = 0;
    *(s16 *)(camera + CAMERA_TARGET_X) = 0;
    *(s16 *)(camera + CAMERA_TARGET_Y) = 0;
    *(s16 *)(camera + CAMERA_TARGET_Z) = 0;
}

void Ovl19UpdateSceneCameraOffset(void) {
    char *camera;
    MATRIX rotation;
    MATRIX identity;
    SVECTOR vector;
    VECTOR rotated;
    long flag;

    rotation = GsIDMATRIX;
    identity = GsIDMATRIX;
    PushMatrix();
    camera = (char *)&D_8007AFF0;
    vector.vx = 0;
    vector.vy = *(u16 *)(camera + CAMERA_YAW);
    vector.vz = 0;
    RotMatrix(&vector, &rotation);
    SetRotMatrix(&rotation);
    SetTransMatrix(&identity);
    vector.vx = 0;
    vector.vy = 0;
    vector.vz = *(u16 *)(camera + CAMERA_DISTANCE);
    RotTrans(&vector, &rotated, &flag);
    PopMatrix();
    *(u16 *)(camera + CAMERA_OFFSET_X) = rotated.vx;
    *(s16 *)(camera + CAMERA_EYE_Y) = *(u16 *)(camera + CAMERA_HEIGHT) + *(u16 *)(camera + CAMERA_TARGET_Y);
    *(u16 *)(camera + CAMERA_OFFSET_Z) = rotated.vz;
}

void Ovl19ApplySceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    SetCameraEyeAndRoll(*(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X),
                       *(s16 *)(camera + CAMERA_EYE_Y),
                       *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z),
                       0);
    SetCameraTarget(*(s16 *)(camera + CAMERA_TARGET_X),
                    *(s16 *)(camera + CAMERA_TARGET_Y),
                    *(s16 *)(camera + CAMERA_TARGET_Z));
}

void Ovl19DrawSceneGeometry(void) {
    char *camera;
    VECTOR eye;
    VECTOR target;

    camera = (char *)&D_8007AFF0;
    eye.vx = *(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X);
    eye.vy = *(s16 *)(camera + CAMERA_EYE_Y);
    eye.vz = *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z);
    target.vx = *(s16 *)(camera + CAMERA_TARGET_X);
    target.vy = *(s16 *)(camera + CAMERA_TARGET_Y);
    target.vz = *(s16 *)(camera + CAMERA_TARGET_Z);
    DrawCulledProjectedPrimitives((ProjectedPrimitiveSource *)&gOvl19SceneGeometry, &eye, &target);
}

void Ovl19AdjustPositionToSurface(s16 *position) {
    s32 query[4];

    query[0] = position[0];
    query[1] = position[1];
    query[2] = position[2];
    query[3] = position[3];
    SetVal8005E2E8(0xC000);
    if (func_8001E4C0(gOvl19SceneGeometry, query)) {
        query[1] = func_8001E38C();
    }
    position[0] = query[0];
    position[1] = query[1];
    position[2] = query[2];
    position[3] = query[3];
}

void Ovl19LoadSceneSpriteArchive(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl19SceneSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData(&gOvl19SceneSpriteSource, (SpriteDataHeader *)gOvl19SceneSpriteData);
}

void Ovl19UploadSceneSpriteChunk(void) {
    DrawSync(0);
    ClearOTagR(gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
}

void Ovl19LoadUiAndActorSprites(void) {
    s32 i;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x14, 0, 0x1AE04, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x14, 0, 0x1AE04, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x52DC));
    *(Ovl19SpriteBlob *)gOvl19UiSpriteData = *(Ovl19SpriteBlob *)(gGraphicsHeapBase + 0x4290);
    InitSpriteSourceData((SpriteSourceData *)gOvl19UiSprite, (SpriteDataHeader *)(gOvl19UiSpriteData + gOvl19UiSpriteOffsets[0]));
    for (i = 0; i < 3; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl19UiSprite + i * 0x40 - 0xF0), (SpriteDataHeader *)(gOvl19UiSpriteData + gOvl19UiSpriteOffsets[i + 2]));
    }
    for (i = 0; i < 2; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl19AuxActorSprites + i * 0x48), (SpriteDataHeader *)(gOvl19UiSpriteData + gOvl19UiSpriteOffsets[1]));
    }
    InitSpriteSourceData(&gOvl19AuxSceneSpriteSource, (SpriteDataHeader *)(gOvl19UiSpriteData + gOvl19UiSpriteOffsets[5]));
}

void func_8001E340(void **arg0);
void SetVal8005E51C(s32 arg0);

/* Keep the descriptor address live across the copy. The existing compiler
 * barrier preserves publication after the complete asset-data transfer. */
void Ovl19LoadSceneGeometry(void) {
    s32 *discPositions;
    u8 *geometry;

    discPositions = gOvl19SceneDiscPositions;
    CdReadDiscSectorsBlocking(0, discPositions[0], discPositions[1] - discPositions[0], 0, D_8007AFF0);
    CdReadDiscSectorsBlocking(0, discPositions[1], discPositions[2] - discPositions[1], 0, gGraphicsHeapBase + 0x4290);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
    memcpy(&D_8009F78C, (void *)D_8007AFF0, 0xBDC);
    geometry = &gOvl19SceneGeometry;
    __asm__("" : : : "memory");
    InitProjectedPrimitiveSource(geometry, (void *)(D_8007AFF0 + 0x2000));
    func_8001E340((void **)geometry);
    SetVal8005E51C(0xC8);
}

void Ovl19QueueSceneSound(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0xF, 1);
}
