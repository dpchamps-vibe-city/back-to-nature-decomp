#include "common.h"

void ovl_11_func_800D6488(u8 arg0, u8 dst[][13], u8 src[][13]) {
    s32 i;
    s32 j;

    switch (arg0) {
    case 0:
        for (i = 0; i < 13; i++) {
            for (j = 12; j >= 0; j--) {
                dst[i][j] = src[j][i];
            }
        }
        break;
    case 1:
        for (i = 0; i < 13; i++) {
            for (j = 12; j >= 0; j--) {
                dst[i][12 - j] = src[i][j];
            }
        }
        break;
    case 2:
        for (i = 12; i >= 0; i--) {
            for (j = 0; j < 13; j++) {
                dst[12 - i][j] = src[j][i];
            }
        }
        break;
    case 3:
        for (i = 0; i < 13; i++) {
            for (j = 0; j < 13; j++) {
                dst[i][j] = src[i][j];
            }
        }
        break;
    }
}
