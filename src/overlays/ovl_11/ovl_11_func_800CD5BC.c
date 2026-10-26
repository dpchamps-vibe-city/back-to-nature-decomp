#include "common.h"
#include "game_types.h"

void ovl_11_func_800CD5BC(Ovl11CD5BCObject *arg0, s32 arg1) {
    s32 idx;
    s32 sum;

    if (!(arg0->flags & 8)) {
        idx = (arg0->value - 1) / 5;
        sum = arg1 + arg0->vals[idx];
        if (sum > 0xFFFF) {
            sum = 0xFFFF;
        }
        arg0->vals[idx] = (u16) sum;
    }
}
