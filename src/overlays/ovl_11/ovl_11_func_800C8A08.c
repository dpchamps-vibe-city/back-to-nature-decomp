#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl11_primary_object_requests.h"

s32 ovl_11_func_800F3BA0(u16 arg0);
void ovl_11_func_800F3AF0(u16 arg0, u16 arg1);
s32 ovl_11_func_800C7270(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800C8A08(Recon_ovl_11_func_800C7270_A0View *arg0) {
    u16 item[3];
    s16 temp_v1;
    s32 temp_v0;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x8A);
    if (temp_v1 == 0) {
        temp_v0 = ovl_11_func_800F3BA0(1U);
        if (temp_v0 == 1) {
            item[0] = 0x69;
            item[2] = (u16) temp_v0;
            Ovl11InstallPrimaryObjectRequest(item);
            return;
        }
        RequestMessageWindow(2, 0x19F);
        return;
    }
    if (temp_v1 == 0x69) {
        arg0->unk6C &= 0xFFFF3FFF;
        ovl_11_func_800F3AF0(0U, 1U);
        PlaySoundEffect(7);
        ovl_11_func_800C7270(arg0, 0x64, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
        return;
    }
    RequestMessageWindow(2, 0x19F);
}
