#include "common.h"

s32 ovl_11_func_800D6A94(s16 arg0);

void ovl_11_func_800C9BB8(void) {
    u32 i;
    s16 (*base)[3];
    s16 (*table)[3];

    base = (s16 (*)[3])&D_80071A00;
    table = base + 22;
    for (i = 0; i < 2; i++) {
        ovl_11_func_800D6A94(table[i][0]);
    }
    table = base + 32;
    for (i = 0; i < 8; i++) {
        ovl_11_func_800D6A94(table[i][0]);
    }
    table = base + 24;
    for (i = 0; i < 8; i++) {
        ovl_11_func_800D6A94(table[i][0]);
    }
}
