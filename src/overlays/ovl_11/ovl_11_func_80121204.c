#include "common.h"

s32 ovl_11_func_80121204(void) {
    struct Ovl11ConditionState *state;
    s16 *threshold;
    s16 prev;
    s16 cur;
    s16 rem;
    s32 ret;
    u32 i;

    state = (struct Ovl11ConditionState *)D_8006C838;
    rem = (state->date[1] * 30 + state->date[2] + 20) % 120;
    i = 0;
    prev = -1;
    threshold = &D_801287CC[0];
    ret = 0x1BD0000;
    cur = *threshold;
    while (1) {
        if (prev < rem && cur >= rem) {
            return ret >> 16;
        }
        threshold++;
        i++;
        ret += 0x10000;
        if (i >= 13) {
            return 0x1BD;
        }
        prev = cur;
        cur = *threshold;
    }
}
