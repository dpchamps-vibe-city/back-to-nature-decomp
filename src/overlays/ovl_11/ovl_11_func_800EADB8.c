#include "common.h"
#include "ovl11_state_record_commands.h"
#include "psyq/stddef.h"
#include "ovl11_state_record_motion_limit.h"

void ovl_11_func_800EADB8(s16 arg0, s16 arg1, s32 arg2, s16 arg3) {
    Ovl11StateRecordAnimationCommand(arg0, arg3, 0, 0);
    Ovl11LimitStateRecordMotion(arg0, arg1, arg2, 0x14);
}
