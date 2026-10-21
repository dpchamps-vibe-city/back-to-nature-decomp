#include "common.h"

void ovl_11_func_800F1D54(u32 arg0, u16 *arg1) {
    s32 i;
    u16 value;
    u16 limit;

    for (i = 0; i < 6; i++) {
        switch (i) {
        case 0:
            value = (arg0 >> 25) & 0x1F;
            limit = 1;
            break;
        case 1:
            value = (arg0 >> 20) & 0x1F;
            limit = 0x7FFF;
            break;
        case 2:
            value = (arg0 >> 15) & 0x1F;
            limit = 0x1F;
            break;
        case 3:
            value = (arg0 >> 10) & 0x1F;
            limit = 0x1F;
            break;
        case 4:
            value = (arg0 >> 5) & 0x1F;
            limit = 0x1F;
            break;
        case 5:
            value = arg0 & 0x1F;
            limit = 0x1F;
            break;
        default:
            value = 0;
            limit = 0;
            break;
        }
        if (value == 0x1F) {
            value = limit;
        }
        switch (i) {
        case 0:
            arg1[i] = value - 1;
            break;
        case 1:
            arg1[i] = value - 1;
            break;
        case 2:
            arg1[i] = value - 1;
            break;
        case 3:
            arg1[i] = value;
            break;
        case 4:
            arg1[i] = value;
            break;
        case 5:
            arg1[i] = value;
            break;
        default:
            arg1[i] = value;
            break;
        }
    }
}
