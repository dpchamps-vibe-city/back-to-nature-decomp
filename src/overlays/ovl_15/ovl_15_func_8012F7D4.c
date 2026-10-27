#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"
#include "psyq/strings.h"
#include "ovl15_card_service.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8012F7D4(void) {
    s32 temp_a1;
    s32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32)(D_8005316C + gTextBankOffsets[0]));
    if (gOvl15MenuStateTimer >= 2) {
        temp_v0 = Ovl15FormatCard(gOvl15SelectedCard);
        if (temp_v0 == 0) {
            gOvl15MenuState = 0xF;
        } else if (temp_v0 == 4) {
            gOvl15MenuState = 0x15;
        } else {
            gOvl15MenuState = 0xE;
        }
    }
}
