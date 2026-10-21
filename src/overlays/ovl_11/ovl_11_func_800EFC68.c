#include "common.h"

u8 *ovl_11_func_800EFF04(s32 arg0, s32 arg1, s32 *arg2);

s32 ovl_11_func_800EFC68(s32 arg0, s32 arg1) {
    s32 sp10;
    s32 temp_v0;
    s32 mask;
    s32 var_a0;
    u8 *temp_a1;

    if ((u32) (arg0 - 0x31) < 0x2EFU) {
        var_a0 = func_8001AF44((arg0 - 0x30) & 0xFFFF);
    } else {
        temp_a1 = ovl_11_func_800EFF04(arg0, arg1, &sp10);
        switch (sp10) {
        case 0:
        case 1:
            var_a0 = (s32) *temp_a1;
            break;
        case 2:
            var_a0 = *(u16 *) temp_a1;
            break;
        case 3:
            var_a0 = *(s16 *) temp_a1;
            break;
        case 4:
        case 5:
        default:
            var_a0 = *(s32 *) temp_a1;
            break;
        }
        mask = _D_80124A18[arg0].field_10;
        if (mask != 0) {
            temp_v0 = var_a0 & mask;
            var_a0 = temp_v0 != 0;
        }
    }
    if ((arg0 == 0xC) || (arg0 == 0xE)) {
        var_a0 += 1;
    }
    return var_a0;
}
