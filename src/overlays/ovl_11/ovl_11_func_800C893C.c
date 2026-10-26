#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_primary_object_state_change.h"

s32 ovl_11_func_800F3A18(u16 arg0);
void ovl_11_func_800F397C(u16 arg0, u16 arg1);

void ovl_11_func_800C893C(Recon_ovl_11_func_800C7270_A0View *arg0) {
    u16 item[3];
    s16 temp_v1;
    s32 temp_v0;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x8A);
    if (temp_v1 == 0) {
        temp_v0 = ovl_11_func_800F3A18(1U);
        if (temp_v0 == 1) {
            item[0] = 0x68;
            item[2] = (u16) temp_v0;
            Ovl11InstallPrimaryObjectRequest(item);
            return;
        }
        RequestMessageWindow(2, 0x19C);
        return;
    }
    if (temp_v1 == 0x68) {
        arg0->unk6C &= 0xFFFF3FFF;
        ovl_11_func_800F397C(0U, 1U);
        PlaySoundEffect(7);
        Ovl11ChangeObjectStateDirect(arg0, 0x65, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
        return;
    }
    RequestMessageWindow(2, 0x19C);
}
