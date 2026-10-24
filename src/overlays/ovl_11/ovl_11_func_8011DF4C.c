#include "common.h"

s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);
/* Overlay alias of the matched word-returning GetVal8005E524. */
s32 GetVal8005E524(void);

s32 ovl_11_func_8011DF4C(s32 arg0) {
    s32 *entry;
    u8 *base;
    s32 **grid;
    s32 **cell;

    if ((arg0 << 16) == 0) {
        D_8012D7F4 = 0;
        D_8012D800[0] = -1620;
        D_8012D800[1] = 0;
        D_8012D800[2] = 1020;
    }
    while (D_8012D7F4 < 7) {
        base = D_8009AFF0;
        D_8012D800[0] = -1420;
        for (D_8012D7F0 = 0; D_8012D7F0 < 7;) {
            grid = (s32 **)(base + 0x3608);
            ovl_11_func_800BDB88(D_8012D800, 0xC000);
            entry = (s32 *)GetVal8005E524();
            if (*entry & 8) {
                cell = grid;
                cell += D_8012D7F4 * 45 + D_8012D7F0;
                *cell = entry;
            }
            D_8012D7F0++;
            D_8012D800[0] += 400;
        }
        D_8012D7F4++;
        D_8012D800[2] -= 400;
    }
    return 1;
}
