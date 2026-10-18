#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800E109C(UnkStruct800E109C *arg0);
s32 ovl_11_func_800D9108(s32 *arg0);

s32 ovl_11_func_800E05A8(UnkStruct800E109C *arg0) {
    s16 temp_s1;
    s32 temp_a1;
    s32 var_v0;
    s32 temp_v0_2;
    s32 var_v1;

    if (ovl_11_func_800E109C(arg0) == 0) {
        return -1;
    }
    temp_a1 = *(s32 *)((u8 *)arg0 + 0x34);
    if (temp_a1 & 0x400) {
        return -1;
    }
    temp_s1 = *(s16 *)((u8 *)arg0 + 0x30);
    if ((temp_s1 == 1) &&
        (((struct Ovl11HandlerState *)D_8006C838)->field_44BA != 3)) {
        if (((struct Ovl11HandlerState *)D_8006C838)->field_E4C8 != temp_s1) {
            if ((++(*(u16 *)((u8 *)arg0 + 0xAC)) >= 5U) &&
                !(temp_a1 & 1) &&
                (ovl_11_func_800D9108((s32 *)((u8 *)arg0 + 0x38)) == temp_s1)) {
                *(s32 *)((u8 *)arg0 + 0x34) |= 1;
            }
        } else {
            temp_v0_2 = *(u16 *)((u8 *)arg0 + 0x16) - 5;
            *(u16 *)((u8 *)arg0 + 0x16) = temp_v0_2;
            if ((s16)temp_v0_2 < 0) {
                var_v1 = 0;
            } else {
                var_v1 = temp_v0_2;
            }
            *(u16 *)((u8 *)arg0 + 0x16) = var_v1;
        }
    }
    var_v0 = 0;
    if ((u32)(((struct Ovl11HandlerState *)D_8006C838)->field_44C0 - 6) >= 0xFU) {
        *(s16 *)((u8 *)arg0 + 0x2C) = 0x12C;
        *(s32 *)((u8 *)arg0 + 0x34) &= ~0x800;
        var_v0 = 0;
    }
    return var_v0;
}
