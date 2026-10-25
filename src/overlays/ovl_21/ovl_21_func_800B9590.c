#include "common.h"
#include "message_window.h"

void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B9590(void) {
    RequestMessageWindow(4, 0x18);
    if (GetMessageWindowStatus() == 2) {
        if (GetMessageWindowChoice() == 1) {
            D_800C0448[0] = 5;
        }
    }
    ovl_21_func_800BA4C0();
}
