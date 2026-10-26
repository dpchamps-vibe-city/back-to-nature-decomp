/*
 * Save-menu archive resources: card-icon bytes and five sprite sources.
 * The loader hooks are empty in this build but remain real calls in the
 * initialization protocol. See ovl15_save_sprite_resources.h for effects and
 * prerequisites; membership/boundary evidence is in notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"
#include "ovl15_save_sprite_resources.h"

void Ovl15MenuResourceHook0(void);
void Ovl15MenuResourceHook1(void);
s32 Ovl15LoadCardIconResources(void);
void Ovl15MenuResourceHook2(void);
void Ovl15LoadMenuSpriteResources(void);

void Ovl15LoadMenuResources(s16 loadSprites) {
    Ovl15MenuResourceHook0();
    Ovl15MenuResourceHook1();
    Ovl15LoadCardIconResources();
    Ovl15MenuResourceHook2();
    if (loadSprites != 0) {
        Ovl15LoadMenuSpriteResources();
    }
}

void Ovl15MenuResourceHook0(void) {
}

void Ovl15MenuResourceHook1(void) {
}

s32 Ovl15LoadCardIconResources(void) {
    s32 completion;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x10, 0, 0x900, gOvl15CardIconResourceBuffer, 1, 1);
    do {
        completion = CdReadArchiveBytes(0x10, 0, 0x900, gOvl15CardIconResourceBuffer, 1, 0);
    } while (completion == 0);
    return completion;
}

void Ovl15MenuResourceHook2(void) {
}

void Ovl15LoadMenuSpriteResources(void) {
    u32 sourceIndex;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0xE, 0, 0x8E90, gOvl15TransferBuffer, 1, 1);
    do {
    } while (CdReadArchiveBytes(0xE, 0, 0x8E90, gOvl15TransferBuffer, 1, 0) == 0);
    UploadSpriteImages(gOvl15LoadedSpriteImages);
    /* The image stream follows the records in the loaded archive. Retain the
     * interior-label expression: both labels refer to the shared workspace. */
    memcpy(gOvl15MenuSpriteData, &gOvl15LoadedSpriteImages[-0x2D74], 0x2D74);
    for (sourceIndex = 0; sourceIndex < 5; sourceIndex++) {
        InitSpriteSourceData(&gOvl15MenuSpriteSources[sourceIndex],
            (SpriteDataHeader *)&gOvl15MenuSpriteData[gOvl15MenuSpriteOffsets[sourceIndex]]);
    }
}
