#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800D6488", ovl_11_func_800D6488);


/* PARKED by /auto_decompilation_loop on 2026-10-11T01:39:15.083Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800D6488.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"

void ovl_11_func_800D6488(s32 arg0, u8 *arg1, u8 *arg2) {
    s32 i;
    s32 j;
    s32 ni;
    u8 *d0;
    u8 *s0;
    u8 *d1;
    u8 *de1;
    u8 *s1;
    u8 *d2;
    u8 *s2;
    u8 *d3;
    u8 *s3;

    switch (arg0 & 0xFF) {
    case 0:
        i = 0;
        while (i < 13) {
            j = 12;
            ni = i + 1;
            d0 = arg1 + i * 13;
            s0 = arg2 + i + 156;
            while (j >= 0) {
                d0[j] = *s0;
                s0 -= 13;
                j--;
            }
            i = ni;
        }
        return;
    case 1:
        i = 0;
        while (i < 13) {
            j = 12;
            ni = i + 1;
            d1 = arg1 + i * 13;
            de1 = d1 + 12;
            s1 = arg2 + i * 13;
            while (j >= 0) {
                de1[-j] = s1[j];
                j--;
            }
            i = ni;
        }
        return;
    case 2:
        i = 12;
        while (i >= 0) {
            j = 0;
            ni = i - 1;
            d2 = arg1 + (12 - i) * 13;
            s2 = arg2 + i;
            while (j < 13) {
                d2[j] = *s2;
                s2 += 13;
                j++;
            }
            i = ni;
        }
        return;
    case 3:
        i = 0;
        while (i < 13) {
            j = 0;
            d3 = arg1 + i * 13;
            s3 = arg2 + i * 13;
            ni = i + 1;
            while (j < 13) {
                d3[j] = s3[j];
                j++;
            }
            i = ni;
        }
        return;
    }
}
#endif
