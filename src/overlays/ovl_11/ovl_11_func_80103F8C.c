#include "common.h"

void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
s16 *func_8001A970 (s32 arg0, s16 *arg1, s32 arg2);
void func_80017A08 (s32 arg0, s32 arg1);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_80104148 (s32 arg0);

void ovl_11_func_80103F8C(void) {
    s32 var_s2;
    s32 temp_s3;
    s32 temp_t0;
    u16 var_a1;

    temp_t0 = D_8005E3C0->field_D8;
    temp_s3 = temp_t0 + 0x58;
    func_80022580((u32 *)(temp_t0 + 0x5C), 0, 0x20, 0x30, 0x101, 0x72);
    var_a1 = 2;
    if (((Ovl11CardView *) D_8006C838)->selector != 3) {
        var_a1 = (u16) ((Ovl11CardView *) D_8006C838)->selector;
    }
    *(u16 *)func_8001A970(var_a1 + 1, D_8012CF30, 1) = 0xFFFF;
    func_80017A08(0, (s32) D_8012CF30);
    func_80017B3C(temp_s3, (s32)(D_80051C0E + D_80054BBC[0]), 0x22, 0x32);
    func_80017B3C(temp_s3, (s32)(D_80051C0E + 0x18 + D_80054BBC[0]), 0xDF, 0x94);
    var_s2 = 0;
    do {
        ovl_11_func_80104148((s16) var_s2);
        var_s2 += 1;
    } while (var_s2 < 6);
}
