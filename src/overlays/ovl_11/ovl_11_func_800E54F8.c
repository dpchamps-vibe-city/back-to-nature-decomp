#include "common.h"

u8 *ovl_11_func_800E36CC(u16 *arg0, s16 arg1, s16 arg2, u32 arg3);

u8 *ovl_11_func_800E54F8(u16 *arg0, s32 arg1) {
    arg1 -= 0x15F;
    return ovl_11_func_800E36CC(arg0, D_80129418, 0, arg1);
}
