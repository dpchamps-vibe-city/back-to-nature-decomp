#include "common.h"

u8 *func_80018B98(s32 *arg0, u8 *arg1, u16 *, s16, s16, s32, s32, s32, s32, s32); /* extern */

s32 func_80011F5C(s32 arg0);

void func_80011FD8(s32 arg0);

void func_80017D9C(s32 *arg0, u16 *arg1, s16 arg2, s16 arg3, s32 arg4) {
    func_80011FD8(func_80018B98(arg0, func_80011F5C(0), arg1, arg2, arg3, 0x1000, 0, arg4, 0, 0));
}
