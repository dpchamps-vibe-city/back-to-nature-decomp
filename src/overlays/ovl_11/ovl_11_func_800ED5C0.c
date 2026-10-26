#include "common.h"
#include "ovl11_record_family_list.h"

s32 ovl_11_func_800ED5C0(s16 arg0, s16 arg1, s32 arg2) {
    s16 var_v0;
    s32 temp_v0;

    var_v0 = 0;
    if (arg1 == 0) {
        var_v0 = -1;
    }
    temp_v0 = Ovl11DrawAndProcessRecordFamilyList(arg0, 1, var_v0);
    gOvl11ValueSlots[arg2] = temp_v0;
    if (temp_v0 == -2) {
        return 1;
    }
    return temp_v0 != -1;
}
