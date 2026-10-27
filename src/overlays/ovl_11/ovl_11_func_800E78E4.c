#include "common.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"

void ovl_11_func_800E78E4(s32 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 temp;

    temp = Rand(arg0 & 0xFFFF);
    if (arg1 != -1) {
        gOvl11ValueSlots[arg1] = temp;
    }
    Ovl11TestStateFieldCondition(temp, arg3, arg2);
}
