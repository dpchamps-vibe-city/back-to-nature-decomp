#include "common.h"
#include "ovl15_save_menu_ui.h"

extern u8 D_80052150[];
extern s8 gOvl15MenuState;
extern s16 gOvl15MenuStateTimer;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);
void ovl_15_func_80134724(void);

void ovl_15_func_8013237C(void) {
    s8 *p;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052150);
    *p = Ovl15WaitForMenuButton(0x15, 0x12);
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x15) {
        ovl_15_func_80134724();
    }
}
