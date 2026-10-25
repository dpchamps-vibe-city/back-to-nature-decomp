#include "common.h"
#include "message_window.h"

void ovl_23_func_800B9310(void);

void ovl_23_func_800B853C(void) {
    s32 state;

    ovl_23_func_800B9310();
    RequestMessageWindow(4, 0x2A);
    state = GetMessageWindowStatus();
    if (state == 2) {
        Ovl23_D_800BF87C_bounds.unk0 = state;
    }
}
