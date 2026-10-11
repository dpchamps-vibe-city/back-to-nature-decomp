#include "common.h"
#include "game_types.h"

void ovl_19_func_800BA0A0(Recon_ovl_19_func_800BA0A0_A0View *arg0);
void ovl_19_func_800BAC40(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_19_func_800B9E78(Recon_ovl_19_func_800BA0A0_A0View *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if ((D_800BF64E - 0x1E) >= (*(s16 *) ((u8 *) arg0 + 6))) {
        ovl_19_func_800BA0A0(arg0);
    }
    if ((*(s16 *) ((u8 *) arg0 + 6)) == 0) {
        ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *) arg0, 2, (s32) arg0->unk4, 0);
    }
}
