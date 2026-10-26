#include "common.h"
#include "ovl15_save_menu_ui.h"

void ovl_15_func_801335F8(void);
void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void ovl_15_func_80134724(void);

void ovl_15_func_80132638(void) {
    s32 temp_a1;
    s8 temp_v0;

    ovl_15_func_801335F8();
    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)(D_80051EC6 + gTextBankOffsets[0]));
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {
    case 1:
    case -1:
        ovl_15_func_80134724();
        gOvl15MenuState = 0x13;
        return;
    case 0:
        gOvl15ConfirmationChoice = 1;
        gOvl15MenuState = 0x1B;
        return;
    }
}
