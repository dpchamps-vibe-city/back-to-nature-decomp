#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800FE3A0", ovl_11_func_800FE3A0);


/* PARKED by /auto_decompilation_loop on 2026-10-11T08:43:28.954Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800FE3A0.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"

s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void func_8001ABF0(u16 *dst, u16 *src);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800FE3A0(s16 arg0, u16 arg1, s16 arg2, s16 arg3) {
    s16 *var_a1;
    s32 var_a0;
    s32 var_a3;
    u16 *var_a1_2;
    u16 temp_a1;
    u32 temp_t0;

    temp_t0 = arg1 & 0xFFFF;
    if (arg0 < 0) {
        var_a1 = (s16 *) ((u8 *) D_8005378C + *D_80054BBC);
    } else {
        var_a3 = 1;
        if (arg0 == 5) {
            var_a1 = (s16 *) ((u8 *) D_800537A2 + *D_80054BBC);
        } else {
            if (temp_t0 >= D_80127214[1]) {
                var_a0 = 0x20000;
                var_a1_2 = &D_80127214[1];
                do {
                    var_a3 = var_a0 >> 0x10;
                    var_a0 += 0x10000;
                } while (var_a3 < 5 && temp_t0 >= *++var_a1_2);
                var_a1_2 -= 1;
            }
            temp_a1 = D_80127214[var_a3 - 1];
            func_8001ABF0(func_8001A970(((s32) ((temp_t0 - temp_a1) * 100) /
                                              (s32) (var_a1_2[0] - temp_a1) +
                                          (var_a3 - arg0) * 100) &
                                            0xFFFF,
                                        &D_8012A028, 4),
                          (u16 *) ((u8 *) D_800535D0 + *D_80054BBC));
            var_a1 = &D_8012A028;
        }
    }
    func_80017B3C(D_8005E3C0->field_D8 + 0x54, (s32) var_a1, (s32) arg2,
                  (s32) arg3);
}
#endif
