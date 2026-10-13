#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800FE14C", ovl_11_func_800FE14C);


/* PARKED by /auto_decompilation_loop on 2026-10-13T05:08:56.959Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800FE14C.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"

void func_80014F0C(u32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6);
void func_80015074(s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s16 arg6);
void func_80014FAC(s32 *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s16 arg9);

void ovl_11_func_800FE14C(s16 level, u16 value, s32 x, s32 y) {
    s32 inner_y;
    s32 inner_x;
    s32 bottom;
    s32 right;
    s32 end_x;
    s32 start_x;
    s32 start_y;
    s16 width;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s32 end_y;
    s32 dark_color;
    s32 light_color;
    s32 top_edge;
    s32 i;
    u16 threshold;
    u32 *ot;
    s32 table_base;
    void *context;

    i = 0;
    end_y = D_8005E3C0->field_D8;
    ot = (u32 *) (end_y + 0x60);
    x = (s16) x;
    y = (s16) y;
    for (; i < 4; i++) {
        right = x + 0x83;
        bottom = y + 8;
        start_x = x;
        if (i % 3 != 0) {
            start_x = right;
        }
        x0 = start_x;
        start_y = y;
        top_edge = i < 2;
        if (top_edge == 0) {
            start_y = bottom;
        }
        y0 = start_y;
        end_x = x;
        if (top_edge != 0) {
            end_x = right;
        }
        x1 = end_x;
        end_y = y;
        if (i % 3 != 0) {
            end_y = bottom;
        }
        y1 = end_y;
        func_80014F0C(ot, x0, y0, x1, y1, 0xE0B888, 0);
    }
    x++;
    inner_x = x;
    inner_y = y + 1;
    func_80015074((s32 *) ot, inner_x, inner_y, 0x82, 7, 0xC8A060, 0);
    if (level < 0) {
        level *= -1;
    }
    light_color = D_80127388[level - 1];
    dark_color = D_8012739C[level - 1];
    if (level == 5) {
        width = 0x82;
    } else {
        threshold = D_80127214[level - 1];
        width = (s16) (((value - threshold) * 0x82) /
                       (D_80127214[level] - threshold));
        if (width >= 0x83) {
            width = 0x82;
        }
    }
    func_80014FAC((s32 *) (D_8005E3C0->field_D8 + 0x5C), inner_x, inner_y, width, 7,
                  dark_color, light_color, dark_color, light_color, 0);
}
#endif
