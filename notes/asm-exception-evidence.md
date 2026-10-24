# Asm-exception evidence

This file records the evidence behind source-policy exceptions (register
bindings, inline asm, scheduling barriers) that the user has authorized, so a
later session can tell a deliberate, justified exception from a workaround that
should be removed. Each entry says what the exception does, why plain C could
not reach the target under our toolchain, and what would retire it.

## ovl_19_func_800BB108 — `register s32 column asm("$16")` (authorized 2026-10-23)

**Exception.** The table loop's column counter is declared
`register s32 column asm("$16")` in `src/overlays/ovl_19/ovl_19_func_800BB108.c`.
The allowlist entry is `ovl_19_func_800bb108` (`register-asm`, `embedded-asm`) in
`.pi/autoloop.json`. The rest of the body is C.

**What the target needs.** The original's first `column = 0`
(`move $s0,$zero`) sits in the delay slot of `jal func_80017A64`, below the
`jal func_80015EE8` that ends the object's first use of `$s0`. The reset at the
end of each outer pass sits in the outer back-branch's delay slot, with
`move $s1,$s5` left before the branch. reorg's backward fill takes the insn
directly before a branch, so that tail reset must be the last insn of the outer
body. A reset at the top of the outer body loses the slot to `move $s1,$s5`.

**Why plain C does not reach it under our cc1.** The scheduler that runs here
is the old backward list scheduler (`sched.c`, not `haifa-sched.c`).
- It computes priority from predecessors, so a constant write has priority 1.
- The `D_8005E3C0->field_D8` double load gives `jal func_80015EE8`, and every
  later call, priority 3.
- A constant write to a pseudo that already crosses calls has no dependency on
  those calls, so it floats above them. `column` crosses the inner loop's calls,
  so its first reset lands at the top of the block.
- That overlaps the object pointer's lifetime in `$s0`. `column` is pushed to
  `$s1`, and `row`, `column` and `y` rotate (best clean C: `[0,0,1,3]`).
- Writes this scheduler does keep below the preceding call are:
  - sets of pseudos that cross no call (`REG_N_CALLS_CROSSED == 0`);
  - every hard-register set. `sched.c:1176` tests `call_used_regs[i]`, the loop
    index, not `regno + i`, so every hard-register set gets an anti-dependence
    on the last call.

**Measurements** (scratch under `build/bb108/`, ledgered).
- About 460 plain-C variants failed between two sessions. They covered
  statement order, natural `for` loops, variable reuse and the nested-definition
  position. Closed-directions entry: "Where must the initial column reset sit".
- A loop-note barrier (`do { } while (0)`, 308 placements) pins the reset into
  the right delay slot but shifts other allocation. Best `[0,0,4,3]`.
- An empty `__asm__ volatile ("")` barrier gives `[0,6,3,7]` or worse. A full
  barrier is the wrong model: only one write needs holding.
- Binding `column` to `$16` makes that one write a hard-register set. With the
  row reset after the `func_80015EE8` call and `offset + (s32)text`, it gives
  132/132 byte-exact (`build/bb108/asm/q2.c`, cleaned to `build/bb108/final.c`).

**Corpus scan** (`build/bb108/scan/heldWrites.mts`, 2674 functions read from
the original image). It looked for a constant write to a callee-saved register,
below a call, with no reference to that register in between.
- **All blocks:** 151 matched writes in 110 functions. Almost all are in blocks
  where every priority is 1, and there this scheduler keeps source order. They
  are not informative.
- **Blocks where a load feeds the calls (so the scheduler reorders) and the
  write has no consumer in its block:**

  | Function | Status | What keeps the write below the call |
  |---|---|---|
  | `ovl_19_func_800BB108` | unmatched (now pinned) | nothing in plain C |
  | `ovl_11_func_800F7230` | matched (2026-10-23) | one empty `do { } while (0)` directly after the call |
  | `ovl_11_func_801024D8` | matched | false positive: in RTL the loads are in an earlier block |
  | `ovl_15_func_801320A8` | matched | a loop.c constant that crosses no call |
  | `func_8001E4C0` | matched | stacked `do { } while (0)` barriers |

