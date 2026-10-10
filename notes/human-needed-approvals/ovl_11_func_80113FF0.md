# ovl_11_func_80113FF0 — human decision needed

- **Parked:** 2026-10-10T20:27:20.530Z
- **Reason:** escalation-exhausted
- **Escalation reached:** deepseek-v4-1-flash
- **Source:** `src/overlays/ovl_11/ovl_11_func_80113FF0.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 0, 9, 19] at 55/96 words, measured 2026-10-10T20:22:51, first scored from src/overlays/ovl_11/ovl_11_func_80113FF0.c) rather than the source left on disk, which measured [0, 0, 12, 8] at 70/90 words, measured 2026-10-10T20:25:55

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_80113FF0 verdict MISMATCH — 55/101 words (54.5%).
Residual (steer by this, not the word count): control-flow 0, population 0, schedule 9, allocation 19.
Next block: 0 (0x80113FF0) — population 0, schedule 9, allocation 19. smallest open residual
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 9 distinct measurements since the residual last improved on [0, 0, 9, 19]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 9 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void ovl_11_func_80111F10(void);
s32 ovl_11_func_800D6014(u16 arg0);
void ovl_11_func_8011D400(s32 *arg0);

s32 ovl_11_func_80113FF0(void) {
    s32 temp_s3;
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    ovl_11_func_80111F10();
    D_8012D538.unk0 = (s32) ((void *) ((u8 *) &D_80051E26 + D_80054BBC[1]));
    D_8012D538.unk4 = (void *) ((u8 *) ((void *) ((u8 *) &D_80051E26 - 0x60C)) + D_80054BBC[1]);
    temp_s3 = D_800719F8;
    D_8012D538.unkA = ovl_11_func_800D6014(0x69U);
    D_8012D538.unk8 = 1;
    ovl_11_func_8011D400((s32 *) D_8012D548);

    p = D_8012D548;
    q = (void **) p;
    a = (s16 *) (p + 0x28);
    c = (s8 *) (p + 0x3C);
    b = (s16 *) (p + 0x46);

    *q++ = (void *) ((s32) &D_80051E26 + 0 + D_80054BBC[1]);
    *(s8 *) (p + 0x5A) = 0xF;
    *(s8 *) (p + 0x5B) = 0x15;
    *(s16 *) (p + 0x5C) = 0;
    *(s16 *) (p + 0x28) = 0x69;
    *(s8 *) (p + 0x3C) = 0;
    *(s16 *) (p + 0x46) = 0x47;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &D_80051E26 + 0x1A + D_80054BBC[1]);
    *a++ = 0x109;
    *c++ = 0;
    *b++ = 0x4A;
    *q++ = (void *) ((s32) &D_80051E26 + 0x32 + D_80054BBC[1]);
    *a++ = 0;
    *c++ = 3;
    *b++ = 0x49;
    if (temp_s3 & 0x4000) {
        *q = (void *) ((s32) &D_80051E26 + 0x4C + D_80054BBC[1]);
    } else {
        *q = NULL;
    }
    *a = 0x1E;
    *c = 0;
    *b = 0x48;
    D_8012D52C = 0;
    return 1;
}
```
