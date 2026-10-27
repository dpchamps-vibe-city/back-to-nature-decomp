#include "common.h"
#include "ovl11_condition_evaluation.h"

s32 ovl_11_func_800E7798(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800E87A0(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_v0;

    var_v0 = arg0;
    if (arg2 != 0) {
        var_v0 = gOvl11ValueSlots[var_v0];
    }
    Ovl11QueueConditionCodeAnySlot(var_v0 & 0xFFFF, arg3 == 0);
    ovl_11_func_800E7798(0, 0, 0, arg1);
    return 1;
}
