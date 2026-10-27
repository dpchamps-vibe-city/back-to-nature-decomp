#include "common.h"
#include "ovl11_object_record_reset_command.h"

s32 ovl_11_func_800E946C(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 var_a0;
    s16 var_a1;
    s32 var_a2;
    s32 var_a3;

    if (arg0 != -1) {
        var_a0 = *((s16 *)&gOvl11ValueSlots[arg0]);
    } else {
        var_a0 = -1;
    }
    if (arg1 != -1) {
        var_a1 = *((s16 *)&gOvl11ValueSlots[arg1]);
    } else {
        var_a1 = -1;
    }
    if (arg2 != -1) {
        var_a2 = gOvl11ValueSlots[arg2];
    } else {
        var_a2 = -1;
    }
    if (arg3 != -1) {
        var_a3 = gOvl11ValueSlots[arg3];
    } else {
        var_a3 = -1;
    }
    Ovl11ResetObjectRecordCommand(var_a0, var_a1, var_a2, var_a3);
    return 1;
}