No ordinary statement shape gives this under our toolchain. The reading first
taken here was that the original compiler keeps such a write below the call and
ours does not. `ovl_11_func_800F7230` weakens that reading. Its two post-call
clears are held below the call in plain C by loop notes: one empty
`do { } while (0)` placed directly after the call, which sched1 treats as a
barrier. The original source may well have had a statement there that expands
to loop notes, such as a compiled-out debug macro. For 800BB108 the loop-note
barrier was tried at 308 placements and reached `[0,0,4,3]`, so the pin stays
until a placement is found that also keeps its allocation.

**What would retire it.**
- The toolchain itself is not changed. It establishes provenance, and the fact
  that most functions match under it is the evidence that it is close enough.
- A plain-C construct that holds that write below the call would replace the
  binding outright.

Related: `notes/file-groupings.md` (`ovl_19` nested-callee pair) and the
precedent `ovl_11_func_800FC8C8`, also a user-authorized callee-saved binding.

## ovl_19_func_800B95D4 — `register s16 *opponent asm("$19")` (authorized 2026-10-23)

**Exception.** The opponent pointer is declared
`register s16 *opponent asm("$19")` in
`src/overlays/ovl_19/ovl_19_func_800B95D4.c`. The allowlist entry is
`ovl_19_func_800b95d4` (`register-asm`) in `.pi/autoloop.json`. There is no
embedded asm, and the rest of the body is C. The user authorized "anything"
for this function after two sessions and 112 ledgered clean-C measurements
failed to match it (`notes/research/ovl_19_func_800B95D4-matching-attempts.md`).

**What the target needs.** The callee-saved roles are: s0 origin/command,
s1 angle, s2 actor pointer, s3 opponent (main and selected), s4 state, s5 the
constant one, s6 the actor base, s7 i, fp the walking view. All of these
except s0 cross every call in the loop, so `global_alloc` hands out
registers in `allocno_compare` priority order,
floor_log2(refs) * refs / live_length * 10000 (`global.c:593`). The target
therefore needs angle > actor > opponent > state.

**Why plain C does not reach it under our cc1.** The actor pointer is the
loop's strength-reduced giv. It is live over the whole loop (about 390 insns)
with 47 weighted refs, which caps its priority near 6056. Every clean
opponent spelling scores above that:
- One body-wide opponent: 34 refs over 119 insns, 14285, which also beats
  angle's 12352. Result: other s1, angle s2, actor s3 (`[0,0,2,7]`).
- A per-iteration `q` plus a separate selected-arm opponent: 12413 and 9670.
  Angle wins only when `angle = 0` follows the opponent reset, which puts the
  wrong insn in the loop-top delay slot (`[0,0,2,3]`, 194/238).

For the target order, the opponent must fall below about 6056, meaning a live
range around the whole loop, which its per-iteration reset rules out. The
other option is for the actor pointer to gain about 35 weighted refs. Neither
appears in the target's code.
- `REG_EQUAL` live-length doubling (`local-alloc.c:874`) halves the
  opponent's priority when its first set is a constant. Measured: the
  selected-arm opponent drops to 6206 and the main one to 4347. But it makes
  the reset a fresh `lui`/`addiu` (cse cannot see `base` at the loop top), or
  reverses the selected arm's two sets, because jump.c moves the else arm.
- Loop-note weighting (`do { } while (0)`) cannot be focused on the actor:
  its references are spread one or two per block across about 25 blocks
  (`flow.c` `calculate_loop_depth` weights whole blocks).

**Measurements** (scratch, this session).
- Pin the actor to `$18` (explicit walk): angle/opponent swap remains, 201/238.
- Pin the actor and the opponent: byte-exact once the selected record is
  spelled `(s16 *)(choice * 72 + (s32)actors)`, which gives the
  `addu s0,v1,s6` operand order.
