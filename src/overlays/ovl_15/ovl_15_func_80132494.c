#include "common.h"
#include "ovl15_save_menu_ui.h"

extern u8 D_800521F6[];
extern s8 D_80137584;
extern s16 D_8013759A;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);
void ovl_15_func_80134724(void);

void ovl_15_func_80132494(void) {
    s8 *p;

    p = &D_80137584;
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14,
                  gTextBankOffsets[0] + (s32)D_800521F6);
    *p = Ovl15WaitForMenuButton(0x17, 0x12);
    if (D_8013759A >= 0x5B) {
        *p = 0x12;
    }
    if (*p != 0x17) {
        ovl_15_func_80134724();
    }
}
