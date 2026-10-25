#include "common.h"
#include "screen_fade.h"
#include "message_window.h"

void ovl_11_func_801035C0(void);
void ovl_11_func_80103714(void);

void ovl_11_func_80103424(void) {
    if (IsScreenFadeComplete() != 0) {
        RequestMessageWindow(3, 0x3C1);
        if (GetMessageWindowStatus() == 2) {
            ovl_11_func_801035C0();
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}
