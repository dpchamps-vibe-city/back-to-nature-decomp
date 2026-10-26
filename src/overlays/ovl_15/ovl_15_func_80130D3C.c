#include "common.h"
#include "ovl15_save_checksums.h"

extern u8 D_80053350[];
extern s8 gOvl15MenuState;
extern s16 gOvl15SelectedSlot;

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);

void ovl_15_func_80130D3C(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((void *)(temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    if (Ovl15VerifySaveChecksums() == -1) {
        gOvl15MenuState = 0xE;
    }
    else if (gOvl15SaveSlotHeaders[gOvl15SelectedSlot].field_34 != 0x14) {
        gOvl15MenuState = 0xE;
    }
    else {
        gOvl15MenuState = 0x13;
    }
}
