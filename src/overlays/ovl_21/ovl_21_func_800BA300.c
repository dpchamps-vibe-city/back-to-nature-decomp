#include "common.h"

INCLUDE_ASM("build/ovl_21/asm/nonmatchings/ovl_21_func_800BA300", ovl_21_func_800BA300);


/* PARKED by /auto_decompilation_loop on 2026-10-11T14:51:15.824Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_21_func_800BA300.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


int ccos (int a);

typedef struct {
    u8 pad_00[0x34];
    s32 unk34;
    u16 unk38;
    u16 unk3A;
    s16 unk3C;
    s16 unk3E;
    u16 unk40;
    u16 unk42;
    u16 unk44;
    u16 pad_46;
    u16 unk48;
    u16 unk4A;
    u16 unk4C;
} Ovl21Func800BA300Rec;

void ovl_21_func_800BA300(void) {
    s16 temp_v1;
    s32 temp_lo;
    s32 var_s1;
    s32 var_s2;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;
    s32 *temp_s4;
    s32 temp_a0;
    u16 temp_a2;
    Ovl21Func800BA300Rec *temp_a0_2;
    u16 *temp_v0;
    Ovl21Func800BA300Rec *var_s0;
    u8 *raw;
    u8 *temp_s3;

    var_v1 = 0;
    var_s2 = 0;
    raw = (u8 *) D_800C0458 - 0x10;
    do {
        if ((*(s16 *) (raw + 0x64A)) > 0) {
            temp_s3 = (u8 *) D_800C0448;
            var_s1 = var_v1 * 0x108;
            var_s0 = (Ovl21Func800BA300Rec *) (temp_s3 + var_s1);
            temp_s4 = (s32 *) (temp_s3 + 0x34);
            do {
                if (*(s32 *) ((u8 *) temp_s4 + var_s1) != 0) {
                    temp_a2 = var_s0->unk4A;
                    temp_a0 = var_s0->unk3A - 1;
                    var_s0->unk3A = temp_a0;
                    var_s0->unk40 = (u16) (var_s0->unk40 + var_s0->unk48);
                    var_s0->unk42 = (u16) (var_s0->unk42 + temp_a2);
                    temp_v1 = var_s0->unk3E;
                    var_s0->unk44 = (u16) (var_s0->unk44 + var_s0->unk4C);
                    if (temp_v1 != 0) {
                        temp_lo = (s32) ((var_s0->unk3C - (s16) temp_a0) << 0xC) / (s32) (temp_v1 * 2);
                        var_v0 = temp_a2 + 0xC;
                        if (temp_lo < 0x800) {
                            var_v0_2 = ccos(temp_lo) * 0x18;
                            if (var_v0_2 < 0) {
                                var_v0_2 += 0xFFF;
                            }
                            var_v0 = -(var_v0_2 >> 0xC);
                        }
                        var_s0->unk4A = (u16) var_v0;
                        temp_a0_2 = (Ovl21Func800BA300Rec *) ((u8 *) D_800C0448 + var_s1);
                        if ((s16) temp_a0_2->unk42 >= 0) {
                            temp_v0 = (u16 *) ((u8 *) D_800C0448 + 0x48 + var_s1);
                            temp_a0_2->unk42 = 0;
                            temp_v0[0] = 0;
                            temp_v0[1] = 0;
                            temp_v0[2] = 0;
                            temp_a0_2->unk38 = 1;
                        }
                        if ((s16) temp_a0_2->unk3A < -0x3B) {
                            temp_s4[var_s1 >> 2] = 0;
                        }
                    }
                }
                var_s1 += 0x4C;
                var_s2 += 1;
                var_s0 = (Ovl21Func800BA300Rec *) ((u8 *) var_s0 + 0x4C);
            } while (var_s2 < (*(s16 *) (temp_s3 + 0x64A)));
        }
        var_v1 += 1;
        var_s2 = 0;
    } while (var_v1 < 6);
}
#endif
