#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);
/* This caller uses the same unprototyped declaration as the adjacent loader. */
void func_80015704();

void ovl_19_func_800BB5D4(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(D_8005E3B0 + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(D_8005E3B0 + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(D_8005E3B0 + 0x566C));
    memcpy(D_800BD090, (void *)(D_8005E3B0 + 0x4290), 0x13DC);
    memcpy(&D_800977F8, (void *)(D_8005E3B0 + 0x4290), 0x13DC);
    func_80015704(&D_800BF690, (SpriteDataHeader *)D_800BD090);
}
