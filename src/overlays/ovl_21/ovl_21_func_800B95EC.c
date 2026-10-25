#include "common.h"
#include "screen_fade.h"

void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B95EC(void) {
    ovl_21_func_800BA4C0();
    if (IsScreenFadeComplete() == 1) {
        BeginScreenFadeDecrease(10, 0, 2);
        D_800C0448[0] = 0;
        D_800C0448[1] += 1;
    }
}
