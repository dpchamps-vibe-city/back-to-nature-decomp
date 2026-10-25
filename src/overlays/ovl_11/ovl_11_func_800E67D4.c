#include "common.h"

s32 Ovl11ReadStateField(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 Ovl11TestStateFieldCondition(s32 arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800E67D4(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    return Ovl11TestStateFieldCondition(
        Ovl11ReadStateField(arg0, arg1, arg2, arg3),
        gOvl11ValueSlots[arg3], arg2);
}
