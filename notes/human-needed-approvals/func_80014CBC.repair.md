# func_80014CBC — repair attempt

`/auto_decompilation_loop repair` did not reach a clean match. `src/func_80014CBC.c` and its allowlist
entry are back as they were at `35d7980e4e74`, so the function still matches.

- Revoked for the attempt: embedded-asm
- Reached tier: gpt-6.1-sol
- Started 2026-10-24T16:05:33.184Z, finished 2026-10-24T16:48:09.122Z
- Last program on disk (local runtime archive): `/home/dave/Documents/code/btn-decompilation/run_output/autoloop/attempts/func_80014CBC.2026-10-24T16-48-09-122Z.repair-final.c`
- Its full change set (local runtime archive): `/home/dave/Documents/code/btn-decompilation/run_output/autoloop/repairs/func_80014CBC.2026-10-24T16-48-09-122Z/changes.patch`

## Constructs the repair had to remove

- src/func_80014CBC.c:57 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("lui %0,%%hi(D_80048B1C)\n\taddiu %0,%0,%%lo(D_80048B1C)" : "=r"(bp) : "r"(arg0))
- src/func_80014CBC.c:62 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("addu %0,%0,%2" : "=r"(eptr) : "0"(bp), "r"(scaled))
- src/func_80014CBC.c:70 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("sw $5,68($sp)" : "=r"(arg3), "=m"(*(s32 *)(eptr + 36)) : "0"(arg3), "r"(arg3), "r"(len), "r"(result))
- src/func_80014CBC.c:83 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("srl %0,%1,0xb" : "=r"(sector_start) : "r"(pos), "r"(arg3))
- src/func_80014CBC.c:84 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("andi %0,%1,0x7ff" : "=r"(pos_mod) : "r"(pos), "r"(sector_start), "r"(arg0))
- src/func_80014CBC.c:90 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__ volatile("" : "=r"(sector_start) : "0"(sector_start))
- src/func_80014CBC.c:101 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__ volatile("" : "=r"(arg3) : "0"(arg3), "r"(nsectors))
- src/func_80014CBC.c:102 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__ volatile("" : "=r"(arg3) : "0"(arg3))
- src/func_80014CBC.c:121 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__ volatile("" : "=r"(arg3) : "0"(arg3), "r"(one))
- src/func_80014CBC.c:128 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("" : "=r"(src) : "0"(src), "r"(pos_mod))
- src/func_80014CBC.c:129 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("" : "=r"(src) : "0"(src))
- src/func_80014CBC.c:130 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("" : "=r"(src) : "0"(src))
- src/func_80014CBC.c:131 — embedded-asm — Embedded assembly is forbidden for an ordinary compiled function
    __asm__("" : "=r"(src) : "0"(src))

## Best clean program

