#include "common.h"
#include "ovl15_save_menu_ui.h"

extern u8 D_80051DCC[];
extern s8 gOvl15MenuState;
extern s8 gOvl15MenuCursor;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_80130048(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80051DCC);
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 0:
        gOvl15MenuCursor = 0;
        gOvl15MenuState = 0x17;
        return;
    case 1:
    case -1:
        gOvl15MenuState = 0;
        return;
    }
}
