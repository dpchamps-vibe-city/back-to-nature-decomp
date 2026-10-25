#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);
/* The original caller uses an unprototyped sprite initializer. */
void func_80015704();
typedef struct {
    u8 bytes[0x29A8];
} SpriteBlob;

void ovl_23_func_800BB534(void) {
    s32 i;
    s32 offset;


    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0x18, 0, 0x1EC1C, (u8 *)(D_8005E3B0 + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0x18, 0, 0x1EC1C, (u8 *)(D_8005E3B0 + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(D_8005E3B0 + 0x6C38));
    *(SpriteBlob *)D_800BBAEC = *(SpriteBlob *)(D_8005E3B0 + 0x4290);
    func_80015704((SpriteSourceData *)D_800BFC30, (SpriteDataHeader *)(D_800BBAEC + D_800BBADC[3]));
    for (i = 0; i < 6; i++) {
        func_80015704((SpriteSourceData *)(D_800BFC30 + i * 0x50 - 0x374), (SpriteDataHeader *)(D_800BBAEC + D_800BBADC[0]));
        offset = 8;
        if (i == 2) {
            offset = 4;
        }
        func_80015704((SpriteSourceData *)(D_800BFC30 + i * 0x44 - 0x1A0), (SpriteDataHeader *)(D_800BBAEC + *(s32 *)((u8 *)D_800BBADC + offset)));
    }
}
