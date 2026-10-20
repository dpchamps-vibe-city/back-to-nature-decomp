#include "common.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
void ovl_15_func_80136C70(s16 slot, s16 mode);
void func_800248B0(s32 arg0, s16 arg1, s16 arg2);
s8 ovl_15_func_801370B4(void);
void ovl_15_func_80134724(void);
void ovl_15_func_80132C1C(void);
void ovl_15_func_801359FC(void);
void ovl_15_func_80135AE0(void);

void ovl_15_func_801326E8(void) {
    s32 var_s0;
    s32 temp_a1;
    s8 temp_v0;

    var_s0 = 0;
    temp_a1 = D_8005E3C0->field_D8;
    func_8001AC10((u32 *)(temp_a1 + 0x18), temp_a1 + 0x14,
                 (s32)&D_80051FEA[D_80054BBC[0]]);
    do {
        ovl_15_func_80136C70(var_s0, 0);
        var_s0 += 1;
    } while (var_s0 < 3);
    func_800248B0(D_8005E3C0->field_D8 + 4, 0x39, (D_80137587 * 0x31) + 0x18);
    temp_v0 = ovl_15_func_801370B4();
    switch (temp_v0) {                              /* irregular */
    case 1:
    case -1:
        ovl_15_func_80134724();
        D_80137588 = 1;
        D_80137584 = 0x1A;
        return;
    case 0:
        ovl_15_func_80132C1C();
        ovl_15_func_801359FC();
        ovl_15_func_80135AE0();
        D_80137584 = 0x1D;
        return;
    }
}
