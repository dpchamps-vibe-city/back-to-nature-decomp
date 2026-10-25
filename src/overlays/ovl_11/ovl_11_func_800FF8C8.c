#include "common.h"
#include "message_window.h"

extern s32 D_801273B0;

void SetVal8005E2BC(s32 arg0);

void ovl_11_func_800FF8C8(s16 arg0, s16 arg1) {
    s32 var_a1;

    if (D_801273B0 == 0x21) {
        if (arg1 == 0) {
            var_a1 = arg0 + 0x37B;
        } else {
            var_a1 = arg1 + 0x37E;
        }
        RequestMessageWindow(3, var_a1);
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            D_801273B0 = 0x20;
        }
    }
    if (D_801273B0 == 0x20) {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3B3);
    }
}
