#include "common.h"

s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800F8B4C(s32 arg0, s32 arg1) {
    s32 firstDisplay;
    s16 *end;

    end = D_80129FD8;
    end = func_8001A970(D_80126E4A + 1, end, 1);
    firstDisplay = arg0;
    *(u16 *)end = 0xFFFF;
    end = D_80129FD8;
    func_80017B3C(firstDisplay, (s32)end, 0x28, 0x6E);
    func_80017B3C(arg0, (s32)((u8 *)D_80051BF0 + D_80054BBC[0]), 0x35, 0x6E);
    *(u16 *)func_8001A970(arg1 / 8, D_80129FD8, 1) = 0xFFFF;
    func_80017B3C(arg0, (s32)D_80129FD8, 0x42, 0x6E);
}
