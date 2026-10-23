# ovl_11_func_80104EC8 — resolved with matching C

**Current status:** Matching resumed at the user's request. The active C now
matches all 1982 relocated words, with residual `[0, 0, 0, 0]` and no unresolved
relocations. The evidence-gated override is **only** `-fno-cse-skip-blocks`;
the earlier two-flag proposal was not adopted. Its evidence comment and policy
audit entry accompany the source. No assembly/register-binding exception is
needed, and the source-policy gate passes.

**Build verification:** `make -j8 check-all` passes for the PS-X EXE and all 13
overlays. The jump-table attribution was regenerated from the compiled source.
See [the matching and flag evidence](../research/ovl_11_func_80104EC8-cse-and-return-sharing.md).

## Historical parking record

The following records the earlier user-requested parking and withdrawn request.
Its measurements and preserved source are historical, not the active solution.

- **Parked:** 2026-10-20T18:49:42.517Z
- **Original reason (withdrawn):** asm-needs-human-approval
- **Current reason:** user-requested parking; matching remains unfinished
- **Escalation reached:** gpt-6.1-sol
- **Source:** `src/overlays/ovl_11/ovl_11_func_80104EC8.c` (INCLUDE_ASM restored)
- **Preserved attempt:** preserved the best measured program ([1, 2479, 3, 161] at 1466/1927 words, measured 2026-10-20T18:42:57, first scored from src/overlays/ovl_11/ovl_11_func_80104EC8.c) rather than the source left on disk, which measured [1, 2560, 1, 152] at 1490/1939 words, measured 2026-10-20T18:48:26

## Historical approval request (withdrawn)

The matching attempt used an unapproved compiler override and failed the source
policy gate. That does not establish that the override is necessary. Future
matching work should start from the preserved C under approved flags, retaining
the experimental results only as conditional evidence. No allowlist change is
needed to leave this function parked.

## Policy findings

- `configs/flag_overrides.mk:718` — **flag-override** — Per-function compiler flag override is not allowlisted
  `CC1FLAGS_ovl_11_func_80104EC8 := -fno-cse-follow-jumps -fno-cse-skip-blocks`
- `configs/flag_overrides.mk:718` — **flag-override** — New per-function compiler flag overrides are forbidden
  `CC1FLAGS_ovl_11_func_80104EC8 := -fno-cse-follow-jumps -fno-cse-skip-blocks`

## Oracle report for the preserved attempt

```
Oracle: ovl_11_func_80104EC8 verdict MISMATCH — 1466/1985 words (73.9%).
Instruction count delta versus target: +3.
Residual (steer by this, not the word count): control-flow 1, population 2479, schedule 3, allocation 161.
The two programs do not contain the same instructions, so no allocation or scheduling reading applies yet — fix the semantics first.
Next block: 250 (0x80106154) — population 1, schedule 0, allocation 0. the instruction populations differ here; nothing below can be read until they agree
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 6 distinct measurements since the residual last improved on [1, 2479, 3, 161]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain.
```

## Preserved attempt

