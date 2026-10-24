#include "common.h"

void ovl_21_func_800BA4C0(void);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
s32 GetVal8005E5B8(void);

void ovl_21_func_800B8654(void) {
    s32 temp_v0;

    ovl_21_func_800BA4C0();
    func_8002261C(4, 0x18);
    temp_v0 = GetVal8005E5B4();
    if (temp_v0 == 2) {
        if (GetVal8005E5B8() == 1) {
            D_800C0448[0] = temp_v0;
        } else {
            D_800C0448[0] = 1;
        }
    }
}
