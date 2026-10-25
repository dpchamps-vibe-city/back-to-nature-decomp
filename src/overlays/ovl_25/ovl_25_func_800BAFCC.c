#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"
#include "sprite_source.h"
#include "sprite_upload.h"

void ovl_25_func_800BAFCC(void) {
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
    memcpy(D_800BE1B4, (u8 *)(gGraphicsHeapBase + 0x4290), 0x1C88);
    InitSpriteSourceData((SpriteSourceData *)&Ovl25Sprite800C024C, (SpriteDataHeader *)(D_800BE1B4 + ((s32 *)D_800BCD68)[0]));
    InitSpriteSourceData((SpriteSourceData *)&Ovl25Sprite800C024C + 1, (SpriteDataHeader *)(D_800BE1B4 + ((s32 *)D_800BCD68)[1]));
    InitSpriteSourceData((SpriteSourceData *)&Ovl25Sprite800C024C + 2, (SpriteDataHeader *)(D_800BE1B4 + ((s32 *)D_800BCD68)[1]));
    InitSpriteSourceData((SpriteSourceData *)&Ovl25Sprite800C024C + 3, (SpriteDataHeader *)(D_800BE1B4 + ((s32 *)D_800BCD68)[2]));
    for (i = 0; i < 6; i++) {
        header = (SpriteDataHeader *)(D_800BE1B4 + ((s32 *)D_800BCD68)[i + 3]);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)&Ovl25Sprite800C024C - 0x3E0 + i * 0x78), header);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)&Ovl25Sprite800C024C - 0x3B0 + i * 0x78), header);
    }
    for (i = 9; i < 11; i++) {
        header = (SpriteDataHeader *)(D_800BE1B4 + ((s32 *)D_800BCD68)[i]);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)&Ovl25Sprite800C013C + (i - 9) * 0x78), header);
        InitSpriteSourceData((SpriteSourceData *)((u8 *)&Ovl25Sprite800C013C + 0x30 + (i - 9) * 0x78), header);
    }
    i = 0;
    while ((current = (SpriteSourceData *)&Ovl25Sprite800C0334 + i, (u32)i < 6)) {
        offsets = (s32 *)D_800BCD68;
        InitSpriteSourceData(current, (SpriteDataHeader *)(D_800BE1B4 + offsets[i + 11]));
        i++;
    }
}
