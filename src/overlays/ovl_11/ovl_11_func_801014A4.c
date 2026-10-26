#include "common.h"
#include "message_window.h"

extern s8 gOvl11SelectionModePhase;
extern u16 D_801273F4[];

void ovl_11_func_801014A4(s8 arg0) {
    RequestMessageWindow(3, D_801273F4[arg0]);
    if (GetMessageWindowStatus() == 2) {
        gOvl11SelectionModePhase = 3;
    }
}
