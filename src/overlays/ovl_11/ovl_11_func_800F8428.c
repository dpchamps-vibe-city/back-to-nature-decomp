#include "common.h"
#include "ovl11_item_stack_clear.h"

void ovl_11_func_800F8428(u16 *arg0, u16 *arg1) {
    s32 sum;

    sum = arg1[2] + arg0[2];
    arg1[2] = sum;
    if ((s16)sum >= 100) {
        arg1[2] = 99;
        arg0[2] = sum - 99;
    } else {
        Ovl11ClearItemStackEntry((s16 *)arg0);
    }
}
