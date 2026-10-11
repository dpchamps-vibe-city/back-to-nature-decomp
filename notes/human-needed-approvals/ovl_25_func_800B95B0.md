# ovl_25_func_800B95B0 — human decision needed

- **Parked:** 2026-10-11T05:04:39.683Z
- **Reason:** asm-needs-human-approval
- **Escalation reached:** deepseek-v4-1-flash
- **Source:** `src/overlays/ovl_25/ovl_25_func_800B95B0.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([1, 51, 0, 2] at 97/105 words, measured 2026-10-11T05:01:41, first scored from build/candidates/sweepL/w01.c) rather than the source left on disk, which was never measured

## What the loop needs

The top escalation tier proposed a source construct the clean-source policy forbids,
and there is no higher agent to adjudicate it. Decide whether the construct is the
correct answer for this function. If it is, add the allowlist entry to
`.pi/autoloop.json` under `sourcePolicy.allowlist` and re-run the loop on this
target. If it is not, the function needs a different structural hypothesis.

## Policy findings

- `configs/flag_overrides.mk:635` — **flag-override** — Per-function compiler flag override is not allowlisted
  `CC1FLAGS_ovl_25_func_800B95B0 := -fno-cse-follow-jumps`
- `configs/flag_overrides.mk:635` — **flag-override** — New per-function compiler flag overrides are forbidden
  `CC1FLAGS_ovl_25_func_800B95B0 := -fno-cse-follow-jumps`

## Oracle report for the preserved attempt

```
Oracle: ovl_25_func_800B95B0 verdict MISMATCH — 97/106 words (91.5%).
Instruction count delta versus target: -1.
Residual (steer by this, not the word count): control-flow 1, population 51, schedule 0, allocation 2.
The two programs do not contain the same instructions, so no allocation or scheduling reading applies yet — fix the semantics first.
Next block: 16 (0x800B96E0) — population 2, schedule 0, allocation 0. the instruction populations differ here; nothing below can be read until they agree
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 15 distinct measurements since the residual last improved on [1, 51, 0, 2]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 15 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"

typedef struct {
    s16 unk0;
    char pad_02[0x0A];
    s32 unkC;
} Recon800B95B0View;

typedef struct {
    u16 v[4];
} Recon800B95B0Row;

typedef struct {
    Recon800B95B0Row r[3];
} Recon800B95B0Block;

extern s16 D_800BFE44;
extern Recon800B95B0Block D_800BCCC0[];

s32 func_80012A34(s32 arg0);

void ovl_25_func_800B95B0(Recon800B95B0View *arg0) {
    s16 rnd;
    s16 sum;
    s16 sel;
    s16 result;
    s32 i;
    s32 val;
    s16 *ref;

    sum = 0;
    sel = 0;
    rnd = func_80012A34(0x64);
    val = arg0->unkC;
    if (val > 0x27FFF) {
        sel = -1;
    } else if (val > 0x1DFFF) {
        sel = 0;
    } else if (val > 0x13FFF) {
        sel = 1;
    } else if (val > 0x9FFF) {
        sel = 3;
    }
    i = 0;
    for (; i < 4; i++) {
        sum = (s16)(sum + D_800BCCC0[sel].r[arg0->unk0 - 1].v[i]);
        if (rnd < sum) {
            break;
        }
    }
    if (sel == -1) {
        result = 0x32;
    } else {
        result = (s16)(0x31 - i * 10);
        if (i == 0) {
            result = (s16)(result + 1);
        }
    }
    ref = &D_800BFE44;
    val = arg0->unkC;
    if (val < 0) {
        val += 0xFFF;
    }
    val >>= 12;
    ovl_25_func_800B9B00((s16 *)arg0, 5, (s16)((result - val) * ref[0x26C] / ref[0x26B]));
}
```
