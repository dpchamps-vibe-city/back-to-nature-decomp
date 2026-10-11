#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/inline_c.h"

void func_8001D6B8(void) {
    MATRIX m;
    s32 i;

    gte_ReadRotMatrix(&m);
    for (i = 0; i < 3; i++) {
        if (D_800491C8.unk0 & ~1) {
            if (D_800491C8.unk4 >= 2) {
                m.m[i][0] = m.m[i][0] * D_800491C8.unk0 / D_800491C8.unk4;
            }
        }
        if (D_800491C8.unk8 & ~1) {
            if (D_800491C8.unkC >= 2) {
                m.m[i][1] = m.m[i][1] * D_800491C8.unk8 / D_800491C8.unkC;
            }
        }
        if (D_800491C8.unk10 & ~1) {
            if (D_800491C8.unk14 >= 2) {
                m.m[i][2] = m.m[i][2] * D_800491C8.unk10 / D_800491C8.unk14;
            }
        }
    }
    gte_SetRotMatrix(&m);
}
