#include "common.h"
#include "game_types.h"

void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);
void ovl_17_func_800BA704(void);
s32 func_8002261C(s32 arg0, s32 arg1);
/* GetVal8005E5B4, named by its original overlay import. */
s32 func_800226A4(void);

void ovl_17_func_800B861C(void) {
    s32 i;
    u8 *base;
    u8 *record;

    for (i = 0; i < 6; i++) {
        base = D_800BD870 + i * 0x50;
        if (i != 2) {
            ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)base, 0x40000);
        }
    }
    record = D_800BD910;
    ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)record, 0x40000);
    ovl_17_func_800BA808(0x40000);
    func_8002261C(4, 2);
    if (func_800226A4() == 2) {
        ovl_17_func_800BA704();
        record[-0xC4] = 5;
    }
}
