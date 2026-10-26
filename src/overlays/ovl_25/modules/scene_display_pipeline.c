/*
 * scene_display_pipeline — reconstructed translation unit.
 *
 * Scene camera/geometry and staged sprite, geometry and sound resources. The
 * camera chain consumes the same private geometry descriptor initialized by
 * the disc-load stage; persistent sprite copies survive reuse of heap
 * scratch.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl25_scene_display_pipeline.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "gte_scene_state.h"
#include "projected_primitives.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "sound_commands.h"

void Ovl25InitSceneCamera(void) {
    char *base;
    char *p;
    char *src;

    base = (char *)&D_8007AFF0;
    p = base + 0x20000;
    src = (char *)gOvl25InitialSceneRecords;
    *(s32 *)(p + 0x5394) = -0x2328;
    *(s32 *)(p + 0x5398) = -0x1194;
    *(s32 *)(p + 0x539C) = 0;
    *(s32 *)(p + 0x53A0) = 0x200;
    *(s32 *)(p + 0x53A4) = 0;
    *(s16 *)(p + 0x53B6) = 0;
    *(s16 *)(p + 0x53B4) = *(u16 *)(src + 0);
    *(s16 *)(p + 0x53B8) = *(u16 *)(src + 4);
}

void Ovl25UpdateSceneCameraOffset(void) {
    char *far_base;
    MATRIX m0;
    MATRIX m1;
    SVECTOR v;
    VECTOR out;
    long flag;

    m0 = GsIDMATRIX;
    m1 = GsIDMATRIX;
    PushMatrix();
    far_base = (char *)&D_8007AFF0;
    v.vx = 0;
    v.vy = *(u16 *)(far_base + 0x253A0);
    v.vz = 0;
    RotMatrix(&v, &m0);
    SetRotMatrix(&m0);
    SetTransMatrix(&m1);
    v.vx = 0;
    v.vy = 0;
    v.vz = *(u16 *)(far_base + 0x25394);
    RotTrans(&v, &out, &flag);
    PopMatrix();
    *(u16 *)(far_base + 0x253AC) = out.vx;
    *(s16 *)(far_base + 0x253AE) = *(u16 *)(far_base + 0x25398) + *(u16 *)(far_base + 0x253B6);
    *(u16 *)(far_base + 0x253B0) = out.vz;
}

void Ovl25ApplySceneCamera(void) {
    char *far_base = (char *)&D_8007AFF0;

    SetCameraEyeAndRoll(*(s16 *)(far_base + 0x253AC) + *(s16 *)(far_base + 0x253B4),
                  *(s16 *)(far_base + 0x253AE),
                  *(s16 *)(far_base + 0x253B0) + *(s16 *)(far_base + 0x253B8),
                  0);
    SetCameraTarget(*(s16 *)(far_base + 0x253B4),
                  *(s16 *)(far_base + 0x253B6),
                  *(s16 *)(far_base + 0x253B8));
}

void Ovl25DrawSceneGeometry(void) {
    char *far_base;
    VECTOR v0;
    VECTOR v1;

    far_base = (char *)&D_8007AFF0;
    v0.vx = *(s16 *)(far_base + 0x253AC) + *(s16 *)(far_base + 0x253B4);
    v0.vy = *(s16 *)(far_base + 0x253AE);
    v0.vz = *(s16 *)(far_base + 0x253B0) + *(s16 *)(far_base + 0x253B8);
    v1.vx = *(s16 *)(far_base + 0x253B4);
    v1.vy = *(s16 *)(far_base + 0x253B6);
    v1.vz = *(s16 *)(far_base + 0x253B8);
    DrawCulledProjectedPrimitives((ProjectedPrimitiveSource *)&gOvl25SceneGeometry, &v0, &v1);
}

void Ovl25LoadSharedSpriteArchive(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl25SharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData((SpriteSourceData *)D_800C021C, (SpriteDataHeader *)gOvl25SharedSpriteData);
    SelectSpriteAnimation((ObjectState *)D_800C021C, 0x17);
}

void Ovl25UploadCommonSpriteImages(void) {
    DrawSync(0);
    ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {

    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
}

void Ovl25LoadObjectSpriteSet(void) {
    SpriteDataHeader *header;
    s32 i;
    s32 *offsets;
    SpriteSourceData *current;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x1A, 0, 0x124D0, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x1A, 0, 0x124D0, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x5F18));
    memcpy(gOvl25ObjectSpriteData, (u8 *)(gGraphicsHeapBase + 0x4290), 0x1C88);
    InitSpriteSourceData((SpriteSourceData *)&Ovl25Sprite800C024C, (SpriteDataHeader *)(gOvl25ObjectSpriteData + ((s32 *)D_800BCD68)[0]));
    InitSpriteSourceData((SpriteSourceData *)&Ovl25Sprite800C024C + 1, (SpriteDataHeader *)(gOvl25ObjectSpriteData + ((s32 *)D_800BCD68)[1]));
    InitSpriteSourceData((SpriteSourceData *)&Ovl25Sprite800C024C + 2, (SpriteDataHeader *)(gOvl25ObjectSpriteData + ((s32 *)D_800BCD68)[1]));
    InitSpriteSourceData((SpriteSourceData *)&Ovl25Sprite800C024C + 3, (SpriteDataHeader *)(gOvl25ObjectSpriteData + ((s32 *)D_800BCD68)[2]));
    for (i = 0; i < 6; i++) {
        header = (SpriteDataHeader *)(gOvl25ObjectSpriteData + ((s32 *)D_800BCD68)[i + 3]);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)&Ovl25Sprite800C024C - 0x3E0 + i * 0x78), header);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)&Ovl25Sprite800C024C - 0x3B0 + i * 0x78), header);
    }
    for (i = 9; i < 11; i++) {
        header = (SpriteDataHeader *)(gOvl25ObjectSpriteData + ((s32 *)D_800BCD68)[i]);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)&Ovl25Sprite800C013C + (i - 9) * 0x78), header);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)&Ovl25Sprite800C013C + 0x30 + (i - 9) * 0x78), header);
    }
    i = 0;
    while ((current = (SpriteSourceData *)&Ovl25Sprite800C0334 + i, (u32)i < 6)) {
        offsets = (s32 *)D_800BCD68;
        InitSpriteSourceData(current, (SpriteDataHeader *)(gOvl25ObjectSpriteData + offsets[i + 11]));
        i++;
    }
}

void func_8001E340(void **arg0);
void SetVal8005E51C(s32 arg0);

/* Streams two CD-sector runs from the D_800BBED0 position table, copies the
 * 0xBDC-byte D_8007AFF0 work area into D_8009F78C, then publishes that copy
 * and a 0x39 value into the D_8007AFF0+0x25388/0x2547C far-buffer slots. The
 * empty memory barrier keeps the BFA8 argument loads after the copy, which is
 * what places the copy source in $a0. */
void Ovl25LoadSceneGeometry(void) {
    s32 *p;
    u8 *dst;
    char *far_base;

    p = gOvl25SceneDiscPositions;
    CdReadDiscSectorsBlocking(0, p[0], p[1] - p[0], 0, D_8007AFF0);
    CdReadDiscSectorsBlocking(0, p[1], p[2] - p[1], 0, gGraphicsHeapBase + 0x4290);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
    memcpy(&gLoadedAreaHeaderCopy, (void *)D_8007AFF0, 0xBDC);
    dst = &gOvl25SceneGeometry;
    __asm__("" : : : "memory");
    InitProjectedPrimitiveSource(dst, (void *)(D_8007AFF0 + 0x2000));
    far_base = (char *)&D_8007AFF0;
    *(s32 *)(far_base + 0x2547C) = 0x39;
    *(s32 *)(far_base + 0x25388) = (s32)&gLoadedAreaHeaderCopy;
    func_8001E340((void **)dst);
    SetVal8005E51C(0xC8);
}

void Ovl25QueueSceneSoundLoads(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x12, 1);
}
