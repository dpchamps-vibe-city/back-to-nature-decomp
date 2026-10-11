# ovl_21_func_800BA300 — human decision needed

- **Parked:** 2026-10-11T14:51:15.824Z
- **Reason:** escalation-exhausted
- **Escalation reached:** deepseek-v4-1-flash
- **Source:** `src/overlays/ovl_21/ovl_21_func_800BA300.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 10, 10, 14] at 76/102 words, measured 2026-10-11T14:42:14, first scored from build/preparation/ovl_21_func_800BA300/cU3.c) rather than the source left on disk, which was never measured

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_21_func_800BA300 verdict MISMATCH — 76/116 words (65.5%).
Instruction count delta versus target: +4.
Residual (steer by this, not the word count): control-flow 0, population 10, schedule 10, allocation 14.
The two programs do not contain the same instructions, so no allocation or scheduling reading applies yet — fix the semantics first.
Next block: 7 (0x800BA3E8) — population 1, schedule 0, allocation 0. the instruction populations differ here; nothing below can be read until they agree
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 15 distinct measurements since the residual last improved on [0, 10, 10, 14]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 15 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


int ccos (int a);

typedef struct {
    u8 pad_00[0x34];
    s32 unk34;
    u16 unk38;
    u16 unk3A;
    s16 unk3C;
    s16 unk3E;
    u16 unk40;
    u16 unk42;
    u16 unk44;
    u16 pad_46;
    u16 unk48;
    u16 unk4A;
    u16 unk4C;
} Ovl21Func800BA300Rec;

void ovl_21_func_800BA300(void) {
    s16 temp_v1;
    s32 temp_lo;
    s32 var_s1;
    s32 var_s2;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;
    s32 *temp_s4;
    s32 temp_a0;
    u16 temp_a2;
    Ovl21Func800BA300Rec *temp_a0_2;
    u16 *temp_v0;
    Ovl21Func800BA300Rec *var_s0;
    u8 *raw;
    u8 *temp_s3;

    var_v1 = 0;
    var_s2 = 0;
    raw = (u8 *) D_800C0458 - 0x10;
    do {
        if ((*(s16 *) (raw + 0x64A)) > 0) {
            temp_s3 = (u8 *) D_800C0448;
            var_s1 = var_v1 * 0x108;
            var_s0 = (Ovl21Func800BA300Rec *) (temp_s3 + var_s1);
            temp_s4 = (s32 *) (temp_s3 + 0x34);
            do {
                if (*(s32 *) ((u8 *) temp_s4 + var_s1) != 0) {
                    temp_a2 = var_s0->unk4A;
                    temp_a0 = var_s0->unk3A - 1;
                    var_s0->unk3A = temp_a0;
                    var_s0->unk40 = (u16) (var_s0->unk40 + var_s0->unk48);
                    var_s0->unk42 = (u16) (var_s0->unk42 + temp_a2);
                    temp_v1 = var_s0->unk3E;
                    var_s0->unk44 = (u16) (var_s0->unk44 + var_s0->unk4C);
                    if (temp_v1 != 0) {
                        temp_lo = (s32) ((var_s0->unk3C - (s16) temp_a0) << 0xC) / (s32) (temp_v1 * 2);
                        var_v0 = temp_a2 + 0xC;
                        if (temp_lo < 0x800) {
                            var_v0_2 = ccos(temp_lo) * 0x18;
                            if (var_v0_2 < 0) {
                                var_v0_2 += 0xFFF;
                            }
                            var_v0 = -(var_v0_2 >> 0xC);
                        }
                        var_s0->unk4A = (u16) var_v0;
                        temp_a0_2 = (Ovl21Func800BA300Rec *) ((u8 *) D_800C0448 + var_s1);
                        if ((s16) temp_a0_2->unk42 >= 0) {
                            temp_v0 = (u16 *) ((u8 *) D_800C0448 + 0x48 + var_s1);
                            temp_a0_2->unk42 = 0;
                            temp_v0[0] = 0;
                            temp_v0[1] = 0;
                            temp_v0[2] = 0;
                            temp_a0_2->unk38 = 1;
                        }
                        if ((s16) temp_a0_2->unk3A < -0x3B) {
                            temp_s4[var_s1 >> 2] = 0;
                        }
                    }
                }
                var_s1 += 0x4C;
                var_s2 += 1;
                var_s0 = (Ovl21Func800BA300Rec *) ((u8 *) var_s0 + 0x4C);
            } while (var_s2 < (*(s16 *) (temp_s3 + 0x64A)));
        }
        var_v1 += 1;
        var_s2 = 0;
    } while (var_v1 < 6);
}
```
