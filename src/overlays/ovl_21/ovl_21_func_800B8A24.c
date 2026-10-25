#include "common.h"
#include "message_window.h"

void ovl_21_func_800B8320(void);
void ovl_21_func_800BA4C0(void);
/* Executable aliases of SetVal8005E2BC and SetMessagePortraitEnabled. */
void SetVal8005E2BC(s32 arg0);

void ovl_21_func_800B8A24(void) {
    u8 *base;

    ovl_21_func_800B8320();
    ovl_21_func_800BA4C0();
    CloseMessageWindow();
    SetVal8005E2BC(0);
    SetMessagePortraitEnabled(0);
    RequestMessageWindow(4, 0x1A);
    base = (u8 *)D_800C0448;
    *(s16 *)(base + 0x988) = 0;
    *(s16 *)base = 6;
}
