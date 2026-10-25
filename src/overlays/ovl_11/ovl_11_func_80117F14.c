#include "common.h"
#include "message_window.h"

extern s16 D_8012D7A8[];

s32 ovl_11_func_80117F14(s32 arg0) {
    if (arg0 < 0) {
        arg0 = 0;
    }
    if (arg0 >= 5) {
        arg0 = 4;
    }
    CloseMessageWindow();
    return RequestMessageWindow(2, D_8012D7A8[arg0]);
}
