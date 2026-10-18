#include "common.h"
#include "game_types.h"

void ovl_17_func_800BA808 (s32 arg0);
void ovl_17_func_800BAFDC (void);
void ovl_17_func_800BAC24 (Recon_ovl_17_func_800BAD9C_A0View *arg0, s32 arg1);
void ovl_17_func_800B8294 (void);

void ovl_17_func_800B81FC(void) {
    s32 var_s0;
    u8 *var_s1;

    ovl_17_func_800BA808(0x40000);
    ovl_17_func_800BAFDC();
    var_s1 = D_800BD870;
    for (var_s0 = 0; var_s0 < 6; var_s0++) {
        ovl_17_func_800BAC24((Recon_ovl_17_func_800BAD9C_A0View *) var_s1, 0x40000);
        var_s1 += 0x50;
    }
    ovl_17_func_800B8294();
    if (((Ovl17ModeView *) D_8006C838)->mode == 3) {
        Ovl17_D_800BD848.field_4 = 2;
    } else {
        Ovl17_D_800BD848.field_4 = 1;
    }
}
