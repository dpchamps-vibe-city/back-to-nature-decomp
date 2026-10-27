/*
 * Overlay-21 scene resources and camera/geometry service.
 * Setup loads retained sprite headers and textures, publishes the geometry
 * descriptor consumed by drawing, then queues sound before camera startup.
 * See ovl21_scene_resources.h, ovl21_scene_camera_geometry.h and the
 * membership/boundary evidence in notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/memory.h"
#include "gte_scene_state.h"
#include "projected_primitives.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "sound_commands.h"
#include "ovl21_scene_camera_geometry.h"
#include "ovl21_scene_resources.h"
#include "surface_collision.h"

/* Offsets from D_8007AFF0. Word resets and low-halfword reads intentionally
 * retain their original widths; computed eye coordinates wrap to halfwords.
 * The two reset-only words at +2539C/+253A4 remain unnamed. */
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

void Ovl21InitSceneCamera(void) {
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

void Ovl21UpdateSceneCameraOffset(void) {
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

void Ovl21ApplySceneCamera(void) {
    char *camera = (char *)&D_8007AFF0;

    SetCameraEyeAndRoll(*(s16 *)(camera + CAMERA_OFFSET_X) + *(s16 *)(camera + CAMERA_TARGET_X),
                       *(s16 *)(camera + CAMERA_EYE_Y),
                       *(s16 *)(camera + CAMERA_OFFSET_Z) + *(s16 *)(camera + CAMERA_TARGET_Z),
                       0);
    SetCameraTarget(*(s16 *)(camera + CAMERA_TARGET_X),
                    *(s16 *)(camera + CAMERA_TARGET_Y),
                    *(s16 *)(camera + CAMERA_TARGET_Z));
}

void Ovl21DrawSceneGeometry(void) {
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
    DrawCulledProjectedPrimitives((ProjectedPrimitiveSource *)&gOvl21SceneGeometry, &eye, &target);
}

void Ovl21LoadSceneSpriteArchive(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl21SceneSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData(&gOvl21PrimarySceneSprite, (SpriteDataHeader *)gOvl21SceneSpriteData);
}

void Ovl21UploadSceneSpriteChunk(void) {
    DrawSync(0);
    ClearOTagR((u32 *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, gGraphicsHeapBase + 0x4290, 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0, 0x2000, gGraphicsHeapBase + 0x4290, 1, 0) == 0);
    UploadSpriteImages(gGraphicsHeapBase + 0x4290);
}

void Ovl21LoadUiAndObjectSprites(void) {
    SpriteDataHeader *header;
    SpriteSourceData *sprite;
    u8 *record;
    s32 index;
    s32 spriteIndex;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x16, 0, 0x16188, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x16, 0, 0x16188, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x65CC));
    memcpy(gOvl21UiSpriteData, (u8 *)(gGraphicsHeapBase + 0x4290), 0x233C);
    header = (SpriteDataHeader *)(gOvl21UiSpriteData + gOvl21UiSpriteOffsets[0]);
    /* This anchor is the second UI source. Preserve the -1 and +1 biases
     * and the 0x30-byte source stride rather than inventing a new array. */
    InitSpriteSourceData(&gOvl21UiSpriteAnchor, header);
    InitSpriteSourceData(&gOvl21UiSpriteAnchor - 1, (SpriteDataHeader *)(gOvl21UiSpriteData + gOvl21UiSpriteOffsets[1]));
    for (index = 0; index < 13; index++) {
        InitSpriteSourceData(&gOvl21UiSpriteAnchor + index + 1, (SpriteDataHeader *)(gOvl21UiSpriteData + gOvl21UiSpriteOffsets[index + 2]));
    }
    /* All eighteen embedded sources intentionally reuse table entry zero. */
    for (index = 0; index < 6; index++) {
        record = D_800C0458 + index * 0x108;
        for (spriteIndex = 0; spriteIndex < 3; spriteIndex++) {
            sprite = (SpriteSourceData *)(record + spriteIndex * 0x4C + 0x40);
            InitSpriteSourceData(sprite, header);
        }
    }
}

/* The name-bound descriptor address and existing compiler barrier preserve
 * the target's copy/binding sequence without changing the descriptor view. */
void Ovl21LoadSceneGeometry(void) {
    s32 *discPositions;
    u8 *geometry;

    discPositions = gOvl21SceneDiscPositions;
    CdReadDiscSectorsBlocking(0, discPositions[0], discPositions[1] - discPositions[0], 0, D_8007AFF0);
    CdReadDiscSectorsBlocking(0, discPositions[1], discPositions[2] - discPositions[1], 0, gGraphicsHeapBase + 0x4290);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
    memcpy(&gLoadedAreaHeaderCopy, (void *)D_8007AFF0, 0xBDC);
    geometry = &gOvl21SceneGeometry;
    __asm__("" : : : "memory");
    InitProjectedPrimitiveSource(geometry, (void *)(D_8007AFF0 + 0x2000));
    EnableSurfaceCells((void **)geometry);
    SetSurfaceHeightTolerance(0xC8);
}

void Ovl21QueueSceneSound(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x12, 1);
}
