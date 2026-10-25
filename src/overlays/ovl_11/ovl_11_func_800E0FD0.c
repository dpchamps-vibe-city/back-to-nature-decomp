#include "common.h"
#include "game_types.h"
#include "random.h"


s32 ovl_11_func_800E0C24(Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);

s32 ovl_11_func_800E0FD0(Recon_ovl_11_func_8010CE80_A0View *arg0) {
    s32 var_a0;
    s32 var_a1;
    s32 temp_v0;

    if ((u32)(D_80070CF8_halfword - 6) >= 0xF) {
        var_a1 = 0xF;
    } else {
        if (arg0->unk26 == 0xF) {
            var_a1 = 0x10;
        } else {
            var_a0 = 3;
            if (arg0->unk34 & 0x2000) {
                var_a0 = 9;
            }
            temp_v0 = Rand(var_a0);
            switch (temp_v0) {
            case 0:
                var_a1 = 0x12;
                break;
            case 1:
                var_a1 = 0xE;
                break;
            case 2:
                var_a1 = 0xF;
                break;
            default:
                var_a1 = 0x12;
                break;
            }
        }
    }
    if (var_a1 != arg0->unk26) {
        ovl_11_func_800E0C24(arg0, var_a1);
    }
    return 0;
}
