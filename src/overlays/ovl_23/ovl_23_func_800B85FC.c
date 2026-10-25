#include "common.h"
#include "message_window.h"

void ovl_23_func_800B9310(void);

void ovl_23_func_800B85FC(void) {
    ovl_23_func_800B9310();
    RequestMessageWindow(4, 0x2C);
    if (GetMessageWindowStatus() == 2) {
        Ovl23_D_800BF87C_bounds.unk0 = 4;
    }
}
