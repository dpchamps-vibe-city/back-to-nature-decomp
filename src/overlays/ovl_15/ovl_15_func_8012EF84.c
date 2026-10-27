#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"
#include "psyq/strings.h"
#include "ovl15_card_service.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012EF84(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        switch (Ovl15ProbeCard(gOvl15SelectedCard)) {
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
