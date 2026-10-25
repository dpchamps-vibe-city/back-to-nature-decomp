#include "common.h"
#include "screen_fade.h"
#include "message_window.h"

void ovl_11_func_80100B50(u8 arg0, u8 arg1);
void ovl_11_func_80100F9C(s32 arg0);
void ovl_11_func_8010289C(void);
void ovl_11_func_80100FFC(void);
void ovl_11_func_8010021C(void);
void ovl_11_func_80103714(void);

void ovl_11_func_801007AC(void) {
    s32 state;
    s16 choice;

    ovl_11_func_80100B50((u8)D_801273EC, 0);
    RequestMessageWindow(3, 0x3BB);
    state = GetMessageWindowStatus();
    if (state == 2) {
        choice = GetMessageWindowChoice();
        if (choice == 1) {
            D_801273E6 = D_801273EC;
            ovl_11_func_80100F9C((u8)D_801273EC);
            BeginScreenFadeIncrease(10, 0, 2);
            Ovl11_D_801273DC = ovl_11_func_8010289C;
        } else if (choice == 2) {
            ovl_11_func_80100F9C((u8)D_801273EC);
            Ovl11_D_801273DC = ovl_11_func_80100FFC;
        } else if (choice == 3) {
            Ovl11_D_801273DC = ovl_11_func_8010021C;
        } else {
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}
