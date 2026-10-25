#include "common.h"
#include "game_types.h"
#include "message_window.h"

void ovl_17_func_800BAFDC(void);
void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);

void ovl_17_func_800B8470(void) {
    s32 i;
    u8 *base;
    u8 *work;

    D_800BB6F8++;
    if (D_800BB6F8 >= 30) {
        D_800BB6F8 = 0;
        work = D_800BD848;
        *(s16 *)(work + 0xCA) -= 100;
        if (*(s16 *)(work + 0xCA) < 0) {
            *(s16 *)(work + 0xCA) = 255;
        }
    }
    ovl_17_func_800BAFDC();
    base = D_800BD870;
    for (i = 0; i < 6; i++) {
        ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        base += 0x50;
    }
    ovl_17_func_800BA808(0x40000);
    RequestMessageWindow(4, 0);
    if (GetMessageWindowStatus() == 2) {
        D_800BD84C = 3;
    }
}