[0, 0, 3, 5] at 96/114 words, measured 2026-10-24T16:09:15 (the experiment ledger's measurement of this exact text).

```c
#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libcd.h"
#include "psyq/memory.h"

/* GP-relative globals owned by this TU (CD loading state).
 * Tentative definitions merged with func_80014854.c / func_80014988.c via -fcommon. */
s32 D_8005E3F0;
s32 D_8005E410;
s32 D_8005E428;
s32 D_8005E430;
s32 D_8005E2B4;

/* func_80021B20 - original TU had no declaration (C89 implicit int).
 * Declared void so the call does NOT define $v0, matching target allocation.
 * Only arg0 is passed: the callee ignores its arguments, and the caller's
 * untouched a1/a2/a3 remain the incoming arg1/arg2/arg3 at the call. */
void func_80021B20(s32 arg0);

/* arg4/arg5 arrive as 4-byte BLKmode aggregates (align 1 keeps the mode BLK
 * on strict-alignment MIPS): assign_parms leaves each in its incoming stack
 * slot with no entry copy, so block 0 carries no parameter loads for them.
 * arg4 is then re-read at each use (the target's two lazy lw v0,80(sp)
 * loads); arg5 is read once into a register local below the arg1 home-store
 * carrier, which is the target's lw s4,84(sp) schedule slot. Callers pass
 * plain s32 values with identical ABI placement. */
typedef struct {
    u_char b[4];
} ReadFlag;

u_char *func_80014CBC(s32 arg0, s32 arg1, s32 arg2, u_char *arg3, ReadFlag arg4, ReadFlag arg5) {
    u32 pos;
    char *bp;
    char *eptr;
    s32 scaled;
    u32 pos_end;
    u_char *result = NULL;
    u_char *src;
    s32 sector_start;
    s32 pos_mod;
    s32 sector_end;
    s32 nsectors;
    s32 sync_result;
    s32 one;
    s32 a5;
    s32 len;

    /* Hybrid-asm entry group (user-authorized). Every literal template below
     * emits bytes the target already contains; the dummy operands exist to
     * pin scheduler dependences and allocno reference counts that no clean
     * source spelling reaches (measured; see this file's research notes).
     *
     * 1. The fused lui/addiu pair materializes the table base. Its arg0
     *    input makes it a successor of the arg0 entry copy, which delays
     *    that copy's second-scheduler release so the copy pair lands above
     *    the pair (target: sw s5/move s5 at +0x0c, lui/addiu at +0x14). */
    bp = (char *)D_80048B1C;
    scaled = arg0 * 40;
    /* 2. The table-entry add with a matching-constraint tie: the result
     *    stays in bp's register (addu v1,v1,v0) and, as an asm output, eptr
     *    is alias-opaque to the scheduler. */
    eptr = bp + scaled;
    len = arg2;
    /* 3. The incoming-arg1 home store. The declared may-write through the
     *    opaque entry pointer makes the table load below a dependence
     *    successor, which releases this store into its original slot
     *    between the callee saves and the loads (sw a1,68 at +0x54). The
     *    len/result inputs gate the arg2 copy and the result zero-init out
     *    of that release window so they keep their late target slots. */
    *(s32 *)((char *)&arg4 - 12) = arg1;
    pos = D_80048B1C[arg0].loc;
    pos += arg1;
    a5 = *(s32 *)&arg5;
    pos_end = pos + len;
    sector_end = pos_end >> 11;
    sector_end += (pos_end & 0x7FF) != 0;
    /* 4. The srl/andi pair: the original schedule emits the sector_start
     *    srl before the pos_mod andi, but every C statement order that does
     *    so also stretches sector_start's live range past the allocno
     *    priority boundary that costs it $s0. The dummy sector_start input
     *    on the andi orders the pair; the arg3/arg0 inputs are in-range
     *    reference pumps holding those webs in the target pick order. */
    sector_start = pos >> 11;
    pos_mod = pos & 0x7FF;
    nsectors = sector_end - sector_start;

    if (D_8005E410 == 0 || (one = 1, pos = 1, a5 == one)) {
        /* sector_start reference pump at the setup-block boundary: +2 refs
         * armor its priority; a barrier at a block top constrains nothing. */
        sector_start = sector_start;
        func_80021B20(pos);
        D_8005E428 = sector_start + D_8005E430;
        CdReadBreak();
        CdFlush();
        CdSync(0, 0);
        CdControl(CdlSetloc, CdIntToPos(D_8005E428, (CdlLOC *)&D_8005E3F0), 0);
        /* Volatile carrier pair inside nsectors' live window: +2 live length
         * and the +1 nsectors reference place the sector count web between
         * the len and result priorities, so the count takes $s7. Volatile
         * keeps them out of call delay slots and load-delay gaps. */
        arg3 = arg3;
        arg3 = arg3;
        CdRead(nsectors, (u_long *)arg3, CdlModeSpeed);
        D_8005E410 = 1;
        if (a5 == 1) {
            D_8005E2B4 = a5;
        }
        goto done;
    }

    sync_result = CdReadSync(1, 0);
    if (sync_result == -1) {
        goto retry;
    }
    if (sync_result != 0) {
        goto zero_tail;
    }

    /* one reference pump inside its live range: 'one' must be a real
     * variable carrying 1 across CdReadSync in $s3 to this compare. */
    arg3 = arg3;
    if (*(s32 *)&arg4 == one) {
        src = arg3 + pos_mod;
        /* Live-output src passthrough chain: +8 to len's live length in a
         * window where the other contested webs are dead, dropping len to
         * the $s6 slot of the target allocation; the pos_mod input is its
         * +1 reference. Emits no bytes. */
        src = src;
        if (arg3 != src)
            memmove(arg3, src, len);
        result = arg3;
        D_8005E410 = 0;
        D_8005E2B4 = 0;
    } else {
        result = arg3 + pos_mod;
        D_8005E410 = 0;
        D_8005E2B4 = 0;
    }
    goto done;

retry:
    D_8005E410 = 0;
    /* The recursion re-reads arg1 from its home slot (old sp + 4, addressed
     * relative to arg4's slot at old sp + 16), reproducing the target's
     * reload-born lw a1,68(sp) as an ordinary schedulable load. The callee
     * is called through an s32-typed pointer so the aggregate parameters
     * can be passed as the plain words the ABI actually carries. */
    ((u_char *(*)(s32, s32, s32, u_char *, s32, s32))func_80014CBC)(arg0, *(s32 *)((char *)&arg4 - 12), len, arg3, *(s32 *)&arg4, a5);
    goto zero_tail;

zero_tail:
    result = NULL;
    D_8005E2B4 = 1;

done:
    return result;
}
```

## Last report

```text
No forbidden constructs remain.

Oracle: func_80014CBC verdict MISMATCH — 52/131 words (39.7%).
Instruction count delta versus target: +14.
Residual (steer by this, not the word count): control-flow 3, population 69, schedule 5, allocation 13.
The two programs do not contain the same instructions, so no allocation or scheduling reading applies yet — fix the semantics first.
Next block: 7 (0x80014E10) — population 1, schedule 0, allocation 0. the instruction populations differ here; nothing below can be read until they agree
`psx_reverse_pipeline` gives the decisions, their source levers and the mechanism sheet to load; `psx_residual_objective` with a source ranks candidate edits and records them.

STALLED: 37 distinct measurements since the residual last improved on [0, 0, 0, 0]. The axis is exhausted, not the function — stop re-spelling it and bring heavier evidence. Audit the premises first, because everything else is conditioned on them and cannot see them: psx_callee_truth confronts every callee declaration in scope with the vendored SDK headers and the callees' own code, psx_sdk_idioms does the same for operation boundaries. A wrong declaration adds call setup no rewrite of this body can remove, and every measurement taken under it scored a different program. Then read the pass that decides the residual before writing another spelling: its psx_reference sheet, or the deciding function itself through psx_compiler_source. After that, enumerate the source space (psx_search_source_shapes) or solve for the compiler state instead of modelling it (psx_solve_local_allocation, psx_search_scheduler_state, psx_allocator_counterfactual). A solver result is a specification for a source shape, and an UNSAT is a real finding that closes a direction. Record what each one closed with psx_record_closed, and read that record before you run one — a direction another session already closed costs minutes to close again. When a search reports no exact candidate, that is not the end of its output: read the per-class residual axes and the runs each class moved, and take the next experiment from the axis that moved rather than from the match count. Before trusting any search verdict, check its caveats for constructs the grammar refused, its axis-effect block for axes that are counted but inert, and its coverage — a --derive-only run sampled, and a sample supports no statement about the domain. 37 distinct programs is past the point where more variation is informative. Exhausting a spelling family is a positive result: the answer is not a spelling. Switch to the author's frame, which the compiler-side tools cannot reach. The vendored SDK headers give the real signature and the real operation for anything the SDK provides — read the header rather than your reconstruction of what the disassembly implies it must say. The already-matched functions in this target's file group are the only record of how this author wrote code: which locals they kept live, how they walked an array, what they hoisted, whether they took a base pointer once or re-indexed each time. A byte-exact neighbour is a proven idiom. notes/file-groupings.md names the group; read three of its members before reading another pass. A residual that survives every rewrite of your own idiom is usually somebody else's idiom.
```

The next `/auto_decompilation_loop repair func_80014CBC` is pointed at this note.
