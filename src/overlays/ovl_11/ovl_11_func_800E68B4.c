#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
s32 Ovl11ReadStateField(s32 fieldId, s32 elementIndex);

s32 ovl_11_func_800E68B4(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_s1;

    var_s1 = 0;
    temp_v0 = Ovl11ReadStateField((s32) arg0, (s32) arg1);
    if (temp_v0 >= arg2) {
        var_s1 = arg3 >= temp_v0;
    }
    return var_s1;
}
