#include "common.h"
#include "screen_fade.h"
#include "message_window.h"

extern s32 D_801273DC;
extern void ovl_11_func_80103714(void);

void ovl_11_func_801033D4(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C0);
        if (GetMessageWindowStatus() == 2) {
            D_801273DC = (s32)ovl_11_func_80103714;
        }
    }
}
