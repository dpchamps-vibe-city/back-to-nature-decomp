#include "common.h"

s32 ovl_11_func_801031E4(u16 arg0) {
    s32 row;
    s32 i;
    s32 j;

    switch (arg0) {
    case 0x181:
        row = 0;
        break;
    case 0x182:
        row = 1;
        break;
    case 0x183:
        row = 2;
        break;
    case 0x184:
        row = 3;
        break;
    default:
        return 0;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 4; j++) {
            if (D_8012CF00_ids[i] == D_80127404[row][j]) {
                D_8012CF00_ids[i] = 0;
                return 1;
            }
        }
    }
    return 2;
}
