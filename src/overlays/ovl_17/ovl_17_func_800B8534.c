#include "common.h"
#include "game_types.h"
#include "message_window.h"

void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);
void ovl_17_func_800BA704(void);

void ovl_17_func_800B8534(void) {
    s32 i;
    s32 state;
    s32 result;
    s32 skip;
    u8 *base;
    u8 *record;
    u8 *origin;

    i = 0;
    skip = 2;
    record = D_800BD870;
    origin = record;
    for (; i < 6; i++) {
        if (i == skip) {
            record = origin + 0xF0;
            i = 3;
        }
        ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)record, 0x40000);
        record += 0x50;
    }
    base = D_800BD910;
    ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
    ovl_17_func_800BA808(0x40000);
    RequestMessageWindow(4, 1);
    state = GetMessageWindowStatus();
    if (state == 2) {
        result = GetMessageWindowChoice();
        if (result == 1) {
            base[-0xC4] = 4;
        } else if (result == state) {
            ovl_17_func_800BA704();
            base[-0xC4] = result;
        }
    }
}
