#include "common.h"
#include "ovl11_balance_accounting.h"

s32 ovl_11_func_800E7CCC(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 selected;
    s16 temp;

    if (arg3 != 0) {
        selected = gOvl11ValueSlots[arg2];
    } else {
        selected = arg2;
    }
    temp = Ovl11AdjustSpendableBalance(selected, arg0);
    if (arg1 != -1) {
        gOvl11ValueSlots[arg1] = temp;
    }
    return temp == 0;
}
