#include "common.h"

u8 *ovl_11_func_800EFF04(s32 arg0, s32 arg1, s32 *arg2);
s32 ovl_11_func_800EFE34(s32 *ptr, s32 arg1, s32 arg2);
s32 ovl_11_func_800EFD54(s32 arg0, s32 arg1, s32 arg2);

void ovl_11_func_800EFABC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp10;
    s32 sp14;
    s32 temp_v1;
    u8 *temp_s0;

    if ((u32) (arg0 - 0x31) < 0x2EFU) {
        func_8001AF70((arg0 - 0x30) & 0xFFFF, arg2 & 0xFFFF);
        return;
    }
    temp_s0 = ovl_11_func_800EFF04(arg0, arg1, &sp10);
    switch (sp10) {
    case 0:
        sp14 = *(u8 *) temp_s0;
        break;
    case 1:
        sp14 = *(u8 *) temp_s0;
        break;
    case 2:
        sp14 = *(u16 *) temp_s0;
        break;
    case 3:
        sp14 = *(s16 *) temp_s0;
        break;
    case 4:
    case 5:
    default:
        sp14 = *(s32 *) temp_s0;
        break;
    }
    temp_v1 = _D_80124A18[arg0].field_10;
    if (temp_v1 != 0) {
        if (arg2 != 0) {
            sp14 = sp14 | temp_v1;
        } else {
            sp14 = sp14 & ~temp_v1;
        }
    } else {
        ovl_11_func_800EFE34(&sp14, arg2, arg3);
        sp14 = ovl_11_func_800EFD54(arg0, arg1, sp14);
    }
    switch (sp10) {
    case 0:
    case 1:
        *(u8 *) temp_s0 = sp14;
        return;
    case 2:
    case 3:
        *(u16 *) temp_s0 = sp14;
        return;
    case 4:
    case 5:
    default:
        *(s32 *) temp_s0 = sp14;
        return;
    }
}
