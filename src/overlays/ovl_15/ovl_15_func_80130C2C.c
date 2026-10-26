#include "common.h"
#include "psyq/strings.h"
#include "ovl15_card_filename.h"

extern u8 D_80053350[];
extern s8 gOvl15MenuState;
extern s16 gOvl15SelectedSlot;
extern u8 D_80140EC0[];

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_80130C2C(void) {
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    Ovl15BuildCardFilename((s32)D_80140EC0, gOvl15SelectedSlot);
    gOvl15MenuState = 12;
}
