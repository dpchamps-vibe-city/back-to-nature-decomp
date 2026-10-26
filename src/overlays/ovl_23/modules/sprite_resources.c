/*
 * Sprite-bank bring-up and pause services for this overlay's record arena.
 * Per-record sources live at arena+0x40 (six, stride 0x50) and arena+0x214
 * (six, stride 0x44); the scene source is arena+0x3B4. The loader and flag
 * walkers deliberately retain their different base-relative expressions.
 * Original data remains external; membership: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl23_sprite_resources.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "sound_commands.h"

void Ovl23PauseActorSprites(void) {
    u8 *sources44;
    u8 *sources50;
    u32 base;
    s32 i;

    base = (u32)gOvl23ActorSpriteSourceBase;
    sources50 = (u8 *)(base - 0x1D4);
    sources44 = (u8 *)base;
    for (i = 5; i >= 0; i--) {
        AddSpriteSourceFlags((u16 *)sources44, 4);
        AddSpriteSourceFlags((u16 *)sources50, 4);
        sources50 += 0x50;
        sources44 += 0x44;
    }
}

void Ovl23ResumeActorSprites(void) {
    u8 *sources44;
    u8 *sources50;
    u32 base;
    s32 i;

    base = (u32)gOvl23ActorSpriteSourceBase;
    sources50 = (u8 *)(base - 0x1D4);
    sources44 = (u8 *)base;
    for (i = 5; i >= 0; i--) {
        ClearSpriteSourceFlags((u16 *)sources44, 4);
        ClearSpriteSourceFlags((u16 *)sources50, 4);
        sources50 += 0x50;
        sources44 += 0x44;
    }
}

void Ovl23DrawPausedSpriteFrame(void) {
    u8 *arena;

    SelectSpriteAnimation((ObjectState *)&gOvl23SceneSpriteSource, 5);
    arena = gOvl23SceneSpriteSource - 0x3B4;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl23SceneSpriteSource, arena[0x3B8],
                  arena[0x3B9], 0, 0);
}

void Ovl23LoadSharedSpriteBank(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl23SharedSpriteBank, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData(&gOvl23SharedBankSpriteSource, (SpriteDataHeader *)gOvl23SharedSpriteBank);
}

void Ovl23LoadSpriteImageBank(void) {
    DrawSync(0);
    ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
}

/* Byte-aligned bank assignment preserves the original unaligned copy, unlike
 * the shared bank's two separately bounded memcpy operations above. */
typedef struct {
    u8 bytes[0x29A8];
} Ovl23SpriteBankBlob;

void Ovl23LoadActorSpriteBank(void) {
    s32 i;
    s32 headerOffsetIndex;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x18, 0, 0x1EC1C, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x18, 0, 0x1EC1C, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x6C38));
    *(Ovl23SpriteBankBlob *)gOvl23ActorSpriteBank = *(Ovl23SpriteBankBlob *)(gGraphicsHeapBase + 0x4290);
    InitSpriteSourceData((SpriteSourceData *)gOvl23SceneSpriteSource, (SpriteDataHeader *)(gOvl23ActorSpriteBank + gOvl23ActorSpriteHeaderOffsets[3]));
    for (i = 0; i < 6; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl23SceneSpriteSource + i * 0x50 - 0x374), (SpriteDataHeader *)(gOvl23ActorSpriteBank + gOvl23ActorSpriteHeaderOffsets[0]));
        headerOffsetIndex = 8;
        if (i == 2) {
            headerOffsetIndex = 4;
        }
        InitSpriteSourceData((SpriteSourceData *)(gOvl23SceneSpriteSource + i * 0x44 - 0x1A0), (SpriteDataHeader *)(gOvl23ActorSpriteBank + *(s32 *)((u8 *)gOvl23ActorSpriteHeaderOffsets + headerOffsetIndex)));
    }
}

void Ovl23QueueSoundBankLoads(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x12, 1);
}
