#include "common.h"
#include "screen_fade.h"

extern s32 D_801273DC;
extern s8 D_801273E4;
extern void ovl_11_func_8010289C(void);

s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
s32 GetVal8005E5B8(void);

void ovl_11_func_801022E8(void) {
    func_8002261C(3, 0x3BF);
    if (GetVal8005E5B4() == 2) {
        if ((s16)GetVal8005E5B8() == 1) {
            BeginScreenFadeIncrease(0xA, 0, 2);
            D_801273DC = (s32)ovl_11_func_8010289C;
        } else {
            D_801273E4 = 3;
        }
    }
}
