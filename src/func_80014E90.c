#include "common.h"

s32 func_80011F5C(s32 arg0);

void *func_8001526C(void *, u32 *, s16 x0, s16 y0, s32 color, s16 cond);

void func_80014E90(u32 *arg0, s16 arg1, s16 arg2, s32 arg3, s16 arg4) {
    func_8001526C(func_80011F5C(0xC), arg0, arg1, arg2, arg3, (s16) (s32) arg4);
}
