#include "common.h"
#include "game_types.h"
#include "sound_commands.h"

void ovl_11_func_800FAAAC(void);

s32 ovl_11_func_800FAAD4(void) {
    if (D_80126FE0 == 0) {
        ovl_11_func_800FAAAC();
        D_80126FE0 = 1;
        PlaySoundEffect(3);
    } else {
        return D_80126FE0;
    }
}
