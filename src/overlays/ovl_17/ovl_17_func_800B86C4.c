#include "common.h"
#include "game_types.h"

void func_80017A08(s32 arg0, s32 arg1);
void ovl_17_func_800BAFDC(void);
void ovl_17_func_800BAC24(Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800BA808(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
/* GetVal8005E5B4, named by its original overlay import. */
s32 func_800226A4(void);

void ovl_17_func_800B86C4(void) {
    s32 i = 0;
    u8 *base;
    s32 data = (s32)D_800556A8;
    u8 *work;
    u8 *state;

    do {
        base = D_800BD848;
        func_80017A08((u16)i, *(u16 *)(base + i * 0x50 + 0x40) + data);
        i++;
    } while (i < 6);
    ovl_17_func_800BAFDC();
    work = D_800BD870;
    for (i = 0; i < 6; i++) {
        ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *)work, 0x40000);
        work += 0x50;
    }
    ovl_17_func_800BA808(0x40000);
    func_8002261C(4, 4);
    if (func_800226A4() == 2) {
        state = D_800BD848;
        *(s16 *)(state + 0x2CE) = 0;
        state[4] = i;
    }
}
