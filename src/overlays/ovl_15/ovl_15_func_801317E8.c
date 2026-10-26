#include "common.h"
#include "ovl15_save_menu_ui.h"

extern u8 D_800530C8[];
extern s8 gOvl15MenuState;
extern s16 gOvl15MenuStateTimer;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

s32 ovl_15_func_801317E8(void) {
    s8 *p;
    s32 ret;

    p = &gOvl15MenuState;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_800530C8);
    ret = Ovl15WaitForMenuButton(6, 0);
    *p = ret;
    if (gOvl15MenuStateTimer >= 0x5B) {
        *p = 0;
    }
    return ret;
}
