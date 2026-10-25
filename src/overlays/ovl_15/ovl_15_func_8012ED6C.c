#include "common.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s16 ovl_15_func_80135C20(s16 arg0);

void ovl_15_func_8012ED6C(void) {
    register s16 *states;
    s32 i;
    s32 status;
    s32 text;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *)(text + 0x18), text + 0x14,
                 (s32) &D_80052FC4[gTextBankOffsets[0]]);
    if (D_8013759A >= 2) {
        for (i = 0; i < 2; i++) {
            states = &D_80137594[i];
            status = ovl_15_func_80135C20(i);
            if (status == 1) {
                status = 0;
            }
            if (status == 7) {
                status = 6;
            }
            if (status != *states) {
                *states = status;
            }
        }
        states = D_80137594;
        if (D_80137594[0] == 4 && states[1] == 4) {
            D_80137584 = 2;
        } else {
            D_80137584 = 3;
        }
    }
}
