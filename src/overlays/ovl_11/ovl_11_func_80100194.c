#include "common.h"
#include "message_window.h"

extern s32 D_801273DC;
extern void ovl_11_func_8010021C(void);
extern void ovl_11_func_80100FFC(void);
extern void ovl_11_func_80103714(void);

void ovl_11_func_80100194(void) {
    s32 var;
    s16 val;

    RequestMessageWindow(3, 0x3B7);
    var = GetMessageWindowStatus();
    if (var == 2) {
        val = GetMessageWindowChoice();
        if (val == 1) {
            D_801273DC = (s32)ovl_11_func_8010021C;
        } else if (val == 2) {
            D_801273DC = (s32)ovl_11_func_80100FFC;
        } else {
            D_801273DC = (s32)ovl_11_func_80103714;
        }
    }
}
