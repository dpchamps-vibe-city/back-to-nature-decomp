#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_state_change.h"

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
    Ovl11ChangePrimaryObjectState((Ovl11C7010Object *)base, 2,
        *(u16 *)(base + 0x38) + 1, -2);
    *(s32 *)(base + 0x6C) &= ~0x20000000;
}
