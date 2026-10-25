#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"

void ovl_21_func_800BB648(void) {
    SpriteDataHeader *header;
    SpriteSourceData *sprite;
    u8 *record;
    s32 i;
    s32 j;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x16, 0, 0x16188, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x16, 0, 0x16188, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    UploadSpriteImages((u8 *)(gGraphicsHeapBase + 0x65CC));
    memcpy(D_800BE108, (u8 *)(gGraphicsHeapBase + 0x4290), 0x233C);
    header = (SpriteDataHeader *)(D_800BE108 + D_800BCCDC[0]);
    InitSpriteSourceData(&D_800C0B2C, header);
    InitSpriteSourceData(&D_800C0B2C - 1, (SpriteDataHeader *)(D_800BE108 + D_800BCCDC[1]));
    for (i = 0; i < 13; i++) {
        InitSpriteSourceData(&D_800C0B2C + i + 1, (SpriteDataHeader *)(D_800BE108 + D_800BCCDC[i + 2]));
    }
    for (i = 0; i < 6; i++) {
        record = D_800C0458 + i * 0x108;
        for (j = 0; j < 3; j++) {
            sprite = (SpriteSourceData *)(record + j * 0x4C + 0x40);
            InitSpriteSourceData(sprite, header);
        }
    }
}
