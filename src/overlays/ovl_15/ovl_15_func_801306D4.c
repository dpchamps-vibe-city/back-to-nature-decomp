#include "common.h"
#include "game_types.h"

s32 ovl_15_func_80135C20(s32 arg0);

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_801306D4(void) {
    s16 temp_v0;
    s32 temp_a1;

    temp_a1 = D_8005E3C0->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*D_80054BBC]);
    if (D_8013759A >= 2) {
        temp_v0 = ovl_15_func_80135C20(D_8013758C) - 4;
        switch (temp_v0) {
        case 0:
            if (D_80137594[D_8013758C] == 0) {
                D_80137584 = 0x10;
            } else {
                D_80137584 = 5;
            }
            break;
        case 1:
            D_80137584 = 4;
            break;
        case 4:
            D_80137584 = 6;
            break;
        case 2:
        case 3:
            D_80137584 = 8;
            break;
        default:
            D_80137584 = 7;
            break;
        }
    }
}
