#include "common.h"

extern u8 D_8005346A[];
extern s16 D_8013758E;

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_15_func_80136660(s16 arg0, s16 arg1, u32 *arg2);

void ovl_15_func_8012FF28(void) {
    s32 temp_a1;
    u32 temp_v0;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, gTextBankOffsets[0] + (s32) D_8005346A);
    temp_v0 = ovl_15_func_80136660(D_8013758C, D_8013758E, (u32 *) gOvl15TransferBuffer);
    switch (temp_v0) {
    case 1:
    case 3:
        break;
    case 2:
    case 5:
        D_80137584 = 0x10;
        break;
    case 4:
        D_80137584 = 0x15;
        break;
    case 0:
        D_80137584 = 0x18;
        break;
    }
}
