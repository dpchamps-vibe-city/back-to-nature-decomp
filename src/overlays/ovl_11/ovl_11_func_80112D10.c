#include "common.h"

void ovl_11_func_80112D10(void) {
    u8 *b;
    u8 *tail;
    u8 *p;
    s32 idx;
    u32 i;
    u32 j;
    u32 k;

    i = 0;
    while (i < 10) {
        b = D_8012D110;
        p = b + 0xC;
        p[i] = 0;
        for (j = 0; j < 10; j++) {
            idx = 0x16 + i * 100;
            for (k = 0; k < 10; k++) {
                b[idx + j * 10 + k] = 0;
            }
        }
        i++;
    }
    tail = D_8012D110;
    *(u32 *)(tail + 0x400) = 0;
    *(u16 *)D_8012D110 = 0;
}
