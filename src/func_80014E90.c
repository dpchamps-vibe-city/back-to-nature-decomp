#include "common.h"
#include "packet_work.h"

void *func_8001526C(void *, u32 *, s16 x0, s16 y0, s32 color, s16 cond);

void func_80014E90(u32 *arg0, s16 arg1, s16 arg2, s32 arg3, s16 arg4) {
    func_8001526C(ReservePacketWorkBytes(0xC), arg0, arg1, arg2, arg3, (s16) (s32) arg4);
}
