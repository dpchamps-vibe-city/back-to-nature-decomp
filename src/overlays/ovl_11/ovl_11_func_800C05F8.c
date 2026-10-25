#include "common.h"
#include "random.h"
#include "game_flags.h"

void ovl_11_func_800D7B24(void);
void ovl_11_func_800D0FB4(void);
void ovl_11_func_800F4CEC(void);
void ovl_11_func_800F4CAC(void);

void ovl_11_func_800C05F8(void) {
    char *state;

    state = (char *)D_8006C838;
    if (*(u16 *)(state + 0x44D4) != 0 && Rand(100) < 31) {
        *(u16 *)(state + 0x44D4) = 0;
        ovl_11_func_800D7B24();
        ovl_11_func_800D0FB4();
        ovl_11_func_800F4CEC();
        ovl_11_func_800F4CAC();
        *(s32 *)(state + 0x44F8) &= ~0x2000;
        SetGameFlag(0x39, 0);
        SetGameFlag(0x3E, 0);
    }
}
