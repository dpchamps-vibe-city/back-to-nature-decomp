#include "common.h"
#include "ovl15_save_menu_ui.h"
#include "psyq/memory.h"
#include "ovl15_save_image.h"
#include "ovl15_save_menu_lifecycle.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_80130FF0(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32)(D_800533C8 + gTextBankOffsets[0]));
    gOvl15MenuState = Ovl15WaitForMenuButton(0x13, 0);
    if (gOvl15MenuStateTimer >= 0x5B) {
        gOvl15MenuState = 0;
    }
    if (gOvl15MenuState != 0x13) {
        Ovl15RemoveSaveChecksumSlots();
        Ovl15RestoreSaveImage();
        Ovl15ResetSaveMenuState();
        gOvl15MenuExitRequest = 1;
    }
}
