#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"

s32 func_8001FABC(s16 arg0);
void ovl_11_func_800FB908(void);
void func_8001719C(u8 *arg0);
/* The initializer does not read its trailing parameters. */
void func_80015704();

void ovl_11_func_800FBE88(void) {
    u32 i;

    func_8001FABC(3);
    ovl_11_func_800FB908();
    D_8012720C = 1;
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0xE, 0, 0x8E90, (u8 *)(D_8005E3B0 + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0xE, 0, 0x8E90, (u8 *)(D_8005E3B0 + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(D_8005E3B0 + 0x7004));
    memcpy(D_8012A048, (u8 *)(D_8005E3B0 + 0x4290), 0x2D74);
    for (i = 0; i < 5; i++) {
        func_80015704((SpriteSourceData *)(D_8012CDF8 + i * 0x30),
                      (SpriteDataHeader *)(D_8012A048 + D_801272F4[i]));
    }
}
