#include "common.h"
#include "ovl15_save_checksums.h"
#include "ovl15_save_menu_ui.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
void ovl_15_func_80134724(void);
void ovl_15_func_80132C1C(void);
void ovl_15_func_801359FC(void);

void ovl_15_func_801326E8(void) {
    s32 var_s0;
    s32 temp_a1;
    s8 temp_v0;

    var_s0 = 0;
    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)&D_80051FEA[gTextBankOffsets[0]]);
    do {
        Ovl15DrawSaveSlotSummary(var_s0, 0);
        var_s0 += 1;
    } while (var_s0 < 3);
    func_800248B0(gActiveRenderContext->field_D8 + 4, 0x39, (gOvl15MenuCursor * 0x31) + 0x18);
    temp_v0 = Ovl15UpdateConfirmationMenu();
    switch (temp_v0) {                              /* irregular */
    case 1:
    case -1:
        ovl_15_func_80134724();
        gOvl15ConfirmationChoice = 1;
        D_80137584 = 0x1A;
        return;
    case 0:
        ovl_15_func_80132C1C();
        ovl_15_func_801359FC();
        Ovl15WriteSaveChecksums();
        D_80137584 = 0x1D;
        return;
    }
}
