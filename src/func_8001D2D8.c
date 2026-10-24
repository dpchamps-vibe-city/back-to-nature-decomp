#include "common.h"

/* Return 1 if any packed GTE SXY argument has X in [-19, 319]. */
s32 func_8001D2D8(s32 sxy0, s32 sxy1, s32 sxy2, s32 sxy3) {
    union {
        unsigned long long wide;
        struct {
            u32 low;
            s32 high;
        } words;
    } t;
    s16 x;

    t.words.high = sxy0 << 16;
    x = t.words.high >> 16;
    if (x >= -19 && x < 320) {
        return 1;
    }
    x = sxy1;
    if (x >= -19 && x < 320) {
        return 1;
    }
    x = sxy2;
    if (x >= -19 && x < 320) {
        return 1;
    }
    x = sxy3;
    return x >= -19 && x < 320;
}
