#include "common.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_15_func_80137228(s16 arg0, s16 arg1);
void ovl_15_func_80135A78(void);
void ovl_15_func_80135380(void);
void ovl_15_func_8012E15C(void);

void ovl_15_func_80130FF0(void) {
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32)(D_800533C8 + gTextBankOffsets[0]));
    D_80137584 = ovl_15_func_80137228(0x13, 0);
    if (D_8013759A >= 0x5B) {
        D_80137584 = 0;
    }
    if (D_80137584 != 0x13) {
        ovl_15_func_80135A78();
        ovl_15_func_80135380();
        ovl_15_func_8012E15C();
        D_80137586 = 1;
    }
}
