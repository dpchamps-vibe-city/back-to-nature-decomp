#include "common.h"

s32 func_80011F5C(s32);
void func_80011FD8(s32);
void *func_800165D8(u32 *, void *, void *, u8 arg3, u8 arg4, s16 arg5, s16 arg6, s32, s32, s32, u16 arg10, s16 arg11, s16 arg12, s16 arg13, s16 arg14);

void func_800161AC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7, u16 arg8) {
    s32 var_s8;
    s32 var_s0;

    var_s8 = arg2 & 0xFF;
    var_s0 = arg3 & 0xFF;
    func_80011FD8(func_800165D8(arg0, func_80011F5C(0), arg1, var_s8, var_s0, (s32)arg4, (s32)arg5, arg6, arg7, 0, (s32)arg8, -1, -1, -1, -1));
}
