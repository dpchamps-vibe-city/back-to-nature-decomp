#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
s32 Ovl11ReadStateField(s32 fieldId, s32 elementIndex);

s32 func_8001AF44(u32 arg0);
s32 Ovl11TestStateFieldCondition(s32 arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800E6744(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 temp_s1;

    temp_s1 = Ovl11ReadStateField((s32) arg0, (s32) arg1);
    if (arg0 == 0x14 && func_8001AF44(0x1BU) == 1 && arg3 != -1) {
        return 0;
    }
    return Ovl11TestStateFieldCondition(temp_s1, arg3, arg2);
}
