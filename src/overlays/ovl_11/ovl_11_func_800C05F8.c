#include "common.h"

/* Overlay alias of the matched Rand definition at 0x80012A34. */
u32 Rand(s32 arg0);
void ovl_11_func_800D7B24(void);
void ovl_11_func_800D0FB4(void);
void ovl_11_func_800F4CEC(void);
void ovl_11_func_800F4CAC(void);
void func_8001AF70(u16 arg0, u16 arg1);

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
        func_8001AF70(0x39, 0);
        func_8001AF70(0x3E, 0);
    }
}
