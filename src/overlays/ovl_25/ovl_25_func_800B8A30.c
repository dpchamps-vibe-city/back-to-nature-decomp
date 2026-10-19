#include "common.h"

INCLUDE_ASM("build/ovl_25/asm/nonmatchings/ovl_25_func_800B8A30", ovl_25_func_800B8A30);


/* PARKED by /auto_decompilation_loop on 2026-10-19T02:15:21.974Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_25_func_800B8A30.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"

typedef struct {
    s16 unk0;
} Recon800B977CA0View;

u32 func_80012A34(s32 arg0);
s32 ovl_25_func_800B977C(Recon800B977CA0View *arg0, s16 arg1);
void ovl_25_func_800B9B00(s16 *arg0, s16 arg1, s16 arg2);
void ovl_25_func_800B95B0(s16 *arg0);

void ovl_25_func_800B8A30(void) {
    s16 *state;
    s16 *rec;
    s16 random;
    s32 value;
    s32 i;
    s16 *ref;
    s16 *objects;

    state = (s16 *)&D_800BFE44_view;
    if (state[0xF9] != 8) {
        ovl_25_func_800B9B00(&state[0xF8], 4, 1);
    }
    if (state[0x135] != 8) {
        ovl_25_func_800B9B00(&state[0x134], 4, 1);
    }
    ref = state;
    objects = ref + 8;
    for (i = 1; i < 4; i++) {
        s16 *state;

        state = (s16 *)&D_800BFE44_view;
        rec = &objects[i * 0x3C];
        if (rec[1] == 8) {
            continue;
        }
        if (rec[2] != 0 && (rec[1] == 4 || rec[1] == 5 || rec[1] == 7 || rec[1] == 6)) {
            continue;
        }
        if (rec[1] == 3) {
            continue;
        }
        if (*(s32 *)&rec[6] > 0x27FFF) {
            if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 0) == 1) {
                ovl_25_func_800B9B00(rec, 4, state[0x26A]);
            } else {
                ovl_25_func_800B95B0(rec);
            }
        } else if (*(s32 *)&rec[6] > 0x1DFFF) {
            if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 1) == 1) {
                ovl_25_func_800B9B00(rec, 4, state[0x26A]);
            } else {
                ovl_25_func_800B95B0(rec);
            }
        } else if (*(s32 *)&rec[6] > 0x13FFF) {
            if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 2) == 1) {
                ovl_25_func_800B9B00(rec, 4, state[0x26A]);
            } else {
                ovl_25_func_800B95B0(rec);
            }
        } else if (*(s32 *)&rec[6] > 0x9FFF) {
            if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 3) == 1) {
                ovl_25_func_800B9B00(rec, 4, state[0x26A]);
            } else if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 4) == 1) {
                ovl_25_func_800B95B0(rec);
            } else {
                ref = objects - 8;
                random = func_80012A34((u16)(120 / ref[0x26C])) + 1;
                value = ref[0x267] * 30;
                if (random >= value / (ref[0x26C] * 100)) {
                    ovl_25_func_800B9B00(rec, 3, 60);
                    value = random - 2;
                    rec[3] = value * 30 / ref[0x26A];
                } else {
                    ovl_25_func_800B9B00(rec, 2, random * (u16)ref[0x26A]);
                }
            }
        } else {
            ovl_25_func_800B9B00(rec, 7, 60);
        }
    }
}
#endif
