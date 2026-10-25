#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "ovl19_actor_controls.h"

void ovl_19_func_800BA0A0(Recon_ovl_19_func_800BA0A0_A0View *arg0);

void ovl_19_func_800B9FB0(Recon_ovl_19_func_800BA0A0_A0View *arg0) {
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 6);
    if ((temp_v1 < (D_800BF4C0[0xC9] - 0x1E)) && (temp_v1 < (D_800BF4C0[0xC9] - 0x2D))) {
        ovl_19_func_800BA0A0(arg0);
    }
    if ((*(s16 *) ((u8 *) arg0 + 6)) == 0) {
        Ovl19SetActorControl((Ovl19Func800BAC40Arg *) arg0, 2, (s32) arg0->unk4, 0);
    }
    if ((*(s16 *) ((u8 *) arg0 + 6)) == (D_800BF4C0[0xC9] - 0x1E)) {
        D_800BF4C0[0x101] = PlaySoundEffect(0x3C);
    }
}
