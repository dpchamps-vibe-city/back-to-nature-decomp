#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"

void ovl_19_func_800BB8F4(void) {
    s32 i;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x14, 0, 0x1AE04, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x14, 0, 0x1AE04, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x52DC));
    *(Ovl19SpriteBlob *)D_800BE470 = *(Ovl19SpriteBlob *)(gGraphicsHeapBase + 0x4290);
    InitSpriteSourceData((SpriteSourceData *)gOvl19UiSprite, (SpriteDataHeader *)(D_800BE470 + D_800BD058[0]));
    for (i = 0; i < 3; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl19UiSprite + i * 0x40 - 0xF0), (SpriteDataHeader *)(D_800BE470 + D_800BD058[i + 2]));
    }
    for (i = 0; i < 2; i++) {
        InitSpriteSourceData((SpriteSourceData *)(gOvl19AuxActorSprites + i * 0x48), (SpriteDataHeader *)(D_800BE470 + D_800BD058[1]));
    }
    InitSpriteSourceData(&D_800BF6D0, (SpriteDataHeader *)(D_800BE470 + D_800BD058[5]));
}
