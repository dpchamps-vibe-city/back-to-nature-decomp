# ovl_11_func_800EBF18 — human decision needed

- **Parked:** 2026-10-11T06:24:25.050Z
- **Reason:** escalation-exhausted
- **Escalation reached:** deepseek-v4-1-flash
- **Source:** `src/overlays/ovl_11/ovl_11_func_800EBF18.c` (INCLUDE_ASM restored)
- **Preserved attempt:** the source on disk is the best measured program ([0, 3, 2, 4] at 75/79 words, measured 2026-10-11T06:01:45)

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_800EBF18 verdict MISMATCH — 75/83 words (90.4%).
Residual (steer by this, not the word count): control-flow 0, population 3, schedule 2, allocation 4.
The two programs do not contain the same instructions, so no allocation or scheduling reading applies yet — fix the semantics first.
Next block: 10 (0x800EBFE0) — population 1, schedule 0, allocation 0. the instruction populations differ here; nothing below can be read until they agree
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 11 distinct measurements since the residual last improved on [0, 3, 2, 4]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 11 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 ovl_11_func_800C3548 (s32 arg0);
void ovl_11_func_800EFABC (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8001AF70 (u16 arg0, u16 arg1);
u8 *ovl_11_func_800EFF04 (s32 arg0, s32 arg1, s32 *arg2);
s32 ovl_11_func_800EFE34 (s32 *ptr, s32 arg1, s32 arg2);
s32 ovl_11_func_800EFD54 (s32 arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800EBF18(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s6;
    s32 tpred;
    s32 c1;
    s32 c2;

    var_s0 = 1;
    c1 = 0x1B;
    c2 = 0x21;
    tpred = arg0 - 0x1F;
    var_s6 = (u32) tpred < 2U;
    var_s3 = 0x20000;
    for (; var_s0 < 0x25; var_s0 = temp_v0 >> 0x10) {
        if (var_s0 != c1) {
            if (var_s0 != c2) {
                if (var_s6) {
                    if (ovl_11_func_800C3548(var_s0) == 1) {
                        ovl_11_func_800EFABC(0x1F, var_s0, arg3, (s32) arg1);
                    } else {
                        ovl_11_func_800EFABC(0x20, var_s0, arg2, (s32) arg1);
                    }
                } else {
                    ovl_11_func_800EFABC((s32) arg0, var_s0, arg2, (s32) arg1);
                }
            }
        }
        temp_v0 = var_s3;
        var_s3 += 0x10000;
    }
    var_s0 = 0;
    if ((u32) (tpred & 0xFFFF) < 2U) {
        var_s1 = 0x10000;
        for (; var_s0 < 7; var_s0 = temp_v1 >> 0x10) {
            ovl_11_func_800EFABC(0x2B, var_s0, arg2, (s32) arg1);
            temp_v1 = var_s1;
            var_s1 += 0x10000;
        }
    }
    return 1;
}
```
