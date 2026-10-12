# ovl_19_func_800BB108 — human decision needed

- **Parked:** 2026-10-12T05:30:13.203Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_19/ovl_19_func_800BB108.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 0, 2, 2] at 121/131 words, measured 2026-10-12T01:06:55, first scored from src/overlays/ovl_19/ovl_19_func_800BB108.c) rather than the source left on disk, which measured [0, 6, 3, 1] at 117/130 words, measured 2026-10-12T05:29:32

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_19_func_800BB108 verdict MISMATCH — 121/133 words (91%).
Instruction count delta versus target: +1.
Residual (steer by this, not the word count): control-flow 0, population 0, schedule 2, allocation 2.
Next block: 12 (0x800BB258) — population 0, schedule 0, allocation 1. smallest open residual
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

VALLEY: the best key [0, 0, 2, 2] is 4 term(s) from exact and 39 distinct programs since have not beaten it, across 99 distinct keys clustered around it. That is a floor, not a gradient. A best this close that a whole sweep cannot improve on means the remaining distance is several coordinates wide: every single-coordinate respelling moves one of the other coordinates the wrong way, scores worse, and reads as a refutation of the direction it tried. Sweeping one axis at a time cannot cross it however long it runs. Take the next experiment from the REQUIREMENT side instead of from another spelling:   psx_target_loop_emission  — what the original's loop pass must have emitted, and by which route   psx_triage                — its cluster-donor finding names a sibling that already reaches a                               mechanism this program does not, with the trace that measured it   psx_analyze_target_schedule / psx_allocator_counterfactual — the same for the later passes Write the program the requirement describes, in one edit, and measure that. STALLED: 39 distinct measurements since the residual last improved on [0, 0, 2, 2]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 39 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80015840 (ObjectState *obj, s8 arg1);
void func_80015EE8 (s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
u32 func_80017A64 (void);
void func_80017A48 (u32 arg0);
s32 ovl_19_func_800BADAC (s16 arg0);
void func_80017B3C (s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_19_func_800BB08C (s32 arg0, s32 arg1);
s32 func_80011F5C (s32 arg0);
void func_80011FD8 (s32 arg0);
void func_8001585C (ObjectState *obj, s8 arg1);

static __inline__ s16 display_value(s16 *data, s32 row, s32 column) {
    return data[row * 4 + column];
}

void ovl_19_func_800BB108(void) {
    u32 sp18;
    u8 *base;
    u8 *object_base;
    ObjectState *const obj = (ObjectState *) D_800BF660;
    s32 values;
    u8 *state;
    s32 *first;
    u8 *text;
    s32 y;
    s32 rowY;
    s32 next;
    s32 more;
    s32 index;
    s32 var_a1;
    s32 var_a3;
    s32 surface;
    s32 width;
    s32 height;
    s32 column;
    s32 remaining;
    u8 *counter;
    s32 var_s1;

    func_80015840((ObjectState *) obj, 0);
    var_a1 = (s32) obj;
    var_s1 = 0;
    counter = (u8 *) obj;
    counter -= 0x1A0;
    values = (s32) counter;
    index = (s32) D_8005E3C0;
    surface = ((struct_8005E3C0 *) index)->field_D8;
    base = counter + 0x1A4;
    width = *base++;
    state = (u8 *) values + 0x1A5;
    height = *state;
    func_80015EE8(surface + 0x18, values + 0x1A0, width, height, 0, 0);
    column = 0;
    index = func_80017A64();
    sp18 = index;
    surface = 1;
    func_80017A48(surface);
    first = &D_8007AE58;
    text = (u8 *) D_800BCD68;
    do {
        next = var_s1 + 1;
        rowY = 60 << 16;
        y = 57;
        state = (u8 *) (values + 0x160);
        do {
            if (var_s1 == 0 && column == 0) {
                var_a1 = *first;
            } else {
                var_a3 = ovl_19_func_800BADAC(((s16 *) state)[var_s1 * 4 + column]);
                var_a1 = (s32) (text + var_a3);
            }
            func_80017B3C(D_8005E3C0->field_D8 + 0xC, var_a1, (s16) (var_s1 * 200 + 21), column > 0 ? rowY >> 16 : (s16) y);
            y += 45;
            rowY += 45 << 16;
            column++;
        } while (column < 4);
        more = next < 2;
        var_s1 = next;
        column = 0;
    } while (more);
    func_80017A48(sp18);
    {
    auto void nested_BB08C(s32 arg0, s32 arg1) __asm__("ovl_19_func_800BB08C");
    counter = (u8 *) D_800BF4C0;
    if (((s16 *) counter)[4] > 0) {
        nested_BB08C(1, 0);
        nested_BB08C(4, 0);
        nested_BB08C(5, 0);
        nested_BB08C(7, 0);
    }
    if (((s16 *) counter)[4] >= 2) {
        nested_BB08C(1, 1);
        nested_BB08C(7, 1);
    }
    }
}
```
