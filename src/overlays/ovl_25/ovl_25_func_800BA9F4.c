#include "common.h"
#include "screen_fade.h"

extern s16 D_800BFE46;

void ovl_25_func_800B83A0(void);

void ovl_25_func_800BA9F4(void) {
    char *far_base;

    if (IsScreenFadeComplete() == 1) {
        ovl_25_func_800B83A0();
        BeginScreenFadeDecrease(0xA, 0, 2);
        far_base = (char *)&D_8007AFF0;
        *(s16 *)(far_base + 0x253B4) = -0x140;
        D_800BFE46 = 6;
    }
}
