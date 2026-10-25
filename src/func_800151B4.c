#include "common.h"
#include "packet_work.h"

void *func_80015644(void *, s32 *arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s16 arg7, u8 arg8);

void func_800151B4(s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6, u8 arg7) {
    SetPacketWorkCursor((s32)func_80015644(ReservePacketWorkBytes(0), arg0, arg1, arg2, (s16) (s32) arg3, (s16) (s32) arg4, arg5, (s16) (s32) arg6, arg7));
}
