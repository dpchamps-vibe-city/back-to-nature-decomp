#include "common.h"
#include "screen_fade.h"

s32 func_8001FABC(s16 arg0);

void ovl_11_func_800FC320(void) {
    func_8001FABC(3);
    BeginScreenFadeIncrease(10, 0, 2);
    D_8012720C = 5;
}
