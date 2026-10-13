#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

u8 *func_80014CBC(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4, s32 arg5);
void func_8001719C(u8 *arg0);
/* The caller uses the unprototyped declaration also seen in ovl_17_func_800BA2C8. */
void func_80015704();

void ovl_17_func_800B9FA8(void) {
    DrawSync(0);
    ClearOTagR((u_long *)D_8005E3C0->field_120, 0x800);
    func_80014CBC(0, 0x25000, 0x5800, (u8 *)(D_8005E3B0 + 0x4290), 1, 1);
    do {
    } while (func_80014CBC(0, 0x25000, 0x5800, (u8 *)(D_8005E3B0 + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(D_8005E3B0 + 0x566C));
    memcpy(D_800BB728, (void *)(D_8005E3B0 + 0x4290), 0x13DC);
    memcpy(&D_800977F8, (void *)(D_8005E3B0 + 0x4290), 0x13DC);
    func_80015704((SpriteSourceData *)D_800BDAA4, (SpriteDataHeader *)D_800BB728);
}
