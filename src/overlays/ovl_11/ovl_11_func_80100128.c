#include "common.h"
#include "message_window.h"

extern s32 D_801273DC;
extern void ovl_11_func_80100194(void);
extern void ovl_11_func_80103714(void);

void ovl_11_func_80100128(void) {
    RequestMessageWindow(3, 0x3B6);
    if (GetMessageWindowStatus() == 2) {
        if ((s16)GetMessageWindowChoice() == 1) {
            D_801273DC = (s32)ovl_11_func_80100194;
        } else {
            D_801273DC = (s32)ovl_11_func_80103714;
        }
    }
}
