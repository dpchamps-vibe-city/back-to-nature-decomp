#include "common.h"
#include "game_types.h"
#include "ovl15_save_menu_ui.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);

void ovl_15_func_8012F3FC(void) {
    s32 var_s0;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80051FEA);
    for (var_s0 = 0; var_s0 < 3; var_s0++) {
        Ovl15DrawSaveSlotSummary((s16)var_s0, 1);
    }
    func_800248B0(gActiveRenderContext->field_D8 + 4, 0x39, (gOvl15MenuCursor * 0x31) + 0x18);
    switch (Ovl15UpdateConfirmationMenu()) {
    case 1:
    case -1:
        gOvl15MenuCursor = 0;
        D_80137584 = 8;
        break;
    case 0:
        D_80137584 = 0x13;
        break;
    }
}
