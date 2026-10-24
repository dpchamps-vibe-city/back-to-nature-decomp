#include "common.h"

/* Clear three rows of six 14-byte records: first halfword is -1 and the
 * remaining six are zero. The two bases reach the same record array, with
 * pX addressing its last halfword. */
void ovl_11_func_8010476C(void) {
    volatile s16 *pW;
    volatile s16 *pX;
    s16 *pBase;
    s16 *base2;
    u32 rows;
    u32 n;
    u32 next;
    u32 rb;

    rows = 0;
    pBase = (s16 *)D_80074838;
    base2 = (s16 *)&D_8006C838;
    base2 += 0x7291;
    do {
        n = 0;
        next = rows + 1;
        pX = (s16 *)(rows * 0x54 + (u32)base2);
        rb = rows * 0x54 + 0x6520;
        pW = (s16 *)(rb + (u32)pBase);
        do {
            pW[-5] = -1;
            pW[-4] = 0;
            pW[-3] = 0;
            pW[-2] = 0;
            pW[-1] = 0;
            pW[0] = 0;
            *pX = 0;
            pX += 7;
            pW += 7;
            n++;
        } while (n < 6U);
        rows = next;
    } while (rows < 3U);
}
