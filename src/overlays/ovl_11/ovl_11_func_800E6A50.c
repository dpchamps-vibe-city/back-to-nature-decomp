#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"

s32 Ovl11ReadStateField(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800E6A50(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    Ovl11ApplyStateFieldOperation(&gOvl11ValueSlots[arg2],
        Ovl11ReadStateField(arg0, arg1, arg2, arg3), arg3);
    return 1;
}
