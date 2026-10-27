#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"
#include "ovl11_state_object_pool_clear.h"

s32 ovl_11_func_800E5C60(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_a1;
    u8 *temp_v0;

    if (arg0 != 0x2C) {
        if (arg2 != 0) {
            var_a1 = gOvl11ValueSlots[arg0];
        } else {
            var_a1 = arg0;
        }
        temp_v0 = Ovl11ResolveStateField(0x25, var_a1, NULL);
        if (arg1 == 0) {
            *(u16 *)temp_v0 |= 0x800;
        } else {
            *(u16 *)temp_v0 &= 0xF7FF;
        }
    } else {
        Ovl11ClearEligibleStateObjects(0);
    }
    return 1;
}
