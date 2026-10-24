#include "common.h"

void func_80024A4C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4);

void func_800248E8(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    func_80024A4C(arg0, 0x18, (s32) arg3, (s32) arg1, (s16) (s32) arg2);
}
