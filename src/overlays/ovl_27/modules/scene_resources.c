/*
 * scene_resources — reconstructed translation unit.
 *
 * Resource-load steps for overlay 27's setup dispatcher. Begin/Poll pairs
 * serialize three archive-entry-0 reads through shared CD request state.
 * Sprite reads reuse graphics-heap scratch at +0x4290; scene data goes to
 * D_8007AFF4. The caller owns ordering, completion waits and sound transfer.
 * See ovl27_scene_resources.h for buffer prerequisites and byte ranges.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl27_scene_resources.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_upload.h"

void Ovl27QueueSceneSoundLoads(void) {
    SetSoundCommandReadBuffer(0, gGraphicsHeapBase + 0x4290);
    QueueSoundLoadCommand(0, 0);
    QueueSoundLoadCommand(0x3E8, 0);
    QueueSoundLoadCommand(0x1A, 0);
}

void Ovl27BeginSceneDataRead(void) {
    DrawSync(0);
    ClearOTagR((unsigned long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x2A800, 0x49000, (u8 *)D_8007AFF4, 1, 1);
}

s32 Ovl27PollSceneDataRead(void) {
    return CdReadArchiveBytes(0, 0x2A800, 0x49000, (u8 *)D_8007AFF4, 1, 0) != 0;
}

void Ovl27BeginSharedSpriteRead(void) {
    DrawSync(0);
    ClearOTagR((unsigned long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
}

s32 Ovl27PollSharedSpriteRead(void) {
    if (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0) {
        return 0;
    }
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    return 1;
}

void Ovl27BeginImageBankRead(void) {
    DrawSync(0);
    ClearOTagR((unsigned long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
}

s32 Ovl27PollImageBankRead(void) {
    if (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0) {
        return 0;
    }
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
    return 1;
}

/* Retained empty setup stage: the dispatcher advances before copying. */
void Ovl27BeginPageCopy(void) {
}

s32 Ovl27CopySelectedPage(void) {
    s32 pageIndex;
    s32 sourceOffset;
    u8 *destination;
    void *source;

    if (((D8006C838View *)D_8006C838)->field_18 >= 0) {
        pageIndex = ((D8006C838View *)D_8006C838)->field_18;
        if (pageIndex >= 4) {
            pageIndex = 3;
        }
    } else {
        pageIndex = 0;
    }
    destination = (u8 *)((D8006C838View *)D_8006C838)->field_1C + 6;
    source = gOvl27PageSources[pageIndex].data;
    sourceOffset = pageIndex * 8;
    memcpy(destination, source, 0x1000);
    ((D8006C838View *)D_8006C838)->field_18 = *(s32 *)((u8 *)gOvl27PageSources + sourceOffset);
    return 1;
}
