#include "common.h"
#include "message_window.h"

void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B8654(void) {
    s32 temp_v0;

    ovl_21_func_800BA4C0();
    RequestMessageWindow(4, 0x18);
    temp_v0 = GetMessageWindowStatus();
    if (temp_v0 == 2) {
        if (GetMessageWindowChoice() == 1) {
            D_800C0448[0] = temp_v0;
        } else {
            D_800C0448[0] = 1;
        }
    }
}
