#include "common.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s16 ovl_15_func_80135C20(s16 arg0);
s32 ovl_15_func_80135FF0(s16 arg0, s16 arg1);

void ovl_15_func_8012F4F4(void) {
    s32 temp_a1;
    s16 status;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32) &D_80052FC4[gTextBankOffsets[0]]);
    if (gOvl15MenuStateTimer >= 2) {
        status = ovl_15_func_80135C20(gOvl15SelectedCard);
        if (status == 1) {
            status = 0;
        }
        if (status == 7) {
            status = 6;
        }
        switch (status) {
        case 0:
            switch (ovl_15_func_80135FF0(gOvl15SelectedCard, gOvl15SelectedSlot)) {
            case 0:
                gOvl15MenuState = 0xF;
                return;
            case 4:
                switch (gOvl15CardStates[gOvl15SelectedCard]) {
                case 0:
                    gOvl15MenuState = 0x15;
                    break;
                default:
                    gOvl15MenuState = 5;
                    break;
                }
                return;
            case 11:
                gOvl15MenuState = 0x11;
                return;
            case 15:
                gOvl15MenuState = 0x12;
                return;
            case 8:
                gOvl15MenuState = 6;
                return;
            }
            break;
        case 5:
            gOvl15MenuState = 0;
            break;
        case 4:
            if (gOvl15CardStates[gOvl15SelectedCard] == 0) {
                gOvl15MenuState = 0x15;
            } else {
                gOvl15MenuState = 5;
            }
            break;
        case 8:
            gOvl15MenuState = 6;
            break;
        case 6:
            gOvl15ConfirmationChoice = 1;
            D_8013759C = 0;
            gOvl15MenuState = 0xB;
            break;
        }
    }
}
