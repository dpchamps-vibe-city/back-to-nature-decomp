# ovl_23_func_800B954C — human decision needed

- **Parked:** 2026-10-15T01:43:31.172Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_23/ovl_23_func_800B954C.c` (INCLUDE_ASM restored)
- **Preserved attempt:** Current worktree attempt preserved verbatim at the user's request; this parking operation did not run a new matching escalation. The target-specific Ovl23_D_800BF87C_draw declaration is retained in include/globals_override.h; unrelated header edits are excluded from the commit.

## What the loop needs

Parked at the user's explicit request using the standard parked-function template.
The current worktree attempt is not byte-exact; no new escalation or policy exception
was attempted during parking. Resume from the preserved C and measured residual
below when matching is requested again.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Match: 134/165 words (81.2%)
VERDICT: MISMATCH — 31 word(s) differ.
  differing words: 0x800B95A0 0x800B95AC 0x800B95E0 0x800B95E4 0x800B95E8 0x800B95EC 0x800B95F0 0x800B95F4 0x800B95FC 0x800B9600 0x800B9604 0x800B960C 0x800B9618 0x800B961C 0x800B9638 0x800B96B4 0x800B96C4 0x800B96CC 0x800B96DC 0x800B970C 0x800B9710 0x800B9714 0x800B9718 0x800B971C (+7 more)
Residual: control-flow 0, population 0, schedule 0, allocation 9.
Next block: 4 (0x800B95F4), population 0, schedule 0, allocation 1.
Also open: blocks 6, 12, 15, 17 (allocation 1 each), 2 and 13 (allocation 2 each).
Source policy: passed before parking.
```

## Preserved attempt

```c
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
```
