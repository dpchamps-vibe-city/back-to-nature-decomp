#include "common.h"
#include "message_window.h"

extern s32 D_801273B0;

void SetVal8005E2BC(s32 arg0);

void ovl_11_func_800FF27C(s16 arg0, s16 arg1) {
    s32 var_a1;

    if (D_801273B0 == 0x11) {
        var_a1 = arg0 * 5 + 0x367;
        RequestMessageWindow(3, arg1 + var_a1);
        if (GetMessageWindowStatus() == 2) {
            CloseMessageWindow();
            D_801273B0 = 0x10;
        }
    }
    if (D_801273B0 == 0x10) {
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(3, 0x3B2);
    }
}
