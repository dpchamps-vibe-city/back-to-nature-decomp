#include "common.h"
#include "game_types.h"

void ovl_19_func_800BA0A0(Recon_ovl_19_func_800BA0A0_A0View *arg0);
void ovl_19_func_800BAC40(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_8001FABC(s16 arg0);

void ovl_19_func_800B9FB0(Recon_ovl_19_func_800BA0A0_A0View *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 6);
    if ((temp_v1 < (D_800BF4C0[0xC9] - 0x1E)) && (temp_v1 < (D_800BF4C0[0xC9] - 0x2D))) {
        ovl_19_func_800BA0A0(arg0);
    }
    if ((*(s16 *) ((u8 *) arg0 + 6)) == 0) {
        ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *) arg0, 2, (s32) arg0->unk4, 0);
    }
    if ((*(s16 *) ((u8 *) arg0 + 6)) == (D_800BF4C0[0xC9] - 0x1E)) {
        D_800BF4C0[0x101] = func_8001FABC(0x3C);
    }
}
