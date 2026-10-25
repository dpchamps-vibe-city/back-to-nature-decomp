#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80024810 (s32 arg0, s16 arg1, s16 arg2);
void func_80024860 (s32 arg0, s16 arg1, s16 arg2);
void func_80017A38 (s16 arg0, s16 arg1);
s32 ovl_11_func_80100A34 (u16 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);


void ovl_11_func_80100898(u8 arg0) {
    s32 temp_s0;
    s32 temp_t0;
    s32 base;
    s32 var_s0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s5;
    u16 temp_a0;

    temp_t0 = gActiveRenderContext->field_D8;
    temp_s0 = temp_t0 + 0x60;
    base = temp_t0 + 0x70;
    func_80022580((u32 *)(temp_t0 + 0x74), 1, 0xE, 0x10, 0x124, 0x94);
    func_80024810(temp_s0, 6, 0x5A);
    func_80024860(temp_s0, 0x12A, 0x5A);
    func_80017A38(0, 2);
    var_s5 = 0x1C;
    for (var_s2 = 0; var_s2 < 2; var_s2++) {
        var_s1 = 0x15;
        for (var_s0 = 0; var_s0 < 0xA; var_s0++) {
            temp_a0 = *((u16 *) ((u8 *) &D_80071318[0] + (((var_s2 * 0xA) + ((arg0 & 0xFF) * 0x14) + var_s0) * 0x16)));
            if (temp_a0 == 0) {
                break;
            }
            func_80017B3C(base, (s32) ((void *) ((u8 *) D_8005175C + (*D_80054BBC + ovl_11_func_80100A34(temp_a0)))), var_s5, var_s1);
            var_s1 = (s32) (s16) (var_s1 + 0xE);
        }
        var_s5 = (s32) (s16) (var_s5 + 0x90);
    }
    func_80017A38(1, 2);
}
