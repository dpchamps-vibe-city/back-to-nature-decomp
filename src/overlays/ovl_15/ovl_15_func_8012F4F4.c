#include "common.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s16 ovl_15_func_80135C20(s16 arg0);
s32 ovl_15_func_80135FF0(s16 arg0, s16 arg1);

void ovl_15_func_8012F4F4(void) {
    s32 temp_a1;
    s16 status;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32) &D_80052FC4[D_80054BBC[0]]);
    if (D_8013759A >= 2) {
        status = ovl_15_func_80135C20(D_8013758C);
        if (status == 1) {
            status = 0;
        }
        if (status == 7) {
            status = 6;
        }
        switch (status) {
        case 0:
            switch (ovl_15_func_80135FF0(D_8013758C, D_8013758E)) {
            case 0:
                D_80137584 = 0xF;
                return;
            case 4:
                switch (D_80137594[D_8013758C]) {
                case 0:
                    D_80137584 = 0x15;
                    break;
                default:
                    D_80137584 = 5;
                    break;
                }
                return;
            case 11:
                D_80137584 = 0x11;
                return;
            case 15:
                D_80137584 = 0x12;
                return;
            case 8:
                D_80137584 = 6;
                return;
            }
            break;
        case 5:
            D_80137584 = 0;
            break;
        case 4:
            if (D_80137594[D_8013758C] == 0) {
                D_80137584 = 0x15;
            } else {
                D_80137584 = 5;
            }
            break;
        case 8:
            D_80137584 = 6;
            break;
        case 6:
            D_80137588 = 1;
            D_8013759C = 0;
            D_80137584 = 0xB;
            break;
        }
    }
}
