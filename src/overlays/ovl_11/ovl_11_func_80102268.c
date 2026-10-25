#include "common.h"
#include "message_window.h"

extern s8 D_801273E4;

s32 ovl_11_func_800D60D4(s16 *arg0);
void *ovl_11_func_80102844(s8 arg0, s8 arg1);

void ovl_11_func_80102268(s8 arg0, s8 arg1) {
    s16 *p;
    s32 v;

    p = ovl_11_func_80102844(arg1, arg0);
    v = p[0];
    if (v == 0xA4) {
        RequestMessageWindow(3, ovl_11_func_800D60D4(p) + 0x3E8);
    } else {
        RequestMessageWindow(3, v);
    }
    if (GetMessageWindowStatus() == 2) {
        D_801273E4 = 3;
    }
}
