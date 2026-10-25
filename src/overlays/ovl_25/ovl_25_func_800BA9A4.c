#include "common.h"
#include "screen_fade.h"
#include "message_window.h"

extern s16 D_800BFE46;

void ovl_25_func_800BA9A4(void) {
    RequestMessageWindow(4, 0x34);
    if (GetMessageWindowStatus() == 2) {
        BeginScreenFadeIncrease(0xA, 0, 2);
        D_800BFE46 = 5;
    }
}
