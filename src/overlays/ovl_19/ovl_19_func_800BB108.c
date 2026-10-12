#include "common.h"

INCLUDE_ASM("build/ovl_19/asm/nonmatchings/ovl_19_func_800BB108", ovl_19_func_800BB108);


/* PARKED by /auto_decompilation_loop on 2026-10-12T05:30:13.203Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_19_func_800BB108.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80015840 (ObjectState *obj, s8 arg1);
void func_80015EE8 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
s32 ovl_19_func_800BADAC (s16 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_19_func_800BB08C (s32 arg0, s32 arg1);
s32 func_80011F5C (s32 arg0);
void func_80011FD8 (s32 arg0);
void func_8001585C (ObjectState *obj, s8 arg1);

static __inline__ s16 display_value(s16 *data, s32 row, s32 column) {
    return data[row * 4 + column];
}

void ovl_19_func_800BB108(void) {
    u32 sp18;
    u8 *base;
    u8 *object_base;
    ObjectState *const obj = (ObjectState *) D_800BF660;
    s32 values;
    u8 *state;
    s32 *first;
    u8 *text;
    s32 y;
    s32 rowY;
    s32 next;
    s32 more;
    s32 index;
    s32 var_a1;
    s32 var_a3;
    s32 surface;
    s32 width;
    s32 height;
    s32 column;
    s32 remaining;
    u8 *counter;
    s32 var_s1;

    func_80015840((ObjectState *) obj, 0);
    var_a1 = (s32) obj;
    var_s1 = 0;
    counter = (u8 *) obj;
    counter -= 0x1A0;
    values = (s32) counter;
    index = (s32) D_8005E3C0;
    surface = ((struct_8005E3C0 *) index)->field_D8;
    base = counter + 0x1A4;
    width = *base++;
    state = (u8 *) values + 0x1A5;
    height = *state;
    func_80015EE8(surface + 0x18, values + 0x1A0, width, height, 0, 0);
    column = 0;
    index = func_80017A64();
    sp18 = index;
    surface = 1;
    func_80017A48(surface);
    first = &D_8007AE58;
    text = (u8 *) D_800BCD68;
    do {
        next = var_s1 + 1;
        rowY = 60 << 16;
        y = 57;
        state = (u8 *) (values + 0x160);
        do {
            if (var_s1 == 0 && column == 0) {
                var_a1 = *first;
            } else {
                var_a3 = ovl_19_func_800BADAC(((s16 *) state)[var_s1 * 4 + column]);
                var_a1 = (s32) (text + var_a3);
            }
            func_80017B3C(D_8005E3C0->field_D8 + 0xC, var_a1, (s16) (var_s1 * 200 + 21), column > 0 ? rowY >> 16 : (s16) y);
            y += 45;
            rowY += 45 << 16;
            column++;
        } while (column < 4);
        more = next < 2;
        var_s1 = next;
        column = 0;
    } while (more);
    func_80017A48(sp18);
    {
    auto void nested_BB08C(s32 arg0, s32 arg1) __asm__("ovl_19_func_800BB08C");
    counter = (u8 *) D_800BF4C0;
    if (((s16 *) counter)[4] > 0) {
        nested_BB08C(1, 0);
        nested_BB08C(4, 0);
        nested_BB08C(5, 0);
        nested_BB08C(7, 0);
    }
    if (((s16 *) counter)[4] >= 2) {
        nested_BB08C(1, 1);
        nested_BB08C(7, 1);
    }
    }
}
#endif
