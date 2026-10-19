# ovl_25_func_800B8A30 — human decision needed

- **Parked:** 2026-10-19T02:15:21.974Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_25/ovl_25_func_800B8A30.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 0, 0, 1] at 166/175 words, measured 2026-10-19T01:02:56, first scored from build/experiments/ovl_25_func_800B8A30/refinit.c) rather than the source left on disk, which measured [0, 0, 0, 5] at 144/175 words, measured 2026-10-19T00:56:17

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_25_func_800B8A30 verdict MISMATCH — 166/176 words (94.3%).
Instruction count delta versus target: +1.
Residual (steer by this, not the word count): control-flow 0, population 0, schedule 0, allocation 1.
Next block: 4 (0x800B8A94) — population 0, schedule 0, allocation 1. smallest open residual
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

VALLEY: the best key [0, 0, 0, 1] is 1 term(s) from exact and 37 distinct programs since have not beaten it, across 47 distinct keys clustered around it. That is a floor, not a gradient. A best this close that a whole sweep cannot improve on means the remaining distance is several coordinates wide: every single-coordinate respelling moves one of the other coordinates the wrong way, scores worse, and reads as a refutation of the direction it tried. Sweeping one axis at a time cannot cross it however long it runs. Take the next experiment from the REQUIREMENT side instead of from another spelling:   psx_target_loop_emission  — what the original's loop pass must have emitted, and by which route   psx_triage                — its cluster-donor finding names a sibling that already reaches a                               mechanism this program does not, with the trace that measured it   psx_analyze_target_schedule / psx_allocator_counterfactual — the same for the later passes Write the program the requirement describes, in one edit, and measure that. STALLED: 37 distinct measurements since the residual last improved on [0, 0, 0, 1]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 37 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"

typedef struct {
    s16 unk0;
} Recon800B977CA0View;

u32 func_80012A34(s32 arg0);
s32 ovl_25_func_800B977C(Recon800B977CA0View *arg0, s16 arg1);
void ovl_25_func_800B9B00(s16 *arg0, s16 arg1, s16 arg2);
void ovl_25_func_800B95B0(s16 *arg0);

void ovl_25_func_800B8A30(void) {
    s16 *state;
    s16 *rec;
    s16 random;
    s32 value;
    s32 i;
    s16 *ref;
    s16 *objects;

    state = (s16 *)&D_800BFE44_view;
    if (state[0xF9] != 8) {
        ovl_25_func_800B9B00(&state[0xF8], 4, 1);
    }
    if (state[0x135] != 8) {
        ovl_25_func_800B9B00(&state[0x134], 4, 1);
    }
    ref = state;
    objects = ref + 8;
    for (i = 1; i < 4; i++) {
        s16 *state;

        state = (s16 *)&D_800BFE44_view;
        rec = &objects[i * 0x3C];
        if (rec[1] == 8) {
            continue;
        }
        if (rec[2] != 0 && (rec[1] == 4 || rec[1] == 5 || rec[1] == 7 || rec[1] == 6)) {
            continue;
        }
        if (rec[1] == 3) {
            continue;
        }
        if (*(s32 *)&rec[6] > 0x27FFF) {
            if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 0) == 1) {
                ovl_25_func_800B9B00(rec, 4, state[0x26A]);
            } else {
                ovl_25_func_800B95B0(rec);
            }
        } else if (*(s32 *)&rec[6] > 0x1DFFF) {
            if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 1) == 1) {
                ovl_25_func_800B9B00(rec, 4, state[0x26A]);
            } else {
                ovl_25_func_800B95B0(rec);
            }
        } else if (*(s32 *)&rec[6] > 0x13FFF) {
            if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 2) == 1) {
                ovl_25_func_800B9B00(rec, 4, state[0x26A]);
            } else {
                ovl_25_func_800B95B0(rec);
            }
        } else if (*(s32 *)&rec[6] > 0x9FFF) {
            if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 3) == 1) {
                ovl_25_func_800B9B00(rec, 4, state[0x26A]);
            } else if (ovl_25_func_800B977C((Recon800B977CA0View *)rec, 4) == 1) {
                ovl_25_func_800B95B0(rec);
            } else {
                ref = objects - 8;
                random = func_80012A34((u16)(120 / ref[0x26C])) + 1;
                value = ref[0x267] * 30;
                if (random >= value / (ref[0x26C] * 100)) {
                    ovl_25_func_800B9B00(rec, 3, 60);
                    value = random - 2;
                    rec[3] = value * 30 / ref[0x26A];
                } else {
                    ovl_25_func_800B9B00(rec, 2, random * (u16)ref[0x26A]);
                }
            }
        } else {
            ovl_25_func_800B9B00(rec, 7, 60);
        }
    }
}
```
