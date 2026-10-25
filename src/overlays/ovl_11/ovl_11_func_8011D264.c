#include "common.h"
#include "sound_commands.h"

s32 ovl_11_func_8011D264(u16 *arg0) {
    s32 temp_v1;
    s32 var_s1;
    u16 var_a0;
    u16 var_v1;

    var_s1 = 0;
    temp_v1 = *(s32 *) ((u8 *) gPad0Input + 8);
    if (temp_v1 & 0x1000) {
        --*arg0;
        PlaySoundEffect(5);
    } else if (temp_v1 & 0x4000) {
        ++*arg0;
        PlaySoundEffect(5);
    } else if (temp_v1 & 0x40) {
        var_s1 = 1;
        PlaySoundEffect((s16) *arg0 == 1);
    } else if (temp_v1 & 0x20) {
        PlaySoundEffect(1);
        *arg0 = 1;
        var_s1 = 1;
    }
    var_v1 = *arg0;
    if ((s16) *arg0 < 0) {
        var_v1 = 1;
    }
    *arg0 = var_v1;
    var_a0 = 0;
    if ((s16) var_v1 < 2) {
        var_a0 = var_v1;
    }
    *arg0 = var_a0;
    return var_s1;
}
