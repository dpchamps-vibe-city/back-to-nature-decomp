#include "common.h"

INCLUDE_ASM("build/ovl_25/asm/nonmatchings/ovl_25_func_800B95B0", ovl_25_func_800B95B0);


/* PARKED by /auto_decompilation_loop on 2026-10-11T05:04:39.683Z.
 * Reason: asm-needs-human-approval.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_25_func_800B95B0.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"

typedef struct {
    s16 unk0;
    char pad_02[0x0A];
    s32 unkC;
} Recon800B95B0View;

typedef struct {
    u16 v[4];
} Recon800B95B0Row;

typedef struct {
    Recon800B95B0Row r[3];
} Recon800B95B0Block;

extern s16 D_800BFE44;
extern Recon800B95B0Block D_800BCCC0[];

s32 func_80012A34(s32 arg0);

void ovl_25_func_800B95B0(Recon800B95B0View *arg0) {
    s16 rnd;
    s16 sum;
    s16 sel;
    s16 result;
    s32 i;
    s32 val;
    s16 *ref;

    sum = 0;
    sel = 0;
    rnd = func_80012A34(0x64);
    val = arg0->unkC;
    if (val > 0x27FFF) {
        sel = -1;
    } else if (val > 0x1DFFF) {
        sel = 0;
    } else if (val > 0x13FFF) {
        sel = 1;
    } else if (val > 0x9FFF) {
        sel = 3;
    }
    i = 0;
    for (; i < 4; i++) {
        sum = (s16)(sum + D_800BCCC0[sel].r[arg0->unk0 - 1].v[i]);
        if (rnd < sum) {
            break;
        }
    }
    if (sel == -1) {
        result = 0x32;
    } else {
        result = (s16)(0x31 - i * 10);
        if (i == 0) {
            result = (s16)(result + 1);
        }
    }
    ref = &D_800BFE44;
    val = arg0->unkC;
    if (val < 0) {
        val += 0xFFF;
    }
    val >>= 12;
    ovl_25_func_800B9B00((s16 *)arg0, 5, (s16)((result - val) * ref[0x26C] / ref[0x26B]));
}
#endif
