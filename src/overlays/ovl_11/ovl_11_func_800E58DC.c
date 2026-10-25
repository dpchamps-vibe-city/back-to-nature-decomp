#include "common.h"
#include "cd_audio.h"
#include "message_window.h"

s32 ovl_11_func_800E58DC(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_s0;
    u32 var_a1;
    s32 var_v0;
    char *base;

    var_v0 = 0;
    if (arg3 != 0) {
        var_s0 = D_80129560[arg1];
    } else {
        var_s0 = arg1;
    }
    base = (char *)&D_8006C838;
    base += 0x8000;
    if ((*(s16 *)(base + 0x64C8) == 2) && (arg0 == 0)) {
        for (var_a1 = 0; var_a1 < 9; var_a1++) {
            if (var_s0 == D_801248F4[var_a1]) {
                var_s0 = var_a1 + 0x731;
                break;
            }
        }
    }
    if (GetCdAudioState() == 0) {
        RequestMessageWindow(arg0, var_s0);
        if (GetMessageWindowStatus() == 2) {
            var_v0 = 1;
            if (arg2 != -1) {
                D_80129560[arg2] = GetMessageWindowChoice();
            }
        }
    }
    return var_v0;
}
