# ovl_11_func_800D3E40 — human decision needed

- **Parked:** 2026-10-11T07:56:55.808Z
- **Reason:** escalation-exhausted
- **Escalation reached:** deepseek-v4-1-flash
- **Source:** `src/overlays/ovl_11/ovl_11_func_800D3E40.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 0, 0, 5] at 82/106 words, measured 2026-10-11T07:44:33, first scored from build/experiments/ovl_11_func_800D3E40/pin/vp_pind_a1.c) rather than the source left on disk, which measured [0, 0, 3, 6] at 82/104 words, measured 2026-10-11T06:46:14

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_800D3E40 verdict MISMATCH — 82/107 words (76.6%).
Residual (steer by this, not the word count): control-flow 0, population 0, schedule 0, allocation 5.
Next block: 0 (0x800D3E40) — population 0, schedule 0, allocation 1. smallest open residual
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 13 distinct measurements since the residual last improved on [0, 0, 0, 5]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 13 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D3E40(Ovl11Func801096FCArg x) {
    register s32 *flag asm("$14");
    register s32 **out asm("$15");
    register s32 result asm("$11");
    register u8 *p asm("$5");
    s32 lo1;
    s32 lo0;
    s16 sel;
    s32 i;
    u8 *base;
    s32 r;
    s32 hi0;
    s32 hi1;

    result = -1;
    i = 0;
    base = (u8 *)&D_800749F4;
    flag = x.unk18;
    out = x.unk1C;
    sel = x.unk10;
    r = x.unk14;
    lo0 = x.unk0 - r;
    hi0 = x.unk0 + r;
    lo1 = x.unk8 - r;
    hi1 = x.unk8 + r;
    do {
        p = base + i * 0xB8;
        if (*(u16 *)p != 0 && sel == *(s16 *)(p + 0x30) && (*(s32 *)(p + 0x34) & 0x20000) == 0 &&
            lo0 < *(s32 *)(p + 0x38) && *(s32 *)(p + 0x38) < hi0 &&
            lo1 < *(s32 *)(p + 0x40) && *(s32 *)(p + 0x40) < hi1) {
            result = i;
            if (*(s32 *)(p + 0x34) & 0x10000) {
                *flag = 0;
                *out = (s32 *)p;
            } else {
                *flag = 1;
                *out = (s32 *)p;
            }
            break;
        }
        i++;
    } while (i < 0x14);
    if (result == -1) {
        p = (u8 *)&D_800742EC;
        i = 0;
        do {
            if (*(u16 *)p != 0 && sel == *(s16 *)(p + 0x30) && (*(s32 *)(p + 0x34) & 0x20000) == 0 &&
                lo0 < *(s32 *)(p + 0x38) && *(s32 *)(p + 0x38) < hi0 &&
                lo1 < *(s32 *)(p + 0x40) && *(s32 *)(p + 0x40) < hi1) {
                result = i;
                *flag = 2;
                *out = (s32 *)p;
                break;
            }
            i++;
            p += 0xB4;
        } while (i < 0xA);
    }
    return result;
}
```