```c
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 ovl_11_func_800D5C3C (s16 arg0);
s32 ovl_11_func_800DADB0 (s16 *arg0, s16 *arg1, s32 arg2, Ovl11D124Entry *arg3, s32 arg4);
s32 ovl_11_func_800E6EE0 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800EC354 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 func_8001AF44 (u32 arg0);
s32 ovl_11_func_800EF0E8 (s16 arg0);
s32 ovl_11_func_800EEC1C (s16 id, s16 which, s16 last, s32 amount);
s32 ovl_11_func_800CFB20 (s32 arg0, s32 arg1);
s32 ovl_11_func_800E9D28 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800C1224 (s16 arg0, s16 arg1);
u32 Rand (s32 arg0);
s32 pow_int (s32 arg0, s32 arg1);
s32 ovl_11_func_800CB4E0 (void);
void ovl_11_func_800CBD78 (Ovl11C7010Object *arg0);
void ovl_11_func_800D666C (s16 arg0);
void ovl_11_func_800D7328 (s16 *arg0);
s32 ovl_11_func_800EF8BC (s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800EFA1C (s16 arg0);

s32 func_80017F30(u16 *arg0, u16 *arg1, u16 *arg2); 


s32 ovl_11_func_80104EC8(s16 arg0) {
    Ovl11D124Entry record18;
    s16 sp20;
    s16 sp22;
    s16 sp24;
    s16 sp26;
    Ovl11D124Entry record28;
    s16 sp30;
    s16 sp32;
    s16 sp34;
    s16 sp36;
    Ovl11D124Entry record38;
    s16 sp40;
    s16 sp42;
    s16 *var_a0_2;
    s16 *var_a1;
    s16 *var_a3;
    s16 *var_v1_5;
    s16 temp_a1_2;
    s16 temp_s0_10;
    s16 temp_s0_11;
    s16 temp_s0_12;
    s16 temp_s0_13;
    s16 temp_s0_2;
    s16 temp_s0_5;
    s16 temp_s0_7;
    s16 temp_s0_8;
    s16 temp_s0_9;
    s16 temp_v1;
    s16 temp_v1_3;
    s16 temp_v1_4;
    s16 temp_v1_5;
    s16 var_a0;
    s16 var_a0_11;
    s16 var_a0_13;
    s16 var_a0_6;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s1;
    s32 temp_a0;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_s0;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_6;
    s32 temp_v0;
    s32 temp_v1_2;
    s32 var_a0_12;
    s32 var_a0_7;
    s32 var_a0_8;
    s32 var_a0_9;
    s32 var_a1_2;
    s32 var_a1_3;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_a2_4;
    s32 var_a2_5;
    s32 var_a2_6;
    s32 var_a3_2;
    s32 var_a3_3;
    s32 var_s4;
    s32 var_v0;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_5;
    s32 var_v0_6;
    u16 *var_s5;
    u16 *var_v1_7;
    u16 temp_v0_2;
    u16 var_a0_3;
    s32 var_v1_6;
    u32 var_a0_10;
    u32 var_a0_4;
    u32 var_a0_5;
    u32 var_v0_2;
    void *temp_a1;
    void *temp_v0_3;
    void *temp_v0_4;
    void *var_v1;
    void *var_v1_2;
    void *var_v1_3;
    void *var_v1_4;

    switch (arg0) {                                 
    case 0x0:                                       
        if (((Ovl11ConditionState *) D_8006C838)->field_929C == 0) {
            temp_v1 = ((Ovl11ConditionWork *) &D_8007AFF0)->otherHeader->kind;
            if ((temp_v1 == 0x30) || (temp_v1 == 0x2A)) {
                goto success;
            }
        }
        goto failure;
    case 0x1:                                       
    case 0x2:                                       
        return (s32) ((u8) ((Ovl11ConditionState *) D_8006C838)->field_92D1 >> 7);
    case 0x3:                                       
        var_a0 = 0xFA;
        var_a2 = 1;
block_192:
        return ovl_11_func_800E6EE0(var_a0, 1, var_a2, 0);
    case 0x4:                                       
        var_a1_2 = 0;
        for (var_a0_7 = 0; var_a0_7 < 10; var_a0_7++) {
            if (((Ovl11ConditionState *) D_8006C838)->items[var_a0_7].id != 0 ||
                (((Ovl11ConditionState *) D_8006C838)->items[var_a0_7].flags & 0x40000)) {
                var_a1_2++;
            }
        }
        if (((Ovl11ConditionState *) D_8006C838)->field_44D2 == 0) {
            if (var_a1_2 < 5) {
                goto success;
            } else {
                goto failure;
            }
        } else {
            if (var_a1_2 < 10) {
                goto success;
            } else {
                goto failure;
            }
        }
    case 0x5:
        for (var_a0_8 = 0; var_a0_8 < 10; var_a0_8++) {
            if ((((Ovl11ConditionState *) D_8006C838)->items[var_a0_8].flags & 0x40000000) &&
                ((Ovl11ConditionState *) D_8006C838)->items[var_a0_8].id == 0x10A) {
                break;
            }
        }
        if (var_a0_8 != 10) {
            if (((Ovl11ConditionState *) D_8006C838)->date[2] !=
                ((Ovl11ConditionState *) D_8006C838)->items[var_a0_8].tile) {
                return func_80017F30(((Ovl11ConditionState *) D_8006C838)->items[var_a0_8].name,
                    (u16 *) ((u8 *) D_80051F26 + *D_80054BC0),
                    (u16 *) ((u8 *) D_80051F26 + 0xE + *D_80054BC0)) != 0;
            }
        }
        goto failure;
    case 0x26: case 0x2B: case 0x2C: case 0x2D: case 0x2E:
    case 0x4D: case 0x5D: case 0x5E: case 0x60:
        goto success;
    case 0x6:
        for (var_a0_9 = 0; var_a0_9 < 10; var_a0_9++) {
            if ((((Ovl11ConditionState *) D_8006C838)->items[var_a0_9].flags & 0x40000000) &&
                ((Ovl11ConditionState *) D_8006C838)->items[var_a0_9].id == 0x10A) {
                break;
            }
        }
        if (var_a0_9 != 10) {
            if (((Ovl11ConditionState *) D_8006C838)->date[2] !=
                ((Ovl11ConditionState *) D_8006C838)->items[var_a0_9].tile) {
                return func_80017F30(((Ovl11ConditionState *) D_8006C838)->items[var_a0_9].name,
                    (u16 *) ((u8 *) D_80051F26 + *D_80054BC0),
                    (u16 *) ((u8 *) D_80051F26 + 0xE + *D_80054BC0)) == 0;
            }
        }
        goto failure;
    case 0x7:                                       
        record18.unk0 = 0x31;
        record18.unk4 = ovl_11_func_800D5C3C(0x31);
        var_a0_2 = &sp20;
        if (ovl_11_func_800DADB0(&sp20, &sp22, 1, &record18, 2) == 0) {
            var_a1 = &sp22;
            var_a2_2 = 0;
            var_a3 = &record18.unk0;
            goto block_282;
        }
        goto success;
    case 0x8:                                       
        var_a0 = 0x104;
        var_a2 = 1;
        return ovl_11_func_800E6EE0(var_a0, 1, var_a2, 0);
    case 0x9:
        temp_v0 = ovl_11_func_800E6EE0(0x103, 1, 1, 0);
        if (temp_v0 == 1) {
            if (((Ovl11ConditionState *) D_8006C838)->field_E6CB ==
                ((Ovl11ConditionState *) D_8006C838)->date[0] + 1) {
                if (ovl_11_func_800EC354(7, 0, -1, -1) == temp_v0) {
                    return 1;
                }
            }
        }
        return 0;
    case 0xA:                                       
        if (((Ovl11ConditionState *) D_8006C838)->field_5492 != -1) {
            if (((Ovl11ConditionState *) D_8006C838)->field_5492 != 0x14) {
success:
                return 1;
            }
            goto block_45;
        }
block_45:
        var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[20].amount;
        switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
        case 0: var_v0_2 = 0x4E20; break;
        case 1: var_v0_2 = 0x9C40; break;
        default: var_v0_2 = 0xC350; break;
        }
block_64:
        return var_a0_3 < var_v0_2;
    case 0xB:                                       
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0x14))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[20].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
        case 0: var_v0_2 = 0x4E20; break;
        case 1: var_v0_2 = 0x9C40; break;
        default: var_v0_2 = 0xC350; break;
        }
            return var_a0_3 < var_v0_2;
        }
        
        return var_v0;
    case 0xC:                                       
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0x14))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[20].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
        case 0: var_v0_2 = 0x4E20; break;
        case 1: var_v0_2 = 0x9C40; break;
        default: var_v0_2 = 0xC350; break;
        }
            return var_a0_3 < var_v0_2;
        }
        
        return var_v0;
    case 0xD:                                       
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0x14)) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[20].amount;
            var_v0_2 = 0xC350;
            return var_a0_3 < var_v0_2;
        }
        goto success;
    case 0xE:                                       
        if (func_8001AF44(0x192U) == 0) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_52C6 == ((Ovl11ConditionState *) D_8006C838)->actors[11].field46) {
                goto block_68;
            }
            
            return var_v0;
        }
block_68:
        if (((Ovl11ConditionState *) D_8006C838)->date[0] == 0) {
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 2) {
                if ((u32) (((Ovl11ConditionState *) D_8006C838)->field_44BE - 0xE) < 5U) {
                    goto failure;
                }
            }
        }
block_72:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x7530; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
        } else {
            
            return var_v0;
        }
        break;
    case 0xF:                                       
        if (func_8001AF44(0xE6U) != 0) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7))) {
                var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
                switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x7530; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
            } else {
                
                return var_v0;
            }
        } else {
            goto failure;
        }
        break;
    case 0x10:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x7530; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
        } else {
            
            return var_v0;
        }
        break;
    case 0x11:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7)) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
            var_v0_2 = 0xEA60;
            return var_a0_3 < var_v0_2;
        }
        goto success;
    case 0x12:                                      
        var_a0_4 = 0xE6;
block_406:
        return func_8001AF44(var_a0_4) != 1;
    case 0x13:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 3))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[3].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x4E20; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
        } else {
            
            return var_v0;
        }
        break;
    case 0x14:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 3))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[3].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x4E20; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
        } else {
            
            return var_v0;
        }
        break;
    case 0x15:
        if (((Ovl11ConditionState *) D_8006C838)->field_5492 == -1 ||
            ((Ovl11ConditionState *) D_8006C838)->field_5492 == 3) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[3].amount;
            if (((Ovl11ConditionState *) D_8006C838)->date[0] == 1) {
                if (var_a0_3 < 40000) {
                    goto success;
                }
            } else {
                if (var_a0_3 < 60000) {
                    if (((Ovl11ConditionState *) D_8006C838)->actors[20].flags & 8) {
                        goto success;
                    }
                }
            }
            goto failure;
        }
        goto success;
    case 0x16:                                      
        var_v0 = 1;
        if (func_8001AF44(0x20U) != 1) {
            if (func_8001AF44(0x25U) != 0) {
                if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 3)) {
                    return (u16) ((Ovl11ConditionState *) D_8006C838)->actors[3].amount <= 0xEA5FU;
                }
                goto success;
            }
            goto failure;
        }
        
        return var_v0;
    case 0x17:                                      
        var_v0 = 0;
        if (ovl_11_func_800EF0E8(0) == 2) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 3)) {
                var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[3].amount;
                var_v0_2 = 0xEA60;
                return var_a0_3 < var_v0_2;
            }
            goto success;
        }
        
        return var_v0;
    case 0x18:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 9))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[9].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x4E20; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
        } else {
            
            return var_v0;
        }
        break;
    case 0x19:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 9))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[9].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x4E20; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
        } else {
            
            return var_v0;
        }
        break;
    case 0x1B:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x4E20; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
        } else {
            
            return var_v0;
        }
        break;
    case 0x1C:                                      
        if ((u32) (((Ovl11ConditionState *) D_8006C838)->field_44C0 - 0xA) < 8U) {
            var_v0 = 0;
            if (((u32) (((Ovl11ConditionState *) D_8006C838)->field_44C0 - 0xD) >= 3U) && ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE)))) {
                var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
                switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x4E20; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
            } else {
                
                return var_v0;
            }
        } else {
            goto failure;
        }
        break;
    case 0x1D:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE))) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: var_v0_2 = 0x4E20; break;
            case 1: var_v0_2 = 0x9C40; break;
            default: var_v0_2 = 0xEA60; break;
            }
            return var_a0_3 < var_v0_2;
        } else {
            
            return var_v0;
        }
        break;
    case 0x1E:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE)) {
            var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
            var_v0_2 = 0xEA60;
            return var_a0_3 < var_v0_2;
        }
        goto success;
    case 0x1F:                                      
        if (((Ovl11ConditionState *) D_8006C838)->field_929C == 0) {
            record18.unk0 = 0x36;
            record18.unk4 = 0;
            temp_s0_3 = ovl_11_func_800DADB0(&sp24, &sp26, 1, &record18, 0xE);
            temp_s0_4 = temp_s0_3 + ovl_11_func_800DADB0(&sp24, &sp26, 0, &record18, 0xE);
            temp_s0 = temp_s0_4 - ovl_11_func_800DADB0(&sp24, &sp26, 1, &record18, 0xA);
            return (temp_s0 - ovl_11_func_800DADB0(&sp24, &sp26, 0, &record18, 0xA)) >= 0xC8;
        }
        goto failure;
    case 0x20:                                      
        var_a0_5 = 0x73;
block_172:
        return func_8001AF44(var_a0_5);
    case 0x21:                                      
        if (ovl_11_func_800E6EE0(0x47, 1, 1, 0) != 1) {
            record28.unk0 = 0x28;
            record28.unk4 = ovl_11_func_800D5C3C(0x28);
            var_v0 = 1;
            if (ovl_11_func_800DADB0(&sp30, &sp32, 1, &record28, 2) == 0) {
                if (ovl_11_func_800DADB0(&sp30, &sp32, 0, &record28, 2) == 0) {
block_323:
                    return (s32) ((u8) ((Ovl11ConditionState *) D_8006C838)->field_91E1 >> 7);
                }
                goto success;
            }
            
            return var_v0;
        }
        goto success;
    case 0x23:
        for (var_a2_3 = 0; var_a2_3 < 7; var_a2_3++) {
            if (((Ovl11ConditionState *) D_8006C838)->animals[var_a2_3].kind != 0x28 ||
                (((Ovl11ConditionState *) D_8006C838)->animals[var_a2_3].flags & 0x20)) {
                goto failure;
            }
        }
        for (var_a2_4 = 0; var_a2_4 < 7; var_a2_4++) {
            if (((Ovl11ConditionState *) D_8006C838)->animals[var_a2_4].amount >= 0x50) {
                goto success;
            }
        }
        goto failure;
    case 0x24:
        if (D_80071CCA == 7) {
            if (func_8001AF44(0x1B) == 0) {
                return 0;
            }
        }
        return 1;
    case 0x25:                                      
        var_s0 = 0;
        var_s5 = D_801274D0;
        var_s4 = 0;
        do {
            temp_v0_2 = *var_s5;
            var_s5 += 1;
            var_s4 += 1;
            record28.unk0 = temp_v0_2;
            record28.unk4 = ovl_11_func_800D5C3C((s16) temp_v0_2);
            temp_s0_5 = var_s0 + ovl_11_func_800DADB0(&sp34, &sp36, 1, &record28, 0xA);
            var_s0 = temp_s0_5 + ovl_11_func_800DADB0(&sp34, &sp36, 0, &record28, 0xA);
        } while (var_s4 < 5);
        return var_s0 >= 0x5A;
    case 0x27:                                      
        var_a0 = 0xCC;
        var_a2 = 0;
        return ovl_11_func_800E6EE0(var_a0, 1, var_a2, 0);
    case 0x28:                                      
        var_a0_10 = 0xE8;
block_338:
        var_v0_3 = func_8001AF44(var_a0_10) ^ 1;
block_339:
        return var_v0_3 == 0;
    case 0x29:
        var_a3_2 = 0;
        for (var_a1_3 = 0; (((Ovl11ConditionState *) D_8006C838)->field_44D2 == 0 ? var_a1_3 < 5 : var_a1_3 < 10); var_a1_3++) {
            if (((Ovl11ConditionState *) D_8006C838)->items[var_a1_3].id == 0 &&
                !(((Ovl11ConditionState *) D_8006C838)->items[var_a1_3].flags & 0x02000000)) {
                var_a3_2++;
            }
        }
        return var_a3_2 >= 5;
    case 0x2A:
        for (var_a2_5 = 0; (((Ovl11ConditionState *) D_8006C838)->field_44D2 == 0 ? var_a2_5 < 5 : var_a2_5 < 10); var_a2_5++) {
            if (((Ovl11ConditionState *) D_8006C838)->items[var_a2_5].id != 0 &&
                (((Ovl11ConditionState *) D_8006C838)->items[var_a2_5].flags & 0x40000) &&
                ((Ovl11ConditionState *) D_8006C838)->items[var_a2_5].value < 0x32) {
                return 0;
            }
        }
        return 1;
    case 0x2F:                                      
        return (u32) ((u16) ((Ovl11ConditionWork *) &D_8007AFF0)->otherHeader->kind - 0x24) >= 2U;
    case 0x30:
        var_a3_3 = 0;
        for (var_a2_6 = 0; (((Ovl11ConditionState *) D_8006C838)->field_44D2 == 0 ? var_a2_6 < 5 : var_a2_6 < 10); var_a2_6++) {
            if (((Ovl11ConditionState *) D_8006C838)->items[var_a2_6].id == 0x109) {
                var_a3_3++;
            }
        }
        return var_a3_3 >= 3;
    case 0x31:                                      
        if (((Ovl11ConditionState *) D_8006C838)->field_E641 != 0) {
            temp_s0_6 = ovl_11_func_800EEC1C(0x122, 1, 1, 0);
            temp_s0_7 = temp_s0_6 + ovl_11_func_800EEC1C(0x123, 1, 1, 0);
            temp_s0_8 = temp_s0_7 + ovl_11_func_800EEC1C(0x124, 1, 1, 0);
            temp_s0_2 = temp_s0_8 + ovl_11_func_800EEC1C(0xE8, 1, 1, 0);
            return ((temp_s0_2 + ovl_11_func_800EEC1C(0x125, 1, 1, 0)) << 0x10) > 0x20000;
        }
        goto failure;
    case 0x32:                                      
        if (D_80071CCA == 0x14) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        goto failure;
    case 0x33:                                      
        if (D_80071CCA == 7) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        goto failure;
    case 0x34:                                      
        if (D_80071CCA == 3) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        goto failure;
    case 0x35:                                      
        if (D_80071CCA == 9) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        goto failure;
    case 0x36:                                      
        if (D_80071CCA == 0xE) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        goto failure;
    case 0x37:                                      
        return ((Ovl11ConditionState *) D_8006C838)->field_E642 != 0x59;
    case 0x38:                                      
        return ((Ovl11ConditionState *) D_8006C838)->field_E670 != 0;
    case 0x39:                                      
        var_v0_3 = ovl_11_func_800CFB20(7, 3) ^ 1;
        return var_v0_3 == 0;
    case 0x3D:                                      
        return ((Ovl11ConditionState *) D_8006C838)->actors[0].field14 >= 0x32;
    case 0x3E:                                      
        var_v0 = 0;
        if ((D_80070D30 & 0xFC) == 0xFC) {
            temp_v1_3 = ((Ovl11ConditionWork *) &D_8007AFF0)->otherHeader->kind;
            if ((temp_v1_3 != 0xC) && (temp_v1_3 != 0x22)) {
                goto failure;
            }
            goto success;
        }
        
        return var_v0;
    case 0x3F:
        if (D_80071CCA == 3) {
            if (func_8001AF44(0x4E) == 1) {
                goto failure;
            }
            if (func_8001AF44(0x4D) == 1) {
                return 0;
            }
        }
        return 1;
    case 0x40:                                      
        var_a0_4 = 0x4D;
        return func_8001AF44(var_a0_4) != 1;
    case 0x41:                                      
        if (func_8001AF44(0x4DU) != 1) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_E798 == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                var_a0_11 = ((Ovl11ConditionState *) D_8006C838)->field_E79A;
block_258:
                var_v0 = 1;
                if (var_a0_11 != ((Ovl11ConditionState *) D_8006C838)->date[2]) {
                    goto failure;
                }
                
                return var_v0;
            }
            
            return var_v0;
        }
        goto failure;
    case 0x42:                                      
        if (func_8001AF44(0x4DU) != 1) {
            temp_v0_3 = (void *) ((u8 *) D_8006C838 + (((Ovl11ConditionState *) D_8006C838)->field_5492 * 0x1D4));
            var_v0 = 0;
            if (((Ovl11ConditionState *) temp_v0_3)->actors[0].location == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                var_a0_11 = ((Ovl11ConditionState *) temp_v0_3)->actors[0].tile;
                goto block_258;
            }
            
            return var_v0;
        }
        goto failure;
    case 0x43:                                      
        if (func_8001AF44(0x4DU) != 1) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->actors[36].location == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                var_a0_11 = ((Ovl11ConditionState *) D_8006C838)->actors[36].tile;
                goto block_258;
            }
            
            return var_v0;
        }
        goto failure;
    case 0x44:                                      
        if (func_8001AF44(0x4DU) != 1) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_5246 == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                var_a0_11 = ((Ovl11ConditionState *) D_8006C838)->field_5248;
                goto block_258;
            }
            
            return var_v0;
        }
        goto failure;
    case 0x45:                                      
        return (u32) (((Ovl11ConditionState *) D_8006C838)->field_E770 - 1) < 7U;
    case 0x46:                                      
        var_v0 = 0;
        if ((u16) ((Ovl11ConditionState *) D_8006C838)->field_E770 >= 0x3CU) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5246 != ((Ovl11ConditionState *) D_8006C838)->date[1]) || (((Ovl11ConditionState *) D_8006C838)->field_5248 != ((Ovl11ConditionState *) D_8006C838)->date[2])) {
                temp_v0_4 = (void *) ((u8 *) D_8006C838 + (((Ovl11ConditionState *) D_8006C838)->field_5492 * 0x1D4));
                var_v0 = 1;
                if (((Ovl11ConditionState *) temp_v0_4)->actors[0].location == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                    var_v0 = 0;
                    if (((Ovl11ConditionState *) temp_v0_4)->actors[0].tile != ((Ovl11ConditionState *) D_8006C838)->date[2]) {
                        goto success;
                    }
                }
                
                return var_v0;
            }
            goto failure;
        }
        
        return var_v0;
    case 0x47:                                      
        return (u32) (((Ovl11ConditionState *) D_8006C838)->field_E772 - 1) < 2U;
    case 0x48:                                      
        return (u16) ((Ovl11ConditionState *) D_8006C838)->field_E772 >= 3U;
    case 0x49:                                      
        var_v0 = 0;
        if (((Ovl11ConditionState *) D_8006C838)->field_E77A >= 0x1E) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_E770 == 0) {
                var_v0 = 0;
                if (((Ovl11ConditionState *) D_8006C838)->field_E76E == 0) {
                    var_v1_6 = ((Ovl11ConditionState *) D_8006C838)->field_E772;
                    var_v0 = 1;
                    goto block_396;
                }
            }
        }
        
        return var_v0;
    case 0x4A:                                      
        return (u16) ((Ovl11ConditionState *) D_8006C838)->field_E76E >= 0xAU;
    case 0x4B:                                      
        if ((((Ovl11ConditionState *) D_8006C838)->actors[0].kind == 0) || (((Ovl11ConditionState *) D_8006C838)->actors[0].kind == 9)) {
            return ((Ovl11ConditionState *) D_8006C838)->field_E657 != 0;
        }
        goto failure;
    case 0x4C:                                      
        record38.unk0 = 0x31;
        record38.unk4 = ovl_11_func_800D5C3C(0x31);
        var_v0 = 1;
        if (ovl_11_func_800DADB0(&sp40, &sp42, 1, &record38, 2) == 0) {
            record38.unk0 = 0x32;
            record38.unk4 = ovl_11_func_800D5C3C(0x32);
            var_v0 = 1;
            if (ovl_11_func_800DADB0(&sp40, &sp42, 1, &record38, 2) == 0) {
                record38.unk0 = 0x178;
                record38.unk4 = ovl_11_func_800D5C3C(0x178);
                var_v0 = 1;
                if (ovl_11_func_800DADB0(&sp40, &sp42, 1, &record38, 2) == 0) {
                    record38.unk0 = 0x179;
                    record38.unk4 = ovl_11_func_800D5C3C(0x179);
                    if (ovl_11_func_800DADB0(&sp40, &sp42, 1, &record38, 2) == 0) {
                        record38.unk0 = 0x34;
                        var_a0_2 = &sp40;
                        var_a1 = &sp42;
                        var_a2_2 = 1;
                        var_a3 = &record38.unk0;
                        record38.unk4 = ovl_11_func_800D5C3C(0x34);
                        block_282:
            return ovl_11_func_800DADB0(var_a0_2, var_a1, var_a2_2, (Ovl11D124Entry *) var_a3, 2) != 0;
                    }
                    goto success;
                }
            }
        }
        
        return var_v0;
    case 0x4F:                                      
        return D_80071A5C >= 0x1F4;
    case 0x50:
        if (!(((Ovl11ConditionState *) D_8006C838)->field_44F8 & 0x200000)) {
            if (((Ovl11ConditionState *) D_8006C838)->field_5224 >= 0x7530) {
                for (var_a0_12 = 0; var_a0_12 < 37; var_a0_12++) {
                    if (((Ovl11ConditionState *) D_8006C838)->actors[var_a0_12].amount >= 50000) {
                        goto success;
                    }
                }
            }
        }
        goto failure;
    case 0x51:                                      
        var_v0 = 0;
        if (((Ovl11ConditionState *) D_8006C838)->field_5224 >= 0x64) {
            var_v0 = 0;
            if (!(((Ovl11ConditionState *) D_8006C838)->field_44F8 & 0x02000000)) {
                var_v0 = 1;
                if (((Ovl11ConditionState *) D_8006C838)->field_91AC != 0x106) {
                    goto failure;
                }
            }
        }
        
        return var_v0;
    case 0x52:                                      
        var_v0 = 0;
        if (((Ovl11ConditionState *) D_8006C838)->field_44CE == 2) {
            if (((Ovl11ConditionState *) D_8006C838)->field_5224 >= 0x1388) {
                return (u8) ((Ovl11ConditionState *) D_8006C838)->field_E6CA < 4U;
            }
            goto failure;
        }
        
        return var_v0;
    case 0x53:                                      
        var_s1 = 0;
        temp_a1_2 = (s16) D_80070CF0 % 9;
        var_s0_2 = 0;
        switch (temp_a1_2) {                        
        case 0:                                     
            var_s1 = 0x41;
            break;
        case 1:                                     
            var_s1 = 0x47;
            break;
        case 2:                                     
            var_s1 = 0x43;
            break;
        case 3:                                     
            var_s1 = 0x4D;
            break;
        case 4:                                     
            temp_s0_9 = ovl_11_func_800EEC1C(0x122, 1, 1, 0);
            temp_s0_10 = temp_s0_9 + ovl_11_func_800EEC1C(0x123, 1, 1, 0);
            temp_s0_11 = temp_s0_10 + ovl_11_func_800EEC1C(0x124, 1, 1, 0);
            var_s0_3 = temp_s0_11 + ovl_11_func_800EEC1C(0xE8, 1, 1, 0);
            var_a0_13 = 0x125;
block_305:
            var_s0_2 = var_s0_3 + ovl_11_func_800EEC1C(var_a0_13, 1, 1, 0);
            break;
        case 5:                                     
            temp_s0_12 = ovl_11_func_800EEC1C(0x8B, 1, 1, 0);
            temp_s0_13 = temp_s0_12 + ovl_11_func_800EEC1C(0x8C, 1, 1, 0);
            var_s0_3 = temp_s0_13 + ovl_11_func_800EEC1C(0x8D, 1, 1, 0);
            var_a0_13 = 0xE3;
            goto block_305;
        case 6:                                     
            var_s1 = 0xA2;
            break;
        case 7:                                     
            var_s1 = 0x44;
            break;
        default:                                    
            var_s1 = 0x97;
            break;
        }
        if (var_s0_2 < 3) {
            if (var_s1 != 0) {
                var_v0 = 1;
                if (ovl_11_func_800EEC1C(var_s1, 1, 1, 0) < 3) {
                    goto failure;
                }
                
                return var_v0;
            }
            goto failure;
        }
        goto success;
    case 0x54:                                      
        var_a0_10 = 0x67;
        return func_8001AF44(var_a0_10) == 1;
    case 0x55:                                      
        var_a0_10 = 0x68;
        return func_8001AF44(var_a0_10) == 1;
    case 0x56:                                      
        var_a0_10 = 0x69;
        return func_8001AF44(var_a0_10) == 1;
    case 0x57:                                      
        var_a0_10 = 0x6A;
        return func_8001AF44(var_a0_10) == 1;
    case 0x58:                                      
        var_a0_10 = 0x6B;
        return func_8001AF44(var_a0_10) == 1;
    case 0x59:                                      
        var_a0_10 = 0x6C;
        return func_8001AF44(var_a0_10) == 1;
    case 0x5A:                                      
        var_a0_10 = 0x6D;
        return func_8001AF44(var_a0_10) == 1;
    case 0x5B:                                      
        var_a0_10 = 0x6E;
        return func_8001AF44(var_a0_10) == 1;
    case 0x5C:                                      
        var_a0_10 = 0x6F;
        return func_8001AF44(var_a0_10) == 1;
    case 0x22:                                      
    case 0x5F:                                      
        return (s32) (((Ovl11ConditionState *) D_8006C838)->field_91E1 >> 7);
    case 0x61:                                      
        if ((ovl_11_func_800E9D28(0xD, 1, 0, 0) != 1) && (ovl_11_func_800E9D28(0xD, 1, 1, 0) != 1) && (ovl_11_func_800E9D28(0xD, 1, 2, 0) != 1)) {
            var_v0_3 = ovl_11_func_800E9D28(0xD, 1, 3, 0) ^ 1;
            return var_v0_3 == 0;
        }
        goto success;
    case 0x62:                                      
        var_a0_10 = 0x2A;
        return func_8001AF44(var_a0_10) == 1;
    case 0x63:                                      
        var_a0_10 = 0x2A;
        return func_8001AF44(var_a0_10) == 1;
    case 0x64:                                      
        var_a0_10 = 0x27;
        return func_8001AF44(var_a0_10) == 1;
    case 0x65:                                      
        var_a0_10 = 0x26;
        return func_8001AF44(var_a0_10) == 1;
    case 0x66:                                      
        var_a0_10 = 0x28;
        return func_8001AF44(var_a0_10) == 1;
    case 0x67:                                      
        var_a0_10 = 0x29;
        return func_8001AF44(var_a0_10) == 1;
    case 0x68:                                      
        var_a0_4 = 0x38;
        if (D_80070D06 == 0) {
            return func_8001AF44(var_a0_4) != 1;
        }
        goto failure;
    case 0x69:                                      
        var_v0 = 1;
        if (func_8001AF44(0x15FU) != 0) {
            var_a0_10 = 0x164;
            if (func_8001AF44(0x163U) != 1) {
                return func_8001AF44(var_a0_10) == 1;
            }
            goto success;
        }
        
        return var_v0;
    case 0x6A:                                      
        var_v0 = 0;
        if (func_8001AF44(0x26U) != 1) {
            var_v0 = 0;
            if (func_8001AF44(0x27U) != 1) {
                var_v0 = 0;
                if (func_8001AF44(0x28U) != 1) {
                    var_a0_4 = 0x2A;
                    if (func_8001AF44(0x29U) != 1) {
                        return func_8001AF44(var_a0_4) != 1;
                    }
                    goto failure;
                }
            }
        }
        
        return var_v0;
    case 0x6B:                                      
        var_v0 = 0;
        if (ovl_11_func_800EF0E8(1) == 2) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7)) {
                var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
                var_v0_2 = 0xEA60;
                return var_a0_3 < var_v0_2;
            }
            goto success;
        }
        
        return var_v0;
    case 0x6C:                                      
        var_a0_6 = 4;
block_355:
        return ovl_11_func_800EF0E8(var_a0_6) == 2;
    case 0x6D:                                      
        var_v0 = 0;
        if (ovl_11_func_800EF0E8(3) == 2) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE)) {
                var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
                var_v0_2 = 0xEA60;
                return var_a0_3 < var_v0_2;
            }
            goto success;
        }
        
        return var_v0;
    case 0x6E:                                      
        var_a0_6 = 0;
        return ovl_11_func_800EF0E8(var_a0_6) == 2;
    case 0x6F:                                      
        var_v0 = 0;
        if (ovl_11_func_800EF0E8(2) == 2) {
        case 0x1A:                                  
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 9)) {
                var_a0_3 = ((Ovl11ConditionState *) D_8006C838)->actors[9].amount;
                var_v0_2 = 0xEA60;
                return var_a0_3 < var_v0_2;
            }
            goto success;
        }
        
        return var_v0;
    case 0x70:                                      
        return ((Ovl11ConditionState *) D_8006C838)->field_929C == 0x15F;
    case 0x71:
        switch (((Ovl11ConditionWork *) &D_8007AFF0)->header->kind) {
        case 0x2A:
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 0) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 7 ||
                    ((Ovl11ConditionState *) D_8006C838)->date[2] == 0x11 ||
                    ((Ovl11ConditionState *) D_8006C838)->date[2] == 0x15) {
                    return 1;
                }
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 1) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 6 ||
                    ((Ovl11ConditionState *) D_8006C838)->date[2] == 0xB) {
                    return 1;
                }
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 2) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 8) {
                    return 1;
                }
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 3) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 9) {
                    return 1;
                }
            }
            return 0;
        case 0x2C:
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 1) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 0) {
                    return 1;
                }
            }
            return 0;
        case 8:
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 1) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 0x13) {
                    return 1;
                }
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 2) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 0x14) {
                    return 1;
                }
            }
            return 0;
        default:
            goto failure;
        }
    case 0x72:                                      
        if (ovl_11_func_800C1224(((Ovl11ConditionState *) D_8006C838)->date[1], ((Ovl11ConditionState *) D_8006C838)->date[2]) != 0) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_E772 != 0) {
                var_v0 = 1;
                if (((Ovl11ConditionState *) D_8006C838)->date[1] == 0) {
                    var_v0 = 1;
                    if (((Ovl11ConditionState *) D_8006C838)->date[2] == 7) {
                        goto failure;
                    }
                }
            }
            
            return var_v0;
        }
        goto failure;
    case 0x73:                                      
        var_v0 = 0;
        if (D_80071A5C >= 0x1F4) {
            var_v0 = 0;
            if (func_8001AF44(0xE7U) == 1) {
                var_v1_6 = ovl_11_func_800E6EE0(0x38, 0, 2, 0);
                var_v0 = 1;
block_396:
                if (var_v1_6 != 0) {
                    var_v0 = 0;
                }
                return var_v0;
            }
        }
        
        return var_v0;
    case 0x74:
        temp_v1_5 = D_80071A22 - 3;
        switch (temp_v1_5) {
        case 0: var_a0_5 = 0x21; break;
        case 4: var_a0_5 = 0x22; break;
        case 6: var_a0_5 = 0x23; break;
        case 11: var_a0_5 = 0x24; break;
        case 17: var_a0_5 = 0x25; break;
        default: goto failure;
        }
        goto block_172;
    case 0x75:                                      
        var_a0_4 = 0x1B;
        return func_8001AF44(var_a0_4) != 1;
    default:
        break;
    }
failure:
    return 0;
}
```
