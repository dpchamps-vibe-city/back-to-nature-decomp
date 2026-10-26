#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "ovl11_primary_object_requests.h"

s32 ovl_11_func_800C7270(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800F3898(u16 arg0, u16 arg1);
s32 ovl_11_func_800F3950(u16 arg0);

void ovl_11_func_800C84DC(Recon_ovl_11_func_800C7270_A0View *arg0) {
    u16 item[3];
    s16 temp_v1;

    temp_v1 = *(s16 *) ((u8 *) arg0 + 0x8A);
    if (temp_v1 == 0x3F || temp_v1 == 0x13D) {
        ovl_11_func_800C7270(arg0, 0x5D, (*(u16 *) ((u8 *) arg0 + 0x38)) + 0x17, -2);
        ovl_11_func_800F3898(0U, 1U);
        PlaySoundEffect(7);
        return;
    } else if (temp_v1 == 0) {
        if (ovl_11_func_800F3950(1U) != 0) {
            PlaySoundEffect(7);
            item[0] = 0x3F;
            item[2] = 1;
            Ovl11InstallPrimaryObjectRequest(item);
        }
        return;
    }
}
