#include "common.h"
#include "random.h"

void ovl_21_func_800BA4C0(void);

void ovl_21_func_800B96B0(void) {
    u8 *state;
    s32 *words;
    s16 selector;

    ovl_21_func_800BA4C0();
    state = (u8 *)D_800C0448;
    if (*(s16 *)(state + 2) == 0) {
        selector = Rand(4);
        if (*(s16 *)(state + 0x640) == selector) {
            selector++;
            selector %= 4;
        }
        ((Ovl21ResultView *)D_8006C838)->selection = D_800BCC14[selector];
    } else {
        ((Ovl21ResultView *)D_8006C838)->selection = D_800BCC14[*(s16 *)(state + 0x642)];
    }
    words = (s32 *)D_8006C838;
    words[0xE62C / 4] = 0;
    words[0x448C / 4]++;
}
