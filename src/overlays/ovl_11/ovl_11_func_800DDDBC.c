#include "common.h"

s32 ovl_11_func_800DDDBC(void) {
    s32 *base;
    s32 *tail;
    s32 *a;
    s32 c;

    c = 2;
    do {
        base = (s32 *)&D_8006C838;
        a = base + (c + 1) * 2;
        a[0x1271] = a[0x126F];
        a[0x1272] = a[0x1270];
        c--;
    } while (c >= 0);
    tail = (s32 *)&D_8006C838;
    tail[0x1271] = 0;
    tail[0x1272] = 0;
    return 1;
}
