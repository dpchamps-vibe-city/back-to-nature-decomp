#include "common.h"
#include "screen_fade.h"
#include "message_window.h"

void ovl_11_func_800BCF28(void);
u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void ovl_11_func_800F9E4C(void);

void ovl_11_func_800F9DB0(void) {
    u32 previous;

    if (D_80126F7C != 0) {
        DrawScreenSubtractOverlay(0x40000, 0x40000, 0x40000);
        if (D_80126F7C == 2) {
            if (GetMessageWindowStatus() == 2) {
                ResetMessageWindow();
                D_80126F7C = 0;
                ovl_11_func_800BCF28();
            }
        } else {
            previous = func_80017A64();
            func_80017A48(3);
            ovl_11_func_800F9E4C();
            func_80017A48(previous);
        }
    }
}