- Pin the opponent alone, on the reset-family body: byte-exact, 238/238. This
  is the committed form and the smallest exception found. With s3 taken, the
  giv falls into s2 by itself.

**Reading.** This is an allocation-priority gap, not a scheduler hold like
800BB108. The original either had a variable structure we have not
reconstructed, or ranked these pseudos differently. The binding encodes the
one register the original chose and leaves every other decision to the
compiler.

**What would retire it.** A clean source in which the opponent's priority
falls below the actor giv's (a loop-carried opponent with no extra
instruction, or an actor pointer with far more weighted references). Start
from the reset family (`[0,0,2,7]`) and check the `.lreg` priorities with
`tools/agent/compilerTrace.ts`.

## func_800142D8 — three-instruction guard branch in asm (authorized 2026-08-09)

**Exception.** Guard branch 2 is written out as embedded asm in
`src/func_800142D8.c`: `slt %0,%1,165` / `beq %0,$0,$L3` / `slt %0,%1,92`,
under `.set noreorder` and `.set nomacro`. The allowlist entry is
`func_800142d8` (`embedded-asm`) in `.pi/autoloop.json`. The operands are plain
`"=&r"`/`"r"` constraints, so cc1 still allocates them and picks the target's
`$v0`/`$a2` itself. Branches 1 and 3, the `csqrt` call and the rest of the body
are C. The full record is `notes/retros/2026-08-09-func_800142D8-retro.md`.

**What the target needs.** In the target, reorg's `fill_eager_delay_slots`
takes the fall-through thread for branch 2 (`beqz`, predicted not taken): it
moves `slt $v0,$a2,92` into the delay slot, and the function stays at 44
instructions. Clean C takes the target thread instead. It copies the
calculation block's first instruction, `addiu $v0,$v1,-0x80`, into the slot
and redirects the branch past it. That costs one extra instruction (45), and
it is the whole residual: clean C reaches 41/45, with registers, schedule and
webs otherwise exact.

**Why plain C does not reach it under our cc1.** C cannot name a delay slot,
and every route to the target's reorg choice found so far breaks the
calculation block.
- **The sweep.** All 1,057,675 legal variable assignments over the statement
  skeleton were enumerated, and none reproduces the target. 13,368 reproduce
  the guard byte-exact, delay slot included, and then lose the calculation
  block's allocation.
- **The differential.** Every merge in the current file's partition also
  appears among those 13,368, so no single merge blocks it.
- **The catch-22.** The guard-correct sources win a sched.c tie-break by LUID
  (source order). The source order that gives that tie-break lets cse delete
  the `move $a2,$a1` copy. The copy survives only when `x = arg0 & 0xFF` sits
  between `y = arg1 & 0xFF` and `t = y` (`cse_insn`'s copy rewrite).
- **Premises ruled out.**
  - Signature: arity 2, 3 and 4 give identical output.
  - Flags: flagProbe finds no fingerprint, and the only 44-instruction column,
    `-fno-rerun-cse-after-loop`, scores 34/44.
  - Assembler: real ASPSX 2.86 under wine emits `nop` for the unfilled slot.
  - Scheduler: `.sched`/`.sched2` replay exactly in all 12 blocks.
  - Tail: 576 spellings are all inert.

**Scope of the negative.** The sweep covers renamings of one statement
skeleton, not every C source. The retro's own conclusion is that the skeleton
is what is wrong. It compared cc1's text output, so a variant that differs only
in spelling (`addu` for `move`) would have been rejected. The sweep artifacts
under `build/bruteForce/func_800142D8/` were removed by a later clean. The
counts above come from the retro.

**What would retire it.** A different statement skeleton, re-derived from the
target's instruction stream, that gives both:
- the guard's fall-through fill on branch 2;
- the calculation block's allocation.

The 13,368 guard-correct partitions are the positive examples to start from.
Load-bearing facts to keep:
- `x` is assigned exactly once (sched.c `adjust_priority` boosts only a
  single-set destination);
- `t = y` does not directly follow `y = arg1 & 0xFF`.
