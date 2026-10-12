#include "common.h"

void ovl_11_func_800C009C(u8 *arg0);

void ovl_11_func_800C02B0(void) {
    s32 var_s2;
    u32 var_s1;
    u8 *var_s0;

    var_s2 = 0;
    var_s0 = Ovl11_D_80128820;
    var_s1 = 0;
    do {
        if ((*(s16 *) ((u8 *) var_s0 + 4)) != 0) {
            ovl_11_func_800C009C(var_s0);
        }
        if ((*(s16 *) ((u8 *) var_s0 + 8)) == 0) {
            if (var_s2 == 0) {
                var_s2 = 1;
            } else if ((*(u16 *) ((u8 *) var_s0 + 0xC)) & 0x4000) {
                *(s16 *)(var_s0 + 8) += 3;
            }
        }
        var_s1 += 1;
        var_s0 += 0xE;
    } while (var_s1 < 0x28U);
}
