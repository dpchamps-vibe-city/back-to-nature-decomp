#include "common.h"

void ovl_21_func_800BA4C0(void);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
s32 GetVal8005E5B8(void);

void ovl_21_func_800B9590(void) {
    func_8002261C(4, 0x18);
    if (GetVal8005E5B4() == 2) {
        if (GetVal8005E5B8() == 1) {
            D_800C0448[0] = 5;
        }
    }
    ovl_21_func_800BA4C0();
}
