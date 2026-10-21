#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80022580 (u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
u32 func_80017A64 (void);
u16 *func_8001AAB8 (s32 arg0, u16 *arg1);
void func_80017A48 (u32 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_80011F5C (s32 arg0);
void func_80011FD8 (s32 arg0);
void func_800136D4 (u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);

void ovl_11_func_800FA9B8(u32 *arg0, s32 arg1) {
    s32 var_s0;
    s16 var_s3;
    u32 temp_s2;
    u32 var_a0;

    func_80022580(arg0, 1, 0x10, 0x30, 0x11F, 0x74);
    var_s0 = 0;
    temp_s2 = func_80017A64();
    var_s3 = 0x16;
    do {
        *func_8001AAB8(var_s0, (u16 *) &D_80129FF0) = 0xFFFF;
        if (var_s0 != 0) {
            var_a0 = 5;
            if (var_s0 < 6) {
                var_a0 = temp_s2;
            }
        } else {
            var_a0 = 4;
        }
        var_s0 += 1;
        func_80017A48(var_a0);
        func_80017B3C(arg1, (s32) &D_80129FF0, var_s3, 0x42);
        var_s3 += 0x29;
    } while (var_s0 < 7);
    func_80017A48(temp_s2);
}
