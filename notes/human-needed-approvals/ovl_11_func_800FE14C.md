# ovl_11_func_800FE14C — human decision needed

- **Parked:** 2026-10-13T05:08:56.959Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_11/ovl_11_func_800FE14C.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 0, 2, 3] at 134/149 words, measured 2026-10-13T05:06:33, first scored from src/overlays/ovl_11/ovl_11_func_800FE14C.c) rather than the source left on disk, which measured [0, 0, 5, 3] at 132/144 words, measured 2026-10-13T05:08:08

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_800FE14C verdict MISMATCH — 134/149 words (89.9%).
Residual (steer by this, not the word count): control-flow 0, population 0, schedule 2, allocation 3.
Next block: 10 (0x800FE258) — population 0, schedule 0, allocation 1. smallest open residual
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 3 distinct measurements since the residual last improved on [0, 0, 2, 3]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain.
```

## Preserved attempt

```c
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
```
