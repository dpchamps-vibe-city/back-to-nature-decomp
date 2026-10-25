#include "common.h"
#include "game_types.h"
#include "ovl19_actor_controls.h"

void ovl_19_func_800BA0A0(Recon_ovl_19_func_800BA0A0_A0View *arg0);

void ovl_19_func_800B9F40(Recon_ovl_19_func_800BA0A0_A0View *arg0) {
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 6);
    if ((temp_v1 < (D_800BF650 - 0x1E)) && (temp_v1 < (D_800BF650 - 0x3C))) {
        ovl_19_func_800BA0A0(arg0);
    }
    if ((*(s16 *) ((u8 *) arg0 + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) arg0, 2, (s32) arg0->unk4, 0);
    }
}
