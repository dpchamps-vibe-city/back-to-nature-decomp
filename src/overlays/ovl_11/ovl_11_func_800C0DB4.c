#include "common.h"

void ovl_11_func_800C0EA4(s32 arg0);

void ovl_11_func_800C0DB4(s16 *arg0, u32 arg1) {
    s16 temp_a0;
    s16 temp_a0_2;
    s16 temp_a0_3;
    s16 temp_a0_4;
    s16 temp_a0_5;

    switch (arg1) {
    case 0:
        temp_a0 = arg0[5];
        if ((temp_a0 == -1) || (temp_a0 == D_80070CFA)) {
        case 1:
            temp_a0_2 = arg0[4];
            if ((temp_a0_2 == -1) || (temp_a0_2 == D_80070CF8_signedHalfword)) {
            case 2:
                temp_a0_3 = arg0[2];
                if ((temp_a0_3 == -1) || (temp_a0_3 == D_80070CF4)) {
                case 3:
                    temp_a0_4 = arg0[1];
                    if ((temp_a0_4 == -1) || (temp_a0_4 == D_80070CF2)) {
                    case 4:
                        temp_a0_5 = arg0[0];
                        if ((temp_a0_5 == -1) || (temp_a0_5 == D_80070CF0)) {
                        default:
                            ovl_11_func_800C0EA4((s32) arg0[6]);
                        }
                    }
                }
            }
        }
        break;
    }
}
