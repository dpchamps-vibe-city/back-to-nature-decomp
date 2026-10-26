#include "common.h"
#include "message_window.h"

extern s32 gOvl11SelectionHandler;
extern void ovl_11_func_80100194(void);
extern void ovl_11_func_80103714(void);

void ovl_11_func_80100128(void) {
    RequestMessageWindow(3, 0x3B6);
    if (GetMessageWindowStatus() == 2) {
        if ((s16)GetMessageWindowChoice() == 1) {
            gOvl11SelectionHandler = (s32)ovl_11_func_80100194;
        } else {
            gOvl11SelectionHandler = (s32)ovl_11_func_80103714;
        }
    }
}
