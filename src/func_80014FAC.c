#include "common.h"
#include "packet_work.h"

void *func_800153BC(void *, s32 *arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s16 arg10);

void func_80014FAC(s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s16 arg9) {
    func_800153BC(ReservePacketWorkBytes(0x24), arg0, arg1, arg2, (s16) (s32) arg3, (s16) (s32) arg4, arg5, arg6, arg7, arg8, (s16) (s32) arg9);
}
