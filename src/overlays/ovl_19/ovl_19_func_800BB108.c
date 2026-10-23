#include "common.h"
#include "game_types.h"

void func_80015840(ObjectState *obj, u8 arg1);
void func_8001585C(ObjectState *obj, u8 arg1);
void func_80015EE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
u32 func_80017A64(void);
void func_80017A48(u32 arg0);
s32 ovl_19_func_800BADAC(s16 arg0);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* Draws the 2x4 value table at the D_800BF4C0 view with the draw mode
 * switched to 1, then refreshes the object at D_800BF660 through the nested
 * helper for the entries the D_800BF4C0 count enables.
 *
 * The helper at 0x800BB08C is a GCC nested function, defined here as in the
 * original translation unit, which emits it immediately before this function
 * (see ovl_19_func_800BB08C.c).
 *
 * User-authorized matching workaround (2026-10-23): `column` is bound to s0.
 * The original keeps the first `column = 0` below the func_80015EE8 call; our
 * cc1's backward scheduler floats a constant write to a call-crossing pseudo
 * above that call, which pushes column out of s0 and rotates row/column/y.
 * Binding the variable makes that one write a hard-register set, which this
 * sched.c keeps below the preceding call. The rest of the body is C. This is
 * evidence of a toolchain scheduling difference, not of a register pin in
 * the original source; see notes/asm-exception-evidence.md.
 */
void ovl_19_func_800BB108(void) {
    void ovl_19_func_800BB08C(s32 arg0, s32 arg1) {
        u8 *base;

        func_80015840((ObjectState *)&D_800BF660, arg0 & 0xFF);
        func_8001585C((ObjectState *)&D_800BF660, arg1 & 0xFF);
        base = D_800BF660 - 0x1A0;
        func_80015EE8(D_8005E3C0->field_D8 + 0x14, (s32)&D_800BF660,
                      base[0x1A4], base[0x1A5], 0, 0);
    }
    u32 saved;
    u8 *base;
    u8 *obj = D_800BF660;
    s32 values;
    u8 *state;
    s32 *first;
    u8 *text;
    s32 y;
    s32 rowY;
    s32 next;
    s32 more;
    s32 offset;
    s32 label;
    register s32 column asm("$16");
    u8 *counter;
    s32 row;

    func_80015840((ObjectState *)obj, 0);
    base = obj;
    obj -= 0x1A0;
    values = (s32)obj;
    func_80015EE8(D_8005E3C0->field_D8 + 0x18, (s32)base, obj[0x1A4], obj[0x1A5], 0, 0);
    row = 0;
    column = 0;
    saved = func_80017A64();
    func_80017A48(1);
    first = &D_8007AE58;
    text = (u8 *)D_800BCD68;
    do {
        next = row + 1;
        rowY = 60 << 16;
        y = 57;
        state = (u8 *)(values + 0x160);
        do {
            if (row == 0 && column == 0) {
                label = *first;
            } else {
                offset = ovl_19_func_800BADAC(((s16 *)state)[row * 4 + column]);
                label = offset + (s32)text;
            }
            func_80017B3C(D_8005E3C0->field_D8 + 0xC, label, (s16)(row * 200 + 21),
                          column > 0 ? rowY >> 16 : (s16)y);
            y += 45;
            rowY += 45 << 16;
            column++;
        } while (column < 4);
        more = next < 2;
        row = next;
        column = 0;
    } while (more);
    func_80017A48(saved);
    counter = (u8 *)D_800BF4C0;
    if (((s16 *)counter)[4] > 0) {
        ovl_19_func_800BB08C(1, 0);
        ovl_19_func_800BB08C(4, 0);
        ovl_19_func_800BB08C(5, 0);
        ovl_19_func_800BB08C(7, 0);
    }
    if (((s16 *)counter)[4] >= 2) {
        ovl_19_func_800BB08C(1, 1);
        ovl_19_func_800BB08C(7, 1);
    }
}
