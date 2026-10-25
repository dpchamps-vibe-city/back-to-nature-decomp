#include "common.h"
#include "message_window.h"

void ovl_21_func_800B8320(void);
void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B86B8(void) {
    u8 *base;

    ovl_21_func_800B8320();
    ovl_21_func_800BA4C0();
    RequestMessageWindow(4, 0x19);
    if (GetMessageWindowStatus() == 2) {
        CloseMessageWindow();
        base = (u8 *)D_800C0448;
        *(s16 *)(base + 0x988) = 0;
        *(s16 *)base = 3;
    }
}
