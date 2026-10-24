#include "common.h"

s32 func_80017F30(u16 *pa, u16 *pb, u16 *pc) {
    s32 raw;
    s32 x;
    s32 y, z;
    u32 mx;
    s32 r;
    u16 sentinel = 0xFFFF;

    goto body;
    for (;;) {
    top:
        if (x == sentinel) goto out;
        pa++;
        pb++;
        pc++;
    body:
        raw = *pa;
        mx = raw & 0xFFFF;
        x = raw;
        y = *pb;
        if (mx == y) {
            r = 0;
            goto top;
        }
        z = *pc;
        if (mx == z) {
            r = raw = 0;
            goto top;
        }
        r = (mx < y) ? -1 : 1;
        break;
    }
out:
    return r;
}
