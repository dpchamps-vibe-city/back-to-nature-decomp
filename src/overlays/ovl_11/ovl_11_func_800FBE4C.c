#include "common.h"
#include "game_types.h"
#include "screen_fade.h"

s32 ovl_11_func_800FBE4C(void) {
    if (D_8012720C == 0) {
        D_8012720C = 1;
        BeginScreenFadeIncrease(10, 0, 2);
    } else {
        return D_8012720C;
    }
}
