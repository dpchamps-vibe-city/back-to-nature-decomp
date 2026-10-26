#include "common.h"
#include "ovl15_save_menu_lifecycle.h"

extern u8 D_80052FC4[];
extern s8 gOvl15MenuState;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);
void ovl_15_func_8013468C(void);

void ovl_15_func_8013143C(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14,
                  gTextBankOffsets[0] + (s32)D_80052FC4);
    Ovl15ResetSaveMenuState();
    gOvl15MenuState = 1;
    ovl_15_func_8013468C();
}
