#include "common.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s16 ovl_15_func_80135C20(s16 arg0);

void ovl_15_func_801316B0(void) {
    s32 text;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (text + 0x18), text + 0x14, (s32) &D_80052FC4[D_80054BBC[0]]);
    if (D_8013759A >= 2) {
        switch (ovl_15_func_80135C20(D_8013758C)) {
        case 4:
            if (D_80137594[D_8013758C] == 0) {
                D_80137584 = 0x1E;
            } else {
                D_80137584 = 5;
            }
            break;
        case 8:
            D_80137584 = 6;
            break;
        default:
            D_80137584 = 7;
            break;
        }
    }
}
