#include "common.h"

s32 ovl_11_func_80113208(s16 arg0, s16 arg1, s16 arg2);

void ovl_11_func_80113124(void) {
    u32 x;
    u32 y;
    u8 *cell;

    cell = D_8012D110 + 0x16 + *(s16 *)D_8012D110 * 100;
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++, cell++) {
            if (*cell == 1) {
                ovl_11_func_80113208(x, y, 1);
            }
            if (*cell == 2) {
                ovl_11_func_80113208(x, y, 2);
            }
        }
    }
}
