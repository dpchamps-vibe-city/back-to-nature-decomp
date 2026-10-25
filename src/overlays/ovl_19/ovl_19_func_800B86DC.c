#include "common.h"
#include "message_window.h"

void ovl_19_func_800BA770(void);

void ovl_19_func_800B86DC(void) {
    s32 status;
    s32 result;

    ovl_19_func_800BA770();
    switch (D_800BF4C0[4]) {
    case 0:
        D_800BF4C0[2] = 2;
        D_800BF4C0[3] = 0;
        return;
    case 1:
        RequestMessageWindow(4, 0xF);
        break;
    case 2:
        RequestMessageWindow(4, 0x10);
        break;
    }
    status = GetMessageWindowStatus();
    if (status == 2) {
        result = GetMessageWindowChoice();
        if (result == 1) {
            D_800BF4C0[0x100] = 0;
            D_800BF4C0[2] = 3;
        } else if (result == status) {
            D_800BF4C0[2] = result;
            D_800BF4C0[3] = 0;
        }
    }
}
