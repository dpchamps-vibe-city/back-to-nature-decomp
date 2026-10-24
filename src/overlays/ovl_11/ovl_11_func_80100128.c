#include "common.h"

extern s32 D_801273DC;
extern void ovl_11_func_80100194(void);
extern void ovl_11_func_80103714(void);

s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
s32 GetVal8005E5B8(void);

void ovl_11_func_80100128(void) {
    func_8002261C(3, 0x3B6);
    if (GetVal8005E5B4() == 2) {
        if ((s16)GetVal8005E5B8() == 1) {
            D_801273DC = (s32)ovl_11_func_80100194;
        } else {
            D_801273DC = (s32)ovl_11_func_80103714;
        }
    }
}
