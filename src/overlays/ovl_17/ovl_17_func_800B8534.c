#include "common.h"
#include "game_types.h"

void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);
void ovl_17_func_800BA704(void);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226A4(void);
s32 func_800225B8(void);

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
    func_8002261C(4, 1);
    state = func_800226A4();
    if (state == 2) {
        result = func_800225B8();
        if (result == 1) {
            base[-0xC4] = 4;
        } else if (result == state) {
            ovl_17_func_800BA704();
            base[-0xC4] = result;
        }
    }
}
