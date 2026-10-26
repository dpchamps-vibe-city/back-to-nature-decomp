#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl11_primary_object_state_change.h"

void ovl_11_func_800C8334(Recon_ovl_11_func_800C8334_A0View *arg0) {
    if (arg0->unk8A == 0x47) {
        Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *)arg0, 0x5C, arg0->unk38 + 0x17, -2);
        PlaySoundEffect(0x13);
    } else {
        RequestMessageWindow(2, 0x201);
    }
}
