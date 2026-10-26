#include "common.h"

extern s32 gOvl11PrimaryHeldButtons;
extern s32 gOvl11PrimaryPressedButtons;
extern u16 D_801232B4[];

void ovl_11_func_800CD6F4(void *arg0) {
    if (*(u32 *)((u8 *)arg0 + 0x6C) & 8) {
        gOvl11PrimaryPressedButtons = 0;
        gOvl11PrimaryHeldButtons = D_801232B4[*(s16 *)((u8 *)arg0 + 0x30)];
    }
}
