#include "common.h"
#include "ovl15_save_menu_ui.h"

extern u8 D_80053530[];
extern s8 gOvl15MenuState;
extern s8 gOvl15ConfirmationChoice;
extern s16 gOvl15SelectedCard;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8013063C(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053530);
    temp_v0 = Ovl15UpdateTwoChoiceMenu();
    if (temp_v0 < 2) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15ConfirmationChoice = 1;
                gOvl15MenuState = 0x11;
            }
        } else {
            gOvl15SelectedCard = temp_v0;
            gOvl15MenuState = 4;
        }
    }
}
