#include "common.h"

s32 func_80012A34(s32 arg0);
s32 ovl_25_func_800B977C(s16 *arg0, s16 arg1);
s32 ovl_25_func_800B9B00(s16 *arg0, s16 arg1, s16 arg2);
s32 ovl_25_func_800B95B0(s16 *arg0);

s32 ovl_25_func_800B9480(s16 *arg0) {
    s16 random;
    s16 *state;
    s32 value;

    if (ovl_25_func_800B977C(arg0, 4) == 1) {
        state = (s16 *)&D_800BFE44_view;
        random = func_80012A34((u16)(120 / state[0x26C])) + 1;
        value = state[0x267] * 30;
        if (random >= value / (state[0x26C] * 100)) {
            ovl_25_func_800B9B00(arg0, 3, 60);
            value = random - 2;
            arg0[3] = value * 30 / state[0x26A];
            return value;
        } else {
            return ovl_25_func_800B9B00(arg0, 2, random * (u16)state[0x26A]);
        }
    } else {
        return ovl_25_func_800B95B0(arg0);
    }
}
