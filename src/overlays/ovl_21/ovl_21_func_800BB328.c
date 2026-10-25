#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);
/* As in the adjacent loader, the initializer is declared without a prototype. */
void func_80015704();

void ovl_21_func_800BB328(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(D_8005E3B0 + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(D_8005E3B0 + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(D_8005E3B0 + 0x566C));
    memcpy(D_800BCD28, (void *)(D_8005E3B0 + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(D_8005E3B0 + 0x4290), 0x13DC);
    func_80015704(&D_800C0ACC, (SpriteDataHeader *)D_800BCD28);
}
