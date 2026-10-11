# ovl_11_func_800FE3A0 — human decision needed

- **Parked:** 2026-10-11T08:43:28.954Z
- **Reason:** escalation-exhausted
- **Escalation reached:** deepseek-v4-1-flash
- **Source:** `src/overlays/ovl_11/ovl_11_func_800FE3A0.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 18, 6, 17] at 49/98 words, measured 2026-10-11T08:06:32, first scored from build/candidates/ovl_11_func_800FE3A0/cand29.c) rather than the source left on disk, which was never measured

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_800FE3A0 verdict MISMATCH — 49/108 words (45.4%).
Instruction count delta versus target: +1.
Residual (steer by this, not the word count): control-flow 0, population 18, schedule 6, allocation 17.
The two programs do not contain the same instructions, so no allocation or scheduling reading applies yet — fix the semantics first.
Next block: 7 (0x800FE45C) — population 1, schedule 0, allocation 0. the instruction populations differ here; nothing below can be read until they agree
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 12 distinct measurements since the residual last improved on [0, 18, 6, 17]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 12 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"

s16 *func_8001A970(s32 arg0, s16 *arg1, s32 arg2);
void func_8001ABF0(u16 *dst, u16 *src);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800FE3A0(s16 arg0, u16 arg1, s16 arg2, s16 arg3) {
    s16 *var_a1;
    s32 var_a0;
    s32 var_a3;
    u16 *var_a1_2;
    u16 temp_a1;
    u32 temp_t0;

    temp_t0 = arg1 & 0xFFFF;
    if (arg0 < 0) {
        var_a1 = (s16 *) ((u8 *) D_8005378C + *D_80054BBC);
    } else {
        var_a3 = 1;
        if (arg0 == 5) {
            var_a1 = (s16 *) ((u8 *) D_800537A2 + *D_80054BBC);
        } else {
            if (temp_t0 >= D_80127214[1]) {
                var_a0 = 0x20000;
                var_a1_2 = &D_80127214[1];
                do {
                    var_a3 = var_a0 >> 0x10;
                    var_a0 += 0x10000;
                } while (var_a3 < 5 && temp_t0 >= *++var_a1_2);
                var_a1_2 -= 1;
            }
            temp_a1 = D_80127214[var_a3 - 1];
            func_8001ABF0(func_8001A970(((s32) ((temp_t0 - temp_a1) * 100) /
                                              (s32) (var_a1_2[0] - temp_a1) +
                                          (var_a3 - arg0) * 100) &
                                            0xFFFF,
                                        &D_8012A028, 4),
                          (u16 *) ((u8 *) D_800535D0 + *D_80054BBC));
            var_a1 = &D_8012A028;
        }
    }
    func_80017B3C(D_8005E3C0->field_D8 + 0x54, (s32) var_a1, (s32) arg2,
                  (s32) arg3);
}
```
