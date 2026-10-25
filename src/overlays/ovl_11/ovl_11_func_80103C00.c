#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "sound_driver.h"
#include "sound_commands.h"


s32 func_800226B0 (void);
void ovl_11_func_80104418 (s16 arg0, s16 *arg1);
s32 ovl_11_func_80104394 (void);

s32 ovl_11_func_80103C00(void) {
    s16 *temp_s1;
    s16 var_s0;
    s32 temp_a1;
    s32 temp_a2;

    if (func_800226B0() == 0) {
        return 0;
    }
    var_s0 = 0;
    temp_s1 = &D_8012CF10[D_8012742C];
    temp_a2 = *(s32 *) ((u8 *) gPad0Input + 0);
    if (temp_a2 & 0x1000) {
        var_s0 = 0xA;
    } else if (temp_a2 & 0x4000) {
        var_s0 = -0xA;
    } else if (temp_a2 & 0x2000) {
        var_s0 = 1;
    } else if (temp_a2 & 0x8000) {
        var_s0 = -1;
    }
    if (var_s0 != 0) {
        PlaySoundEffect(5);
        ovl_11_func_80104418(var_s0, temp_s1);
    }
    temp_a1 = *(s32 *) ((u8 *) gPad0Input + 8);
    if (temp_a1 & 0x800) {
        if (ovl_11_func_80104394() == 1) {
            PlaySoundEffect(1);
            return -2;
        }
        PlaySoundEffect(0);
        return 2;
    }
    if (temp_a1 & 0x20) {
        PlaySoundEffect(1);
        return -1;
    }
    if (temp_a1 & 0x40) {
        PlaySoundEffect(0);
        return 1;
    }
    return 0;
}
