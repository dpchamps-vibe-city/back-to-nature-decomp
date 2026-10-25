#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "message_window.h"

s32 ovl_11_func_800C7270(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800D0EA4(s32 arg0, s32 arg1);

void ovl_11_func_800C8670(Recon_ovl_11_func_800C8334_A0View *arg0, s16 *arg1) {
    s32 matches;

    if (arg0->unk8A == 0x68) {
        matches = 0;
        if (arg0->unk38 == arg1[4]) {
            matches = 1;
        }
        if (arg1[4] == 6 && (arg0->unk38 == 1 || arg0->unk38 == 3)) {
            matches = 1;
        }
        if (arg1[4] == 7 && (arg0->unk38 == 2 || arg0->unk38 == 0)) {
            matches = 1;
        }
        if (matches) {
            ((Recon_ovl_11_func_800C7270_A0View *)arg0)->unk6C |= 0x4000;
            PlaySoundEffect(7);
            ovl_11_func_800C7270((Recon_ovl_11_func_800C7270_A0View *)arg0,
                0x5F, arg0->unk38 + 0x17, -2);
            ovl_11_func_800D0EA4(arg1[2], 1);
        }
    } else {
        RequestMessageWindow(2, 0x1E4);
    }
}
