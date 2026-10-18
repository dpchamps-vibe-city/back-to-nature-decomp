#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C72F8(Ovl11C7010Object *arg0, u16 *arg1);
s32 ovl_11_func_800C73FC(Ovl11C7010Object *arg0, u16 *arg1);
s32 ovl_11_func_800C74AC(u8 *arg0, u16 *arg1);
s32 ovl_11_func_800C7524(Ovl11C7010Object *arg0, u16 *arg1);

s32 ovl_11_func_800C76EC(Ovl11C7010Object *arg0) {
    s16 *temp_s0;
    s32 temp_a0;
    u16 pressed;
    u16 held;

    temp_a0 = arg0->unk6C;
    if (temp_a0 & 0x18000000) {
        return 0;
    }
    if (*(s16 *)((u8 *)arg0 + 0x16) >= 100) {
        return 1;
    }
    pressed = *(u16 *)&D_801231F8;
    held = *(u16 *)&D_801231F4;
    if (temp_a0 & 0x40) {
        return 0;
    }
    if (*(s16 *)((u8 *)arg0 + 0x18) != 0) {
        if (held & 0x80) {
            (*(s16 *)((u8 *)arg0 + 0x18))++;
            return 1;
        }
    } else if (pressed & 0x80) {
        temp_s0 = &arg0->unk84;
        if (ovl_11_func_800C72F8(arg0, (u16 *)temp_s0) ||
            ovl_11_func_800C73FC(arg0, (u16 *)temp_s0) ||
            ovl_11_func_800C74AC((u8 *)arg0, (u16 *)temp_s0)) {
            arg0->sprite.field_5 = 0;
            return 1;
        }
        if (ovl_11_func_800C7524(arg0, (u16 *)temp_s0)) {
            return 1;
        }
    }
    *(s16 *)((u8 *)arg0 + 0x18) = 0;
    return 0;
}
