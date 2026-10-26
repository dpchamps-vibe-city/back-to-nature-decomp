#include "common.h"
#include "ovl15_save_menu_ui.h"
#include "ovl15_save_menu_lifecycle.h"

extern u8 D_800534DC[];
extern s8 gOvl15MenuState;
extern s16 gOvl15MenuStateTimer;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_8013010C(void) {
    s8 *p;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800534DC);
    *p = Ovl15WaitForMenuButton(0x18, 0);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    if (*p != 0x18) {
        Ovl15ResetSaveMenuState();
        gOvl15MenuExitRequest = 1;
    }
}
