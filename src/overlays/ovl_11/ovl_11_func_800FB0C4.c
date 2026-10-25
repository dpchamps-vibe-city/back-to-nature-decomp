#include "common.h"
#include "message_window.h"

s32 ovl_11_func_800D60D4(s16 *arg0);

void ovl_11_func_800FB0C4(s16 *arg0) {
    s32 var_a1 = arg0[0];

    if (var_a1 == 0xA4) {
        var_a1 = ovl_11_func_800D60D4(arg0) + 0x3E8;
    }
    RequestMessageWindow(3, var_a1);
    if (GetMessageWindowStatus() == 2) {
        D_80126FE0 = 1;
        CloseMessageWindow();
    }
}
