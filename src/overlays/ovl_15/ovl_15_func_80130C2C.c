#include "common.h"

extern u8 D_80053350[];
extern s8 D_80137584;
extern s16 D_8013758E;
extern u8 D_80140EC0[];

void func_8001AC10(void *arg0, s32 arg1, s32 arg2);
void ovl_15_func_80137544(char *arg0, s32 arg1);

void ovl_15_func_80130C2C(void) {
    func_8001AC10((void *)(gActiveRenderContext->field_D8 + 0x18), gActiveRenderContext->field_D8 + 0x14, gTextBankOffsets[0] + (s32)D_80053350);
    ovl_15_func_80137544((s32)D_80140EC0, D_8013758E);
    D_80137584 = 12;
}
