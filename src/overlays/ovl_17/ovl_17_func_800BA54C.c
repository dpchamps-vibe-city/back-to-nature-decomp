#include "common.h"
#include "ovl17_timer_hud.h"

void ovl_17_func_800BA630(void);
void ovl_17_func_800BA698(void);
void ovl_17_func_800BA704();

void ovl_17_func_800BA54C(void) {
    s32 i;

    Ovl17_D_800BD848.field_0 = 0;
    Ovl17_D_800BD848.field_4 = 0;
    Ovl17_D_800BD848.field_6 = 6;
    Ovl17_D_800BD848.field_8 = 0x80;
    Ovl17_D_800BD848.field_A = 0;
    Ovl17_D_800BD848.field_C = 4;
    Ovl17_D_800BD848.field_E = 0x96;
    Ovl17_D_800BD848.field_10 = 1;
    Ovl17_D_800BD848.field_12 = 0x28;
    Ovl17_D_800BD848.field_14 = 0x12;
    Ovl17_D_800BD848.field_16 = 0xAA;
    Ovl17_D_800BD848.field_18 = 0x64;
    Ovl17_D_800BD848.field_1C = 0;
    Ovl17_D_800BD848.field_20 = 0;
    Ovl17_D_800BD848.field_22 = 0;
    Ovl17_D_800BD848.field_24 = 0x40000;
    for (i = 0; i < 6; i++) {
        Ovl17_D_800BD848.entries[i] = -1;
    }
    Ovl17_D_800BD848.field_2C8 = -1;
    Ovl17_D_800BD848.field_2D0 = 0x32;
    Ovl17_D_800BD848.field_2D2 = 0x400;
    Ovl17_D_800BD848.field_2D4 = 0;
    ovl_17_func_800BA630();
    ovl_17_func_800BA698();
    ovl_17_func_800BA704();
    Ovl17UpdateTimerHud(-1);
}
