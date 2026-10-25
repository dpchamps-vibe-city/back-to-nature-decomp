#include "common.h"
#include "screen_fade.h"
#include "message_window.h"

extern s32 D_801273DC;
extern s8 D_801273E4;
extern void ovl_11_func_8010289C(void);

void ovl_11_func_801022E8(void) {
    RequestMessageWindow(3, 0x3BF);
    if (GetMessageWindowStatus() == 2) {
        if ((s16)GetMessageWindowChoice() == 1) {
            BeginScreenFadeIncrease(0xA, 0, 2);
            D_801273DC = (s32)ovl_11_func_8010289C;
        } else {
            D_801273E4 = 3;
        }
    }
}
