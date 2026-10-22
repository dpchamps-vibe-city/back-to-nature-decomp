#include "common.h"
#include "psyq/memory.h"

void ovl_11_func_800DB9A4(void) {
    s32 i;
    u8 *b1;
    u8 *p1;
    u8 *b2;
    u8 *b3;
    u8 *b4;
    u8 v1;
    u8 v2;

    memset(&D_800742AC, 0, 0x40);
    for (i = 0; i < 5; i++) {
        *(s16 *)((u8 *)&D_800742AC + 4 + i * 0xC) = -1;
    }

    b1 = (u8 *) &D_8006C838;
    (*(s32 *) (b1 + 0x30)) = 0;
    v1 = 0xFF;
    i = 0x7F;
    b1 += 0xE6BF;
    do {
        *b1 = v1;
        b1 -= 1;
        i -= 1;
    } while (i >= 0);

    v2 = 0xFF;
    for (i = 7; i >= 0; i--) {
        b2 = (u8 *) &D_8006C838;
        b2[0xE6C0 + i] = v2;
    }
    for (i = 0x7F; i >= 0; i--) {
        b3 = (u8 *) &D_8006C838;
        b3[0xE6C8 + i] = 0;
    }

    b4 = (u8 *) &D_8006C838;
    p1 = b4 + 0x8000;
    (*(s8 *) (p1 + 0x6641)) = 1;
    (*(s8 *) (p1 + 0x6646)) = 0x1E;
    (*(s8 *) (p1 + 0x664C)) = 0x1F;
    (*(s8 *) (p1 + 0x6650)) = 0;
    (*(s8 *) (p1 + 0x6655)) = 0x1C;
    (*(s8 *) (p1 + 0x6656)) = 0;
    (*(s8 *) (p1 + 0x6658)) = 0x76;
    (*(s8 *) (p1 + 0x6659)) = 0x1C;
    (*(s8 *) (p1 + 0x666C)) = 0;
}
