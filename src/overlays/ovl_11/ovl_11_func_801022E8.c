#include "common.h"
#include "screen_fade.h"
#include "message_window.h"

extern s32 gOvl11SelectionHandler;
extern s8 gOvl11SelectionModePhase;
extern void ovl_11_func_8010289C(void);

void ovl_11_func_801022E8(void) {
    RequestMessageWindow(3, 0x3BF);
    if (GetMessageWindowStatus() == 2) {
        if ((s16)GetMessageWindowChoice() == 1) {
            BeginScreenFadeIncrease(0xA, 0, 2);
            gOvl11SelectionHandler = (s32)ovl_11_func_8010289C;
        } else {
            gOvl11SelectionModePhase = 3;
        }
    }
}
