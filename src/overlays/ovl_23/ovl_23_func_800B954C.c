#include "common.h"

INCLUDE_ASM("build/ovl_23/asm/nonmatchings/ovl_23_func_800B954C", ovl_23_func_800B954C);


/* PARKED by /auto_decompilation_loop on 2026-10-15T01:43:31.172Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_23_func_800B954C.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"

void func_80015840(ObjectState *obj, s8 arg1);
/* The original caller masks the promoted frame byte before this legacy call. */
void func_8001585C();
void func_80015BF0(s32 arg0, SpriteSourceData *arg1, s16 arg2, s16 arg3);

void ovl_23_func_800B954C(void) {
    s16 i;
    s16 frame;
    s32 kind;
    s16 x;
    s32 dx;
    s32 position;
    s32 roundedCamera;
    s32 test;
    s32 half;
    s32 screenX;
    ObjectState *sprite;
    u8 *base;
    Ovl23D87CView *walk;

    i = 0;
    base = Ovl23_D_800BF87C_draw;
    sprite = (ObjectState *)(base + 0x3B4);
    walk = (Ovl23D87CView *)base;
    do {
        kind = i * 8;
        if (walk->unk5CC[0].unk4 != -1) {
            position = ((Ovl23D87CView *)(base + kind))->unk5CC[0].unk0;
            kind = walk->unk5CC[0].unk6;
            frame = kind;
            test = (s16)(i / 12);
            roundedCamera = i - test * 12;
            roundedCamera = roundedCamera < 6;
            roundedCamera ^= 1;
            test = roundedCamera * 8;
            test += frame;
            frame = test;
            kind = position;
            if (position < 0) {
                kind += 4095;
            }
            position = kind >> 12;
            test = *(s32 *)(base + 0x10);
            dx = position - test / 4096;
            position = abs(dx);
            kind = HWD0;
            kind += (u32)kind >> 31;
            position = (kind >> 1) < position;
            if (!position) {
                x = (kind >> 1) - dx;
                func_80015840(sprite, 4);
                func_8001585C(sprite, frame & 0xFF);
                func_80015BF0(D_8005E3C0->field_D8 + 0xD0, (SpriteSourceData *)sprite, x, walk->unk5CC[0].unk4);
            }
        }
        walk = (Ovl23D87CView *)((u8 *)walk + 8);
        i++;
        test = i < 12;
    } while (test);
    i = 0;
    base = Ovl23_D_800BF87C_draw;
    sprite = (ObjectState *)(base + 0x3B4);
    walk = (Ovl23D87CView *)base;
    do {
        kind = i * 8;
        if (walk->unk418[0].unk4 != -1) {
            position = ((Ovl23D87CView *)(base + kind))->unk418[0].unk0;
            kind = walk->unk418[0].unk6;
            kind ^= 6;
            frame = kind == 0;
            test = (s16)(i / 12);
            roundedCamera = i - test * 12;
            roundedCamera = roundedCamera < 6;
            roundedCamera ^= 1;
            frame |= roundedCamera * 2;
            kind = position;
            if (position < 0) {
                kind += 4095;
            }
            position = kind >> 12;
            test = *(s32 *)(base + 0x10);
            dx = position - test / 4096;
            position = abs(dx);
            kind = HWD0;
            kind += (u32)kind >> 31;
            position = (kind >> 1) < position;
            if (!position) {
                x = (kind >> 1) - dx;
                func_80015840(sprite, 3);
                func_8001585C(sprite, frame & 0xFF);
                func_80015BF0(D_8005E3C0->field_D8 + 0xD0, (SpriteSourceData *)sprite, x, walk->unk418[0].unk4);
            }
        }
        walk = (Ovl23D87CView *)((u8 *)walk + 8);
        i++;
        test = i < 54;
    } while (test);
}
#endif
