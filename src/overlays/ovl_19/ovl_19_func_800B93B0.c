#include "common.h"
#include "message_window.h"

/* ovl_19 bss state base, referenced with absolute addressing (lui + %lo). */
extern s16 D_800BF4C0[];

void ovl_19_func_800B93B0(void) {
    s32 temp_s0;
    s32 temp_v0;

    RequestMessageWindow(4, 0xE);
    temp_s0 = GetMessageWindowStatus();
    if (temp_s0 == 2) {
        temp_v0 = GetMessageWindowChoice();
        D_800BF4C0[3] = 0;
        if (temp_v0 == 1) {
            D_800BF4C0[0x100] = 0;
            D_800BF4C0[2] = 3;
        } else if (temp_v0 == temp_s0) {
            D_800BF4C0[2] = temp_v0;
        }
    }
}
