#include "common.h"
#include "game_types.h"

/* Legacy overlay name for Rand; prototype witnessed by src/Rand.c. */
u32 Rand(s32 arg0);
s32 ovl_11_func_8010CE80(Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);
void ovl_11_func_80110E28(u8 *arg0);
void ovl_11_func_80110E34(Ovl11MotionState *arg0);

s32 ovl_11_func_8010CB6C(Ovl11ObjectCB6C *arg0) {
    s32 increment;
    u16 narrow;
    s32 index;
    s32 x;
    x = arg0->unk34;
    x &= 2;
    if (x) {
        arg0->unk16 += 1;
    }
    if (arg0->unkB8 & 7) {
        arg0->unk16 -= 2;
    }
    if (arg0->unkB8 & 1) {
        increment = arg0->unkAE;
        increment += 1;
        narrow = increment;
        arg0->unkAE = increment;
        index = increment;
        if (narrow > 255) {
            index = 255;
        }
        arg0->unkAE = index;
    }
    if (arg0->unkB8 & 2) {
        arg0->unkB0 += 1;
        arg0->unkB0 = arg0->unkB0 > 255 ? 255 : arg0->unkB0;
    }
    if (arg0->unkB8 & 4) {
        arg0->unkB2 += 1;
        arg0->unkB2 = arg0->unkB2 > 255 ? 255 : arg0->unkB2;
    }
    arg0->unk16 = arg0->unk16 < 0 ? 0 : arg0->unk16;
    arg0->unk16 = arg0->unk16 > 255 ? 255 : arg0->unk16;
    ovl_11_func_80110E28((u8 *)arg0);
    if (arg0->unkB4 != 0) {
        arg0->unkB4 -= 1;
    }
    if (arg0->unkB4 != 0) {
        ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *)arg0, 11);
        x = -3300;
        index = arg0->unkAC;
        increment = 1;
        arg0->unk30 = increment;
        arg0->unk22 = 0;
        arg0->unk38 = x;
        arg0->unk3C = 0;
        increment = 5800;
        increment -= index * 200;
        arg0->unk40 = increment;
    } else {
        arg0->unkB8 &= 0xFFF8;
        ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *)arg0, 0);
        arg0->unkBE = 0;
        ovl_11_func_80110E34((Ovl11MotionState *)arg0);
    }
    arg0->unk2C = Rand(300);
    arg0->unkB8 &= 0xFCA7;
    arg0->unk34 &= ~2;
    arg0->unk34 &= ~0x2000;
    return 0;
}
