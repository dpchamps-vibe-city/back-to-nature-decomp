#include "common.h"
#include "ovl15_save_menu_ui.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012F300(void) {
    s16 temp_v1;
    s32 temp_a1;
    s32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32) D_80053566);
    temp_v0 = Ovl15UpdateSaveSlotMenu(1);
    if (temp_v0 < 3) {
        if (temp_v0 < 0) {
            if (temp_v0 == -1) {
                D_8013758C = 2;
                D_80137584 = 0;
            }
        } else {
            temp_v1 = gOvl15SaveSlotStatus[(s16) temp_v0];
            D_8013758E = (s16) temp_v0;
            if (temp_v1 == 0) {
                gOvl15ConfirmationChoice = 1;
                D_80137584 = 9;
            } else if (temp_v1 == 1) {
                D_80137584 = 0xA;
            } else if (temp_v1 == 2) {
                D_80137584 = 0x11;
            } else if (temp_v1 == 3) {
                D_80137584 = 0x12;
            }
        }
    }
}
