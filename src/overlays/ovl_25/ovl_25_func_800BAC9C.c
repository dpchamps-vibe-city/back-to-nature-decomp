#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

u8 *func_80014CBC(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4, s32 arg5);
void func_8001719C(u8 *arg0);
/* Only the two consumed arguments are supplied by this caller. */
void func_80015704();
void func_80015840(ObjectState *obj, u8 arg1);

void ovl_25_func_800BAC9C(void) {
    DrawSync(0);
    ClearOTagR((u_long *)D_8005E3C0->field_120, 0x800);
    func_80014CBC(0, 0x25000, 0x5800, (u8 *)(D_8005E3B0 + 0x4290), 1, 1);
    do {
    } while (func_80014CBC(0, 0x25000, 0x5800, (u8 *)(D_8005E3B0 + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(D_8005E3B0 + 0x566C));
    memcpy(D_800BCDD4, (void *)(D_8005E3B0 + 0x4290), 0x13DC);
    memcpy(&D_800977F8, (void *)(D_8005E3B0 + 0x4290), 0x13DC);
    func_80015704((SpriteSourceData *)D_800C021C, (SpriteDataHeader *)D_800BCDD4);
    func_80015840((ObjectState *)D_800C021C, 0x17);
}
