#include "common.h"
#include "game_types.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800C74AC(u8 *arg0, u16 *arg1) {
    if ((u32) (*arg1 - 0x22) < 0x15U) {
        *(s32 *)(arg0 + 0x6C) |= 0x1800;
        ovl_11_func_800C70CC((Ovl11C7010Object *)arg0, 0x53,
            D_8006C858[*(s16 *)(arg0 + 0x84)].action.field_6, -2);
        return 1;
    }
    return 0;
}
