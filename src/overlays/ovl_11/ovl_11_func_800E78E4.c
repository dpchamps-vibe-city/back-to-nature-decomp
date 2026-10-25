#include "common.h"
#include "random.h"

s32 Ovl11TestStateFieldCondition(s32 arg0, s32 arg1, s32 arg2);

void ovl_11_func_800E78E4(s32 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 temp;

    temp = Rand(arg0 & 0xFFFF);
    if (arg1 != -1) {
        gOvl11ValueSlots[arg1] = temp;
    }
    Ovl11TestStateFieldCondition(temp, arg3, arg2);
}
