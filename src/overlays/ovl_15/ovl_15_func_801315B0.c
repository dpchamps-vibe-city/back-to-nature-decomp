#include "common.h"
#include "ovl15_save_menu_ui.h"

extern u8 D_80052FFA[];
extern s8 gOvl15MenuState;
extern s8 gOvl15ConfirmationChoice;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_801315B0(void) {
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052FFA);
    gOvl15MenuState = Ovl15WaitForMenuButton(2, 0x1F);
    gOvl15ConfirmationChoice = 1;
}
