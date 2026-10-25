#include "common.h"
#include "random.h"

void ovl_25_func_800B9B00(s16 *arg0, s16 arg1, s16 arg2);

void ovl_25_func_800B95B0(s16 *arg0) {
    s16 rnd;
    s16 sum;
    s16 sel;
    s32 result;
    s32 i;
    s32 val;
    s32 timer;
    s32 weight;
    s16 *ref;

    sum = 0;
    sel = 0;
    rnd = Rand(0x64);
    val = *(s32 *)&arg0[6];
    if (val > 0x27FFF) {
        sel = -1;
    } else if (val > 0x1DFFF) {
        sel = 0;
    } else if (val > 0x13FFF) {
        sel = 1;
    } else if (val > 0x9FFF) {
        sel = 3;
    }
    i = 0;
    for (; i < 4; i++) {
        weight = D_800BCCC0[sel][arg0[0] - 1][i];
        sum = (s16)(sum + weight);
        if (rnd < sum) {
            break;
        }
    }
    if (sel == -1) {
        result = 0x32;
    } else {
        result = (s16)(0x31 - i * 10);
        if (i == 0) {
            result = result + 1;
        }
        result = (s16)result;
    }
    val = *(s32 *)&arg0[6];
    timer = result - val / 4096;
    ref = (s16 *)&D_800BFE44_view;
    ovl_25_func_800B9B00((s16 *)arg0, 5, (s16)(timer * ref[0x26C] / ref[0x26B]));
}

