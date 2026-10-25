#include "common.h"
#include "game_types.h"
#include "random.h"

s32 ovl_11_func_8010B57C(u16 *arg0, s32 arg1);

s32 ovl_11_func_8010B8DC(Recon_ovl_11_func_8010CE80_A0View *arg0) {
    s32 mode;
    s32 random;
    u16 current;

    if ((u32)(D_80070CF8_halfword - 6) < 15) {
        mode = 1;
        current = arg0->unk26;
        if (arg0->unk26 != 15) {
            if (arg0->unk34 & 0x2000) {
                random = Rand(7);
                mode = 17;
            } else if (arg0->unk34 & 0x8000) {
                random = Rand(2);
                mode = 18;
            } else {
                random = Rand(2);
                mode = 18;
            }
            switch (random) {
            case 0:
                break;
            case 1:
                mode = 2;
                break;
            }
            current = arg0->unk26;
        }
    } else {
        mode = 2;
        current = arg0->unk26;
    }
    if (mode != (s16)current) {
        ovl_11_func_8010B57C((u16 *)arg0, mode);
    }
    return 0;
}
