#include "common.h"

INCLUDE_ASM("build/ovl_23/asm/nonmatchings/ovl_23_func_800B8354", ovl_23_func_800B8354);


/* PARKED by /auto_decompilation_loop on 2026-10-11T12:31:05.058Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_23_func_800B8354.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"

s32 func_80012A34(s32 arg0);
extern u8 D_800BF87C[];
extern u16 D_800BBA64[];
extern u16 D_800BBA90[];

typedef struct {
    u8 pad_00[0x24];
    u16 unk24;         /* 0x24 */
    u16 unk26;         /* 0x26 */
    u16 unk28;         /* 0x28 */
    u16 unk2A;         /* 0x2A */
    u16 unk2C;         /* 0x2C */
    s16 unk2E;         /* 0x2E */
    u16 unk30;         /* 0x30 */
    u16 unk32;         /* 0x32 */
    u32 unk34;         /* 0x34 */
    u32 unk38;         /* 0x38 */
    u32 unk3C;         /* 0x3C */
    u8 pad_40[0x10];
} BtnEntry50;

typedef struct {
    BtnEntry50 unk0[32];
} BtnView50;

typedef struct {
    u8 pad_00[0x70];
    u16 unk70;
    u16 unk72;
} BtnTail;

void ovl_23_func_800B8354(void) {
    s32 i;
    u8 *base;
    u8 *rec2;
    u8 *work;
    u8 *rec4;
    u16 *src;

    i = 0;
    base = D_800BF87C;
    rec2 = base + 0xA0;
    work = D_80074838;
    src = D_800BBA64;
    for (; i < 6; i++) {
        BtnEntry50 *pr = &((BtnView50 *)base)->unk0[i];

        pr->unk24 = i;
        pr->unk26 = 1;
        pr->unk28 = 2;
        pr->unk2A = 2;
        pr->unk2C = 0;
        ((BtnView50 *)base)->unk0[i].unk38 = 0xFFFE5000;
        ((BtnView50 *)base)->unk0[i].unk3C = ((s16 *)&D_800BBA40)[i] << 12;
        ((BtnTail *)pr)->unk70 = src[0];
        ((BtnTail *)pr)->unk72 = src[1];
        if (i == 4) {
            rec4 = base + 0x140;
            *(u16 *)(rec4 + 0x30) = 0xFF;
            *(u16 *)(rec4 + 0x32) = 0xFF;
        } else {
            if (i != 2) {
                pr->unk30 = func_80012A34(0x60) + 0xA0;
                pr->unk32 = func_80012A34(0x9C) + 0x64;
            } else {
                *(u16 *)(rec2 + 0x30) = *(u16 *)(work + 0x11C2);
                *(u16 *)(rec2 + 0x32) = *(u16 *)(work + 0x1258);
            }
        }
        pr->unk2E = D_800BBA90[(s16)pr->unk32 / 50];
        ((BtnView50 *)base)->unk0[i].unk34 =
            (s16)*(u16 *)(base + 0x3B0) * ((s16)pr->unk30 + 0x80) << 4;
        src += 2;
    }
}
#endif
