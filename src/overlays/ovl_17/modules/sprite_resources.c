/*
 * Staged sprite/audio resources, ovl_17:800B9FA8..800BA54C.
 * The scene initializer calls the three blocking archive stages in order,
 * then queues the audio bank. Retained bank storage outlives heap scratch;
 * other scene code consumes the published header variants and sprite sources.
 * See ovl17_sprite_resources.h for byte windows, side effects and constraints.
 */
#include "ovl17_sprite_resources.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "sound_driver.h"
#include "sound_commands.h"

void Ovl17LoadSharedSpriteBank(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(gOvl17SharedSpriteBank, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    InitSpriteSourceData((SpriteSourceData *)gOvl17SharedBankSpriteSource, (SpriteDataHeader *)gOvl17SharedSpriteBank);
}

void Ovl17LoadSpriteImageBank(void) {
    DrawSync(0);
    ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {

    } while (CdReadArchiveBytes(0, 0, 0x2000, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4290));
}

typedef struct {
    u8 bytes[0xC28];
} Ovl17RecordSpriteBankBlob;

void Ovl17LoadRecordSpriteBank(void) {
    s32 record;

    DrawSync(0);
    ClearOTagR((u_long *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x12, 0, 0x1E0A8, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x12, 0, 0x1E0A8, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x4EB8));
    /* An aggregate copy retains the bank after scratch is reused. */
    *(Ovl17RecordSpriteBankBlob *) gOvl17RecordSpriteBank = *(Ovl17RecordSpriteBankBlob *)(gGraphicsHeapBase + 0x4290);
    gOvl17SpriteHeaderVariantA = gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[1];
    gOvl17SpriteHeaderVariantB = gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[2];
    InitSpriteSourceData(&gOvl17SceneSpriteSource, (SpriteDataHeader *)(gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[4]));
    InitSpriteSourceData((SpriteSourceData *) gOvl17DisplaySpriteSource, (SpriteDataHeader *)(gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[0]));
    InitSpriteSourceData((SpriteSourceData *)(gOvl17DisplaySpriteSource + 0x60), (SpriteDataHeader *)(gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[3]));
    for (record = 0; record < 6; record++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl17DisplaySpriteSource + record * 0x50 - 0x1E4), (SpriteDataHeader *)(gOvl17RecordSpriteBank + gOvl17SpriteHeaderOffsets[2]));
    }
}

void Ovl17QueueSoundBankLoads(void) {
    RewindSoundLoadSlots(2, 0);
    RewindSoundLoadSlots(1, 0);
    QueueSoundLoadCommand(0x3E7, 0);
    QueueSoundLoadCommand(0x15, 1);
}
