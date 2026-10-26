#include "common.h"
#include "game_types.h"

s32 ovl_15_func_80135C20(s32 arg0);

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_801306D4(void) {
    s16 temp_v0;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        temp_v0 = ovl_15_func_80135C20(gOvl15SelectedCard) - 4;
        switch (temp_v0) {
        case 0:
            if (gOvl15CardStates[gOvl15SelectedCard] == 0) {
                gOvl15MenuState = 0x10;
            } else {
                gOvl15MenuState = 5;
            }
            break;
        case 1:
            gOvl15MenuState = 4;
            break;
        case 4:
            gOvl15MenuState = 6;
            break;
        case 2:
        case 3:
            gOvl15MenuState = 8;
            break;
        default:
            gOvl15MenuState = 7;
            break;
        }
    }
}
