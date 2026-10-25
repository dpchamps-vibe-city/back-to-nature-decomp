#include "common.h"
#include "packet_work.h"

u8 *func_80018B98(s32 *arg0, u8 *arg1, u16 *, s16, s16, s32, s32, s32, s32, s32); /* extern */

void func_80017D9C(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4) {
    SetPacketWorkCursor(func_80018B98(arg0, ReservePacketWorkBytes(0), arg1, arg2, arg3, 0x1000, 0, arg4, 0, 0));
}
