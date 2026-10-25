#include "common.h"

/* The callee sign-extends both incoming halfwords and returns an s32. */
s32 ovl_11_func_800D60F0(s16 arg0, s16 arg1);

s32 ovl_11_func_800E93CC(s16 arg0, s16 arg1) {
    ItemData *item;
    u32 flags;

    if (arg1 != 0) {
        arg1 = *(s16 *)&gOvl11ValueSlots[arg0];
    } else {
        arg1 = arg0;
    }
    if (arg1 == 0xA4) {
        return 1;
    }
    item = D_8006C858;
    item += arg1;
    flags = item->u0.flags;
    flags &= 0x1C00;
    if (flags) {
        flags = ovl_11_func_800D60F0(arg1, 3);
        if ((u16)flags < 3) {
            return 1;
        }
    }
    return 0;
}
