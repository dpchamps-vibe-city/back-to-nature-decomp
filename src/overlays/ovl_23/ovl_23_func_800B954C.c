#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"

void func_80015840(ObjectState *obj, s8 arg1);
/* The original caller masks the promoted frame byte before this legacy call. */
void func_8001585C();
void func_80015BF0(s32 arg0, SpriteSourceData *arg1, s16 arg2, s16 arg3);

/* Draws the two marker strips (12 entries at +0x5CC, 54 at +0x418): every
 * live entry within half a screen of the camera gets its sprite frame and is
 * blitted at HWD0 / 2 - delta.
 *
 * Two constructs are here for the allocator, not for behaviour:
 *
 * - "test = roundedCamera * 8; test = frame + roundedCamera * 8;" gives the
 *   target's addu v1,s1,v1 operand order. expand_binop swaps commutative
 *   operands when the target is the second operand, so "test = frame + test"
 *   always emits test + frame. Here cse substitutes test for the recomputed
 *   product after expansion, and the first store is dead.
 *
 * - "distance = (position | cam) < 0; ... + (distance >> 1)" adds 0 to x.
 *   Flow still sees that use of position and cam inside the if-body, so both
 *   are live across the delta and HWD0 / 2 computation. Combine then proves
 *   the term is zero (nonzero_bits of a comparison) and deletes it, but
 *   global-alloc works from flow's live-in sets, which are not recomputed.
 *   That conflict picture puts position in t0, cam in a2 and delta in a3, as
 *   in the target. Without the term the result is 141/165.
 *
 * Mechanism: notes/research/ovl_23_func_800B954C-variable-partition.md. */
void ovl_23_func_800B954C(void) {
    s16 i;
    s32 frame;
    s32 kind;
    s16 x;
    s32 position;
    s32 roundedCamera;
    s32 distance;
    s32 test;
    s32 cam;
    s32 delta;
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
            test = frame + roundedCamera * 8;
            test = (u32)test << 16;
            frame = test >> 16;
            kind = position;
            if (position < 0) {
                kind = position + 4095;
            }
            distance = kind >> 12;
            cam = *(s32 *)(base + 0x10);
            delta = distance - cam / 4096;
            distance = abs(delta);
            distance = HWD0 / 2 < distance;
            if (!distance) {
                distance = (position | cam) < 0;
                x = HWD0 / 2 - delta + (distance >> 1);
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
        distance = i * 8;
        if (walk->unk418[0].unk4 != -1) {
            position = ((Ovl23D87CView *)(base + distance))->unk418[0].unk0;
            kind = walk->unk418[0].unk6;
            kind ^= 6;
            frame = kind == 0;
            test = (s16)(i / 12);
            roundedCamera = i - test * 12;
            roundedCamera = roundedCamera < 6;
            roundedCamera ^= 1;
            test = roundedCamera * 2;
            frame |= test;
            kind = position;
            if (position < 0) {
                kind = position + 4095;
            }
            distance = kind >> 12;
            cam = *(s32 *)(base + 0x10);
            delta = distance - cam / 4096;
            distance = abs(delta);
            distance = HWD0 / 2 < distance;
            if (!distance) {
                distance = (position | cam) < 0;
                x = HWD0 / 2 - delta + (distance >> 1);
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
