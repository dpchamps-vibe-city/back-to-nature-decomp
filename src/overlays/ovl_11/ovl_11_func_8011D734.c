#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

s16 ovl_11_func_8011D890(Ovl11D548Arg x, s32 mode);

s32 ovl_11_func_8011D734(Ovl11D548Arg *arg0) {
    s16 *dst;
    s32 var_s1;
    s32 temp_a0;
    s32 temp_v1;

    var_s1 = 0;
    dst = &arg0->index;
    temp_a0 = ((SomeStruct *) gPad0Input)->field_0x0;
    temp_v1 = ((SomeStruct *) gPad0Input)->field_0x8;
    if (temp_a0 & 0x1000) {
        *dst = ovl_11_func_8011D890(*arg0, 0);
        PlaySoundEffect(5);
    } else if (temp_a0 & 0x4000) {
        *dst = ovl_11_func_8011D890(*arg0, 1);
        PlaySoundEffect(5);
    } else {
        if (temp_v1 & 0x40) {
            var_s1 = 1;
            PlaySoundEffect(0);
        } else if (temp_v1 & 0x10) {
            if (gOvl11InteractionDrawMode != 7) {
                var_s1 = 2;
                PlaySoundEffect(0);
            }
        } else if (temp_v1 & 0x20) {
            var_s1 = 3;
            PlaySoundEffect(1);
        }
    }
    return var_s1;
}
