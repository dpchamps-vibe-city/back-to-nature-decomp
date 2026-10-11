#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D5810(s16 arg0);
void ovl_11_func_800C9DC0(void);
s32 ovl_11_func_800C9E3C(s32 arg0);
s32 ovl_11_func_800C7270(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800D7348(u16 *dst, u16 *src);
s32 func_8001FABC(s16 arg0);

void ovl_11_func_800C8D9C(Recon_ovl_11_func_800C7270_A0View *arg0) {
    u8 *var_v1;
    s32 var_a2;
    u16 temp_v0;
    u32 var_a1;
    u16 *var_s1;

    var_a2 = 1;
    var_a1 = 0;
    var_v1 = (u8 *)arg0 + 0x90;
    do {
        if (*(s16 *)var_v1 != 0) {
            var_a2 = 0;
        }
        var_a1 += 1;
        var_v1 += 6;
    } while (var_a1 < 8U);
    temp_v0 = arg0->unk84;
    if ((var_a2 == 0) || (temp_v0 != 0)) {
        var_s1 = &arg0->unk84;
        if (((s16)arg0->unk84 == 0) || (ovl_11_func_800D5810((s16)arg0->unk84) == 1)) {
            arg0->unk6C &= 0xE7FFFFFF;
            ovl_11_func_800C9DC0();
            if ((ovl_11_func_800C9E3C(0) != -1) && ((s16)arg0->unk84 != 0)) {
                ovl_11_func_800C7270(arg0, 0x15, 0x47, 0xA);
                ovl_11_func_800D7348(&arg0->unkF0, var_s1);
                arg0->unk6C |= 0x8000;
                func_8001FABC(7);
            }
        }
    }
}
