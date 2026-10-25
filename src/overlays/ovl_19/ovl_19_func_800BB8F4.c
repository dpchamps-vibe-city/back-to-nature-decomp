#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);
/* The caller supplies only the sprite object and its data header. */
void func_80015704();

void ovl_19_func_800BB8F4(void) {
    s32 i;

    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x14, 0, 0x1AE04, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x14, 0, 0x1AE04, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(gGraphicsHeapBase + 0x52DC));
    *(Ovl19SpriteBlob *)D_800BE470 = *(Ovl19SpriteBlob *)(gGraphicsHeapBase + 0x4290);
    func_80015704((SpriteSourceData *)D_800BF660, (SpriteDataHeader *)(D_800BE470 + D_800BD058[0]));
    for (i = 0; i < 3; i++) {
        func_80015704((SpriteSourceData *)(D_800BF660 + i * 0x40 - 0xF0), (SpriteDataHeader *)(D_800BE470 + D_800BD058[i + 2]));
    }
    for (i = 0; i < 2; i++) {
        func_80015704((SpriteSourceData *)(D_800BF4E8 + i * 0x48), (SpriteDataHeader *)(D_800BE470 + D_800BD058[1]));
    }
    func_80015704(&D_800BF6D0, (SpriteDataHeader *)(D_800BE470 + D_800BD058[5]));
}
