#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800EBF18", ovl_11_func_800EBF18);


/* PARKED by /auto_decompilation_loop on 2026-10-11T06:24:25.050Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800EBF18.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 ovl_11_func_800C3548 (s32 arg0);
void ovl_11_func_800EFABC (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8001AF70 (u16 arg0, u16 arg1);
u8 *ovl_11_func_800EFF04 (s32 arg0, s32 arg1, s32 *arg2);
s32 ovl_11_func_800EFE34 (s32 *ptr, s32 arg1, s32 arg2);
s32 ovl_11_func_800EFD54 (s32 arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800EBF18(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s6;
    s32 tpred;
    s32 c1;
    s32 c2;

    var_s0 = 1;
    c1 = 0x1B;
    c2 = 0x21;
    tpred = arg0 - 0x1F;
    var_s6 = (u32) tpred < 2U;
    var_s3 = 0x20000;
    for (; var_s0 < 0x25; var_s0 = temp_v0 >> 0x10) {
        if (var_s0 != c1) {
            if (var_s0 != c2) {
                if (var_s6) {
                    if (ovl_11_func_800C3548(var_s0) == 1) {
                        ovl_11_func_800EFABC(0x1F, var_s0, arg3, (s32) arg1);
                    } else {
                        ovl_11_func_800EFABC(0x20, var_s0, arg2, (s32) arg1);
                    }
                } else {
                    ovl_11_func_800EFABC((s32) arg0, var_s0, arg2, (s32) arg1);
                }
            }
        }
        temp_v0 = var_s3;
        var_s3 += 0x10000;
    }
    var_s0 = 0;
    if ((u32) (tpred & 0xFFFF) < 2U) {
        var_s1 = 0x10000;
        for (; var_s0 < 7; var_s0 = temp_v1 >> 0x10) {
            ovl_11_func_800EFABC(0x2B, var_s0, arg2, (s32) arg1);
            temp_v1 = var_s1;
            var_s1 += 0x10000;
        }
    }
    return 1;
}
#endif
