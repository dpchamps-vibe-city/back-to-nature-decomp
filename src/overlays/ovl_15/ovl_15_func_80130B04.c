#include "common.h"
#include "ovl15_save_menu_ui.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_80130B04(void) {
    s32 temp_a1;
    s8 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053596);
    temp_v0 = Ovl15UpdateSaveSlotMenu(0);
    if (temp_v0 < 3) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                gOvl15SelectedCard = 2;
                gOvl15MenuState = 0;
            }
        } else {
            if (gOvl15SaveSlotStatus[temp_v0] != 0) {
                gOvl15MenuState = 0xA;
            } else {
                gOvl15SelectedSlot = (s16) temp_v0;
                gOvl15MenuState = 0xB;
            }
        }
    }
}
