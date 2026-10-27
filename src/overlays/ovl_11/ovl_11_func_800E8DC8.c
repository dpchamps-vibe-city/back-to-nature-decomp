#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"
#include "ovl11_state_record_commands.h"

s32 ovl_11_func_800E8DC8(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    Ovl11ChangeStateRecordFlag800(arg0, 0, 0, 0);
    temp_v0 = Ovl11ResolveStateField(0x22, arg0, NULL);
    *(s32 *)temp_v0 = gOvl11ValueSlots[arg2];
    *(s32 *)(temp_v0 + 4) = gOvl11ValueSlots[arg1];
    *(s32 *)(temp_v0 + 8) = gOvl11ValueSlots[arg3];
    return 1;
}
