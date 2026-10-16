#include "common.h"
#include "game_types.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void ovl_15_func_80136C70(s16 slot, s16 mode);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
s8 ovl_15_func_801370B4(void);

void ovl_15_func_8012F3FC(void) {
    s32 var_s0;
    s32 temp_a1;

    temp_a1 = D_8005E3C0->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14, D_80054BBC[0] + (s32)D_80051FEA);
    for (var_s0 = 0; var_s0 < 3; var_s0++) {
        ovl_15_func_80136C70((s16)var_s0, 1);
    }
    func_800248B0(D_8005E3C0->field_D8 + 4, 0x39, (D_80137587 * 0x31) + 0x18);
    switch (ovl_15_func_801370B4()) {
    case 1:
    case -1:
        D_80137587 = 0;
        D_80137584 = 8;
        break;
    case 0:
        D_80137584 = 0x13;
        break;
    }
}
