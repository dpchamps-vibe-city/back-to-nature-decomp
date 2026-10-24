#include "common.h"

s32 ovl_11_func_800F3EF0(void) {
    u16 *base;
    u16 *pending;
    u16 *src;
    u16 *dst;
    s32 i;
    u32 cap;

    base = D_800711C4;
    pending = base + 0x7E;
    if (*pending == 0) {
        return 0;
    }
    cap = 999;
    src = base + 0x7F;
    dst = base;
    for (i = 0; i < 25; i++) {
        u16 *current = dst;
        *current += *src;
        /* Keep the cleanup in both arms: tail merging preserves allocation. */
        if (*current >= 1000) {
            *current = cap;
            *src++ = 0;
        } else {
            *src++ = 0;
        }
        dst++;
    }
    *(s32 *)&base[0x1A] += *(s32 *)&pending[0x1A];
    pending[0] = 0;
    *(s32 *)&pending[0x1A] = 0;
    return 1;
}
