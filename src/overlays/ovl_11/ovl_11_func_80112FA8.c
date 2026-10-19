#include "common.h"

s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);
/* Overlay alias of the matched word-returning GetVal8005E524. */
s32 func_8001EF98(void);

s32 ovl_11_func_80112FA8(s32 arg0) {
    s32 *entry;
    u8 *base;
    s32 **grid;
    s32 **cell;

    ((Ovl11GridState *)D_8012D110)->probability = 10;
    D_8012D110[0x15] = 1;
    if ((arg0 << 16) == 0) {
        D_8012D0F4 = 0;
        D_8012D100[0] = -2000;
        D_8012D100[1] = 0;
        D_8012D100[2] = 1800;
    }
    while (D_8012D0F4 < 10) {
        base = D_8009AFF0;
        D_8012D100[0] = -1800;
        for (D_8012D0F0 = 0; D_8012D0F0 < 10;) {
            grid = (s32 **)(base + 0x3608);
            ovl_11_func_800BDB88(D_8012D100, 0xC000);
            entry = (s32 *)func_8001EF98();
            if (*entry & 8) {
                cell = grid;
                cell += D_8012D0F4 * 45 + D_8012D0F0;
                *cell = entry;
            }
            D_8012D0F0++;
            D_8012D100[0] += 400;
        }
        D_8012D0F4++;
        D_8012D100[2] -= 400;
    }
    ((Ovl11GridState *)D_8012D110)->flags &= ~0x2000;
    return 1;
}
