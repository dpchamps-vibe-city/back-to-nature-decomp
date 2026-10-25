#include "common.h"
#include "random.h"

extern s16 gCalendarYear;
extern s16 D_800BF4C8;
extern u16 D_800BCFE4[];

s32 ovl_19_func_800B85F4(void) {
    s16 s0;
    s16 s1;
    s32 v;

    s0 = gCalendarYear;
    s1 = D_800BF4C8;
    if (s0 >= 2) {
        s0 = 1;
    }
    if (s1 >= 3) {
        s0 = 2;
    }
    v = (s16)Rand(D_800BCFE4[s0 * 3 + s1]);
    if (s0 != 0 && s1 == 2) {
        v = (s16)(v + 0xAB);
    }
    return v;
}
