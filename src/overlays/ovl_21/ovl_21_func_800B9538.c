#include "common.h"

void ovl_21_func_800BA4C0(void);
s32 GetVal8005E5B4(void);
s32 GetVal8005E5B8(void);

void ovl_21_func_800B9538(void) {
    if (GetVal8005E5B4() == 2) {
        if (GetVal8005E5B8() == 1) {
            D_800C0448[0] = 5;
        } else {
            D_800C0448[0] = 0xF;
        }
    }
    ovl_21_func_800BA4C0();
}
