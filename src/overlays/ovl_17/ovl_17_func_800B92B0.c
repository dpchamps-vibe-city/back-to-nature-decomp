#include "common.h"
#include "game_types.h"

void ovl_17_func_800BAF50(void);
void ovl_17_func_800BAD9C(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);

void ovl_17_func_800B92B0(void) {
    s32 i;
    u8 *record;
    u8 *base;
    u8 *work;
    u8 *display;

    ovl_17_func_800BAF50();
    base = D_800BD870;
    work = base - 0x28;
    record = base;
    for (i = 0; i < 6; i++) {
        ovl_17_func_800BAD9C((Recon_ovl_17_func_800BAD9C_A0View *) record,
                            *(s32 *) (work + 0x24));
        record += 0x50;
    }
    display = D_800BD848;
    ovl_17_func_800BA808(*(s32 *) (display + 0x24));
}
