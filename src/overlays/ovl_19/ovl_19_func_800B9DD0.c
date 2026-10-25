#include "common.h"
#include "game_types.h"
#include "ovl19_actor_controls.h"

void ovl_19_func_800BA0A0(Recon_ovl_19_func_800BA0A0_A0View *arg0);

s32 ovl_19_func_800B9DD0(Recon_ovl_19_func_800B9DD0_A0View *arg0) {
    ovl_19_func_800BA0A0(((s32)arg0));
    if (arg0->unk6 == 0) {
        Ovl19SetActorControl(((Ovl19Func800BAC40Arg *)(((s32)arg0))), 2, arg0->unk4, 0);
    } else {
        return arg0->unk6;
    }
}
