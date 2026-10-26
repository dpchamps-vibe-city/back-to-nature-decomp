#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl11_primary_object_state_change.h"

void ovl_11_func_800C8764(Recon_ovl_11_func_800C8334_A0View *arg0) {
    ItemData *item;
    s32 var_a2;

    if (arg0->unk8A == 0) {
        RequestMessageWindow(2, 0x20A);
        return;
    }
    item = &D_8006C858[arg0->unk8A];
    var_a2 = 0;
    if (((item->u0.flags & 0x9000) == 0x9000) || ((*(s32 *) ((u8 *) item + 0x14)) == -1)) {
        var_a2 = 1;
    }
    if (var_a2 != 0) {
        Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0, 0x61, arg0->unk38 + 0x17, -2);
        PlaySoundEffect(9);
    }
}
