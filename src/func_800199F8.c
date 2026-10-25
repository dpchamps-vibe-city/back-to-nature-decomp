#include "common.h"
#include "packet_work.h"

u8 *func_80018B98(s32 *arg0, u8 *arg1, u16 *, s16, s16, s32, s32, s32, s32, s32); /* extern */

void func_80017A38(s16 arg0, s16 arg1);

s32 func_800198E0(u16 *arg0, s32 arg1, s16 arg2);

s32 func_800199F8(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, s16 arg7) {
    s32 temp_s0;

    func_80017A38(arg7, 0);
    temp_s0 = (arg3 + arg5) - func_800198E0(arg2, arg6, arg7);
    SetPacketWorkCursor(func_80018B98(arg0, ReservePacketWorkBytes(0), arg2, (s16) temp_s0, arg4, arg6, 0, 0, 0, 0));
    return ReservePacketWorkBytes(0);
}
