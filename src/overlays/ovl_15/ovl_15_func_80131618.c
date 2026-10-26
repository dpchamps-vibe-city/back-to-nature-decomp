#include "common.h"
#include "ovl15_save_menu_ui.h"

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_80131618(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053530);
    temp_v0 = Ovl15UpdateTwoChoiceMenu();
    if (temp_v0 < 2) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15ConfirmationChoice = 1;
                D_80137584 = 0x1F;
            }
        } else {
            D_8013758C = temp_v0;
            D_80137584 = 4;
        }
    }
}
