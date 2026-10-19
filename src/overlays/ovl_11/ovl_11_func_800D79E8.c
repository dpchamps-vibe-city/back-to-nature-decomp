#include "common.h"

s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);
/* Overlay alias of the matched word-returning GetVal8005E524. */
s32 func_8001EF98(void);

s32 ovl_11_func_800D79E8(s32 arg0) {
    s32 *entry;
    u8 *base;
    s32 **grid;
    s32 **cell;

    if ((arg0 << 16) == 0) {
        D_80128D8C = 0;
        D_80128D98[1] = 0;
        D_80128D98[2] = 1500;
    }
    while (D_80128D8C < 25) {
        base = D_8009AFF0;
        D_80128D98[0] = -4200;
        for (D_80128D88 = 0; D_80128D88 < 45;) {
            grid = (s32 **)(base + 0x3608);
            ovl_11_func_800BDB88(D_80128D98, 0xC000);
            entry = (s32 *)func_8001EF98();
            if (*entry & 8) {
                cell = grid;
                cell += D_80128D8C * 45 + D_80128D88;
                *cell = entry;
            }
            D_80128D88++;
            D_80128D98[0] += 400;
        }
        D_80128D8C++;
        D_80128D98[2] -= 400;
    }
    return 1;
}
