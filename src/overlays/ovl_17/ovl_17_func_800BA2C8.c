#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);
/* Only out and header are read by the initializer; the caller is unprototyped. */
void func_80015704();
typedef struct {
    u8 bytes[0xC28];
} SpriteBlob;

void ovl_17_func_800BA2C8(void) {
    s32 i;

    DrawSync(0);
    ClearOTagR((u_long *) gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x12, 0, 0x1E0A8, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x12, 0, 0x1E0A8, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(gGraphicsHeapBase + 0x4EB8));
    *(SpriteBlob *) D_800BCB08 = *(SpriteBlob *)(gGraphicsHeapBase + 0x4290);
    D_800BD74C = D_800BCB08 + D_800BB6FC[1];
    D_800BD750 = D_800BCB08 + D_800BB6FC[2];
    func_80015704(&D_800BDDF8, (SpriteDataHeader *)(D_800BCB08 + D_800BB6FC[4]));
    func_80015704((SpriteSourceData *) D_800BDA74, (SpriteDataHeader *)(D_800BCB08 + D_800BB6FC[0]));
    func_80015704((SpriteSourceData *)(D_800BDA74 + 0x60), (SpriteDataHeader *)(D_800BCB08 + D_800BB6FC[3]));
    for (i = 0; i < 6; i++) {
        func_80015704((SpriteSourceData *)(D_800BDA74 + i * 0x50 - 0x1E4), (SpriteDataHeader *)(D_800BCB08 + D_800BB6FC[2]));
    }
}
