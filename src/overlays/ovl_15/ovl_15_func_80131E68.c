#include "common.h"
#include "ovl15_save_menu_ui.h"

extern u8 D_80052F4E[];
extern s8 D_80137584;
extern s16 D_8013759A;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

s32 ovl_15_func_80131E68(void) {
    s8 *p;
    s32 ret;

    p = &D_80137584;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80052F4E);
    ret = Ovl15WaitForMenuButton(0x1E, 0);
    *p = ret;
    if (D_8013759A >= 0x5B) {
        *p = 0;
    }
    return ret;
}
