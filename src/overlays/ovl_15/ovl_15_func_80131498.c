#include "common.h"

s16 ovl_15_func_80135C20(s16 arg0);

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);

void ovl_15_func_80131498(void) {
    s16 *var_s0;
    s32 var_s1;
    s16 var_v1;
    s32 temp_a1;

    temp_a1 = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (temp_a1 + 0x18), temp_a1 + 0x14, (s32) &D_80052FC4[*gTextBankOffsets]);
    if (gOvl15MenuStateTimer >= 2) {
        for (var_s1 = 0; var_s1 < 2; var_s1++) {
            var_s0 = &gOvl15CardStates[var_s1];
            var_v1 = ovl_15_func_80135C20(var_s1);
            if (var_v1 == 1) {
                var_v1 = 0;
            }
            if (var_v1 == 7) {
                var_v1 = 6;
            }
            if (var_v1 != *var_s0) {
                *var_s0 = var_v1;
            }
        }
        if (gOvl15CardStates[0] == 4 && gOvl15CardStates[1] == gOvl15CardStates[0]) {
            gOvl15MenuState = 2;
        } else {
            gOvl15MenuState = 3;
        }
    }
}
