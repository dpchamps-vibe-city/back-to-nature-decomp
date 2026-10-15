#include "common.h"

INCLUDE_ASM("build/ovl_17/asm/nonmatchings/ovl_17_func_800B9940", ovl_17_func_800B9940);


/* PARKED by /auto_decompilation_loop on 2026-10-15T20:35:20.567Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_17_func_800B9940.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);

s16 ovl_17_func_800B9940(s32 arg0) {
    s32 sp10;
    u16 sp14;
    s32 sp18;
    s16 *temp_s0_2;
    s32 *var_a1;
    s32 temp_s0;
    s32 temp_s1;
    s32 var_a0;
    s32 var_s3;
    s32 var_s5;
    u8 *temp_s2;
    u8 *work;
    u8 *records;
    s32 off;
    u16 end;
    u8 *record;

    sp14 = 2;
    var_a1 = (s32 *)D_8005E3C0->field_D8;
    temp_s1 = (s32)(var_a1 + 2);
    work = D_80074838;
    records = D_800BD848;
    if (((Ovl17ModeView *)D_8006C838)->mode != 3) {
        sp14 = (u16)((Ovl17ModeView *)D_8006C838)->mode;
    }
    var_s3 = 0x40;
    var_s5 = 0;
    end = 0xFFFF;
    off = 0;
    record = records + 0x40;
    sp10 = 0;
    sp18 = sp14;
    sp18 *= 4;
    do {
        if ((*(s16 *) (records + 0x2BC)) == var_s5) {
            D_800BD738[0] = 0x90;
            D_800BD738[1] = end;
            func_80017B3C(temp_s1, (s32) D_800BD738, 0x2E, var_s3);
        }
        D_800BD738[1] = end;
        D_800BD738[0] = (s16) *((u16 *) ((u8 *) &D_800BB6EC + (var_s5 * 2)));
        func_80017B3C(temp_s1, (s32) D_800BD738, 0x38, var_s3);
        if (((*(s16 *) (work + 0x6514)) == 3) && (var_s5 == 2)) {
            var_a1 = &D_80075AD8;
        } else {
            var_a1 = (s32 *) &D_800556A8[*(u16 *) (record)];
        }
        func_80017B3C(temp_s1, (s32) var_a1, 0x46, var_s3);
        func_80017B3C(temp_s1, (s32) &D_800BB49A, 0x8E, var_s3);
        temp_s0 = off + ((((sp18 + sp14) * 4) + sp14) * 4);
        temp_s2 = work + 0x6520 + temp_s0;
        *(u16 *)func_8001A970((s32) (*(s16 *) ((u8 *) temp_s2 + 0)), (s16 *)D_800BD738, 2) = end;
        func_80017B3C(temp_s1, (s32) D_800BD738, 0xB2, var_s3);
        temp_s0_2 = (s16 *) ((u8 *) &D_8007AD5A + temp_s0);
        *(u16 *)func_8001A970((s32) *temp_s0_2, (s16 *)D_800BD738, 2) = end;
        func_80017B3C(temp_s1, (s32) D_800BD738, 0xCA, var_s3);
        func_80017B3C(temp_s1, (s32) ((void *) ((u8 *) &D_800BB49A + 0xA)), 0xDE, var_s3);
        if ((*(s16 *) (records + 0x2BC)) == var_s5) {
            sp10 = (sp10 + ((u16) (*(s16 *) ((u8 *) temp_s2 + 0)) * (u16) *temp_s0_2)) & 0xFFFF;
        }
        var_s3 = (s16)(var_s3 + 0xD);
        off += 0xE;
        var_s5 += 1;
        record += 0x50;
    } while (var_s5 < 6);
    func_80017B3C(temp_s1, (s32) ((void *) ((u8 *) &D_800BB49A + 0x1A)), 0x4E, 0x90);
    var_a0 = 0;
    if (arg0 != 0) {
        var_a0 = sp10;
    }
    *(u16 *)func_8001A970(var_a0, (s16 *)D_800BD738, 4) = 0xFFFF;
    func_80017B3C(temp_s1, (s32) D_800BD738, 0xB6, 0x90);
    return (s16) sp10;
}
#endif
