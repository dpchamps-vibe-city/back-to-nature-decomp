#include "common.h"
#include "screen_fade.h"
#include "sound_commands.h"

void ovl_11_func_800FC320(void) {
    PlaySoundEffect(3);
    BeginScreenFadeIncrease(10, 0, 2);
    D_8012720C = 5;
}
