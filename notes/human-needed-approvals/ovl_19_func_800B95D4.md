# ovl_19_func_800B95D4 — human decision needed

- **Parked:** 2026-10-16T07:03:15.344Z
- **Reason:** escalation-exhausted
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_19/ovl_19_func_800B95D4.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([0, 0, 1, 6] at 196/236 words, measured 2026-10-16T06:21:21, first scored from src/overlays/ovl_19/ovl_19_func_800B95D4.c) rather than the source left on disk, which measured [0, 0, 2, 3] at 194/238 words, measured 2026-10-16T06:55:10

## What the loop needs

Every tier on the escalation ladder returned without a byte-exact match. The function
needs either a new structural hypothesis or a policy decision that the ladder cannot
make on its own. The preserved attempt and the oracle report below are the starting
point.

## Policy findings

- none recorded

## Oracle report for the preserved attempt

```
Oracle: ovl_19_func_800B95D4 verdict MISMATCH — 196/239 words (82%).
Instruction count delta versus target: +1.
Residual (steer by this, not the word count): control-flow 0, population 0, schedule 1, allocation 6.
Next block: 1 (0x800B9618) — population 0, schedule 0, allocation 1. smallest open residual
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 16 distinct measurements since the residual last improved on [0, 0, 1, 6]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 16 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"

s32 func_80012A34(s32 arg0);
s32 func_8001FABC(s16 arg0);
s32 ovl_19_func_800BA2D4(void);
s32 ovl_19_func_800BA33C(u8 *arg0);
s32 ovl_19_func_800BA468(void *arg0, void *arg1);
s32 ovl_19_func_800BA5B4(void *arg0, void *arg1);
s32 ovl_19_func_800BA73C(Ovl19Func800BA73CArg *arg0);
s32 ovl_19_func_800BA750(void *arg0);
void ovl_19_func_800BAC40(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2, s32 arg3);

typedef struct {
    s16 unk0;
    s16 command;
    s16 angle;
    s16 timer;
    s16 value;
    s16 unkA;
    s32 flag;
    s16 rest[28];
} Ovl19StateRecord;

static __inline__ Ovl19StateRecord *actorAt(s16 *base, s32 index) {
    return (Ovl19StateRecord *)base + index;
}

void ovl_19_func_800B95D4(void) {
    s32 i;
    s32 angle;
    s16 *base;
    s16 *q;
    s16 *other;
    s16 *state;
    s16 *origin;
    s16 *view;
    s32 choice;
    s32 status;
    s32 command;
    s32 one;

    i = 0;
    one = 1;
    base = D_800BF4C0 + 8;
    view = base - 8;
    q = base;
    do {
        Ovl19StateRecord *p = actorAt(base, i);

        angle = 0;
        if (i == 0) {
            q = base + 36;
        }
        other = q;
        status = ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)p);
        origin = base - 8;
        if (status == one && origin[i * 32 + 0x51] == 2) {
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)p, 4, p->angle, 30);
            origin[0x101] = func_8001FABC(0x3B);
            origin[0x101] = func_8001FABC(0x44);
        } else {
        if (p->command == 4) {
            state = D_800BF4C0;
            if (p->timer != p->timer / state[0xBB] * state[0xBB]) {
                continue;
            }
            if (p->timer > 0) {
                if (other[1] == 4 && ovl_19_func_800BA468(other, p) == one &&
                    ovl_19_func_800BA468(p, other) == one) {
                    choice = ovl_19_func_800BA2D4();
                    origin = (s16 *)actorAt(base, choice);
                    other = base;
                    if (choice == 0) {
                        other = state + 0x2C;
                    }
                    angle = (s16)(origin[2] + ovl_19_func_800BA5B4(origin, other));
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)origin, 9, (s16)(angle - angle / 4 * 4), state[0xC8]);
                    *(s32 *)(origin + 6) = one;
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)other, 10, other[2], state[0xC9]);
                    *(s32 *)(other + 6) = 0;
                    continue;
                }
            } else {
                if (ovl_19_func_800BA468(p, other) == one) {
                    angle = (s16)(p->angle + ovl_19_func_800BA5B4(p, other));
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)p, 6, (s16)(angle - angle / 4 * 4), state[0xC7]);
                    p->flag = one;
                    command = 7;
                    if (ovl_19_func_800BA73C((Ovl19Func800BA73CArg *)other) == one) {
                        command = 8;
                    }
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)other, command, other[2], state[0xC7]);
                    state[0x101] = func_8001FABC(0x3C);
                } else {
                    ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)p, 5, p->angle, state[0xC6]);
                    p->flag = 0;
                }
                continue;
            }
        }
        if (ovl_19_func_800BA750(p) == one) {
            command = ovl_19_func_800BA33C((u8 *)p);
            switch (command) {
                case 0:
                case 1:
                    angle = (s16)func_80012A34(4);
                    break;
                case 3:
                    angle = view[10];
                    break;
                case 2:
                    if (func_80012A34(101) <= p->value * 100 / 255) {
                        angle = (s16)(view[10] + ovl_19_func_800BA5B4(p, other));
                        angle = (s16)(angle - angle / 4 * 4);
                    } else {
                        angle = p->angle;
                    }
                    break;
            }
            ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)p, command, angle, D_800BF4C0[0xBC]);
        }
        }
    } while (view += 36, q = base, ++i < 2);
}
```
