#include "common.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s16 ovl_15_func_80135C20(s16 arg0);

void ovl_15_func_8012EF84(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        switch (ovl_15_func_80135C20(gOvl15SelectedCard)) {
        case 4:
            if (gOvl15CardStates[gOvl15SelectedCard] == 0) {
                gOvl15MenuState = 0x15;
            } else {
                gOvl15MenuState = 5;
            }
            break;
        case 5:
            gOvl15MenuState = 4;
            break;
        case 8:
            gOvl15MenuState = 6;
            break;
        default:
            gOvl15MenuState = 7;
            break;
        }
    }
}
