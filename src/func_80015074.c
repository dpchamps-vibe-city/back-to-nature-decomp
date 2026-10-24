#include "common.h"

s32 func_80011F5C(s32 arg0);

void *func_800154CC(void *, s32 *arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, s16 arg7);

void func_80015074(s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6) {
    func_800154CC(func_80011F5C(0x18), arg0, arg1, arg2, (s16) (s32) arg3, (s16) (s32) arg4, arg5, (s16) (s32) arg6);
}
