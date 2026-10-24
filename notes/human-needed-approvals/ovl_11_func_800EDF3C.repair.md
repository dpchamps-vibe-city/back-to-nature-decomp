# ovl_11_func_800EDF3C — repair attempt

`/auto_decompilation_loop repair` did not reach a clean match. `src/overlays/ovl_11/ovl_11_func_800EDF3C.c` and its allowlist
entry are back as they were at `05ded5c72d28`, so the function still matches.

- Revoked for the attempt: register-asm, embedded-asm
- Reached tier: gpt-6.1-sol
- Started 2026-10-24T09:40:25.648Z, finished 2026-10-24T10:49:07.082Z
- Last program on disk (local runtime archive): `/home/dave/Documents/code/btn-decompilation/run_output/autoloop/attempts/ovl_11_func_800EDF3C.2026-10-24T10-49-07-082Z.repair-final.c`
- Its full change set (local runtime archive): `/home/dave/Documents/code/btn-decompilation/run_output/autoloop/repairs/ovl_11_func_800EDF3C.2026-10-24T10-49-07-082Z/changes.patch`

## Constructs the repair had to remove



## Best clean program

[0, 0, 0, 3] at 44/48 words, measured 2026-10-24T09:44:33 (the experiment ledger's measurement of this exact text).

```c
#include "common.h"

/* User-authorized parked recovery: three real local register roles and
 * a two-instruction large-offset address prefix; calls and control are C. */

s32 func_8001AF44(u32 arg0);

s32 ovl_11_func_800EDF3C(s16 arg0, s32 arg1) {
    s16 var_a0;
    s32 temp_v0;
    char *p;

    arg1 <<= 0x10;
    if (arg1 != 0) {
        var_a0 = D_80129560[arg0];
    } else {
        var_a0 = arg0;
    }
    if (var_a0 == 0xB) {
        goto block_b;
    }
    if (var_a0 == 0x14) {
        temp_v0 = func_8001AF44(0x25U);
        if (temp_v0 == 1) {
            if (D_80070CF2 == temp_v0) {
                goto ret1;
            }
            goto block_7;
        }
    }
    goto ret1;
block_7:
    return 0;
ret1:
    return 1;
block_b:
    if (func_8001AF44(0x129U) == 1) {
        goto block_7;
    }
    p = (char *) &D_8006C838;
    temp_v0 = 0x8000;
    p += temp_v0;
    arg1 = *(u8 *) (p + 0x6647);
    temp_v0 = 0xFF;
    if (arg1 != temp_v0) {
        goto block_7;
    }
    goto ret1;
}
```

## Last report

```text
No forbidden constructs remain.

Oracle: ovl_11_func_800EDF3C verdict MISMATCH — 45/49 words (91.8%).
Residual (steer by this, not the word count): control-flow 0, population 0, schedule 0, allocation 3.
Next block: 8 (0x800EDFC0) — population 0, schedule 0, allocation 1. smallest open residual
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 43 distinct measurements since the residual last improved on [0, 0, 0, 0]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 43 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

The next `/auto_decompilation_loop repair ovl_11_func_800EDF3C` is pointed at this note.
