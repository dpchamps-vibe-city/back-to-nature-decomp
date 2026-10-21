#include "common.h"

s32 ovl_11_func_800E20F0(u16 *arg0) {
    s16 temp_a1;
    s32 temp_v0;
    s32 temp_v0_2;
    u16 temp_v1;
    s32 var_v1;

    temp_v1 = *arg0;
    if (temp_v1 == 0) {
        return -1;
    }
    if (temp_v1 != 0x109) {
        return 0;
    }
    temp_a1 = *(s16 *) ((u8 *) arg0 + 0x30);
    if ((temp_a1 == 1) && (((Ovl11E20F0StateView *)D_8006C838)->field_44BA != 3)) {
        if (((Ovl11E20F0StateView *)D_8006C838)->field_E4C8 != temp_a1) {
            temp_v0 = (*(u16 *) ((u8 *) arg0 + 0xAC)) + 1;
            (*(u16 *) ((u8 *) arg0 + 0xAC)) = temp_v0;
            if ((u32) (temp_v0 & 0xFFFF) >= 5U) {
                (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) | 1);
            }
        } else {
            s32 checkedEnergy;
            temp_v0_2 = (*(u16 *) ((u8 *) arg0 + 0x16)) - 5;
            (*(u16 *) ((u8 *) arg0 + 0x16)) = temp_v0_2;
            checkedEnergy = (s16)temp_v0_2;
            var_v1 = temp_v0_2;
            if (checkedEnergy < 0) {
                var_v1 = 0;
            }
            (*(u16 *) ((u8 *) arg0 + 0x16)) = var_v1;
        }
    }
    if ((u32) (((Ovl11E20F0StateView *)D_8006C838)->field_44C0 - 6) >= 0xFU) {
        (*(s16 *) ((u8 *) arg0 + 0x2C)) = 0x12C;
        (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) & ~0x800);
    }
    return 0;
}
