#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"
s32 Ovl11ReadStateField(s32 fieldId, s32 elementIndex);

s32 ovl_11_func_800EB700(s16 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return Ovl11TestStateFieldCondition(Ovl11ReadStateField(arg0, gOvl11ValueSlots[(s16)arg1]), arg3, arg2);
}
