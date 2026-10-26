#include "common.h"
#include "game_types.h"

void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800CD670(s16 arg0) {
    u8 *base = (u8 *)&gPrimaryObjectBase;

    if (*(s32 *)(base + 0x6C) & 0x40000000) {
        return;
    }
    if (*(s32 *)(base + 0x6C) & 8) {
        return;
    }
    *(s16 *)(base + 0x2E) = arg0;
    *(s32 *)(base + 0x6C) |= 0xA;
    *(u16 *)(base + 0x30) = *(u16 *)(base + 0x38);
    ovl_11_func_800C70CC((Ovl11C7010Object *)base, 2,
        *(u16 *)(base + 0x38) + 1, -2);
    *(s32 *)(base + 0x6C) &= ~0x20000000;
}
