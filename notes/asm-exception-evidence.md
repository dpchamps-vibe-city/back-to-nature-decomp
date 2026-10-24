# Asm-exception evidence

This file records the evidence behind source-policy exceptions (register
bindings, inline asm, scheduling barriers) that the user has authorized, so a
later session can tell a deliberate, justified exception from a workaround that
should be removed. Each entry says what the exception does, why plain C could
not reach the target under our toolchain, and what would retire it.

The six-candidate re-audit below also records **unresolved repair debt**. An
existing authorization, an exhausted source family, or an UNSAT conditioned on
one compiler state is not proof that arbitrary clean C cannot match. Those
entries explicitly distinguish a proved local obstruction from that stronger
claim; they grant no new exceptions.

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

## Six-candidate re-audit — 2026-10-24

**Conclusion:** no whole-function clean-C impossibility was established for any
of these six. The shared theme is compiler bookkeeping, not an instruction
that C cannot express: pseudo sets, coalesced quantities, lifetime padding,
reload-generated copies, and parameter residence. In particular, many pins do
not imply many independent obstacles. All six target read-before-definition
scans were clean in this audit; that rules out that fingerprint, not every
possible handwritten origin.

The strongest family is `func_80012598` / `func_8001231C`: a common first
initialization loop, with different constants and second phases. The two sprite
functions share a scheduling/allocation interaction, but are not established as
one source TU. The CD reader and overlay predicate have different mechanisms.
There is no evidence here for one compiler-flag change fixing all six.

Measurements below use production container flags and staged keys
`[control-flow, population, schedule, allocation]`. Scratch sources are under
`build/asm-exception-audit/` and are also captured by the experiment ledger.
No live C, headers, flags, or allowlist entries were changed. The pre-existing
working-tree removal of `func_80016280` from the allowlist was preserved: its
hybrid still byte-matches, but currently fails the policy gate.

Verification after the audit: `make check` passed (original executable payload
matched), `git diff --check` passed, and the document's relative links resolve.
This verifies the unchanged live build, not any non-exact scratch candidate.

### func_80019070 — the historical scheduler ceiling is superseded

**Existing exception.** Sixteen fixed-register locals and prologue, parameter
load, and primitive-linking asm in `src/func_80019070.c`. Historical reasoning:
[prologue research](research/func_80019070-prologue-allocation-and-arg2-truncation.md)
and [tail certificate](research/func_80019070-tail-window-unsat-certificate.md).

**Counterexample to the broad claim.** The complete no-asm candidate preserved
in [the repair note](human-needed-approvals/func_80019070.repair.md), source SHA
`ba9cfd7427d527f155157f09e52fb1a8958f872c9f0fbffd0489d9c987a55668`, was freshly
compiled. It reaches **`[0,0,0,1]`, 79/81 words**. `diffFunc` identifies only
`0x800190B4` and `0x800190B8`: the `sltiu` result and its branch use `$a0`, not
`$v0`. Thus the target's instruction population **and final order** are already
reachable without asm. The earlier scalar/boost UNSAT does not cover this
aggregate/SUBREG representation. The old tail certificate assumes an asm write
of `$v0`; it cannot exclude an all-C prologue.

This is a compiler-mechanism witness, not evidence that the original author
used its union, XOR identities, or empty loop wrappers. It also splits the
`setSprt` operation into `setlen`/`setcode`, which triage flags for SDK-boundary
review. It is not a completed, promotion-approved reconstruction.

**A genuine, narrower obstruction: padded local lifetimes.** Current
instrumented local allocation replays 23/23 decisions on this candidate.
In compiler block 0:

| Quantity | Meaning | Actual interval | Interval tried first | Assignment |
|---|---|---|---|---|
| q0 / r117 | two-word header carrier | 4..28 | 2..30 | `$v0/$v1` |
| q2 / r123 | palette `< 6` predicate | 30..42 | 28..43 | `$a0` |

Intervals are the allocator's half-instruction positions, not addresses.
`gcc/local-alloc.c:block_alloc` (lines 1433–1475 in the pinned 2.95.2 tree)
computes `fake_birth` / `fake_death`, extending each interval by a neighboring
instruction to avoid false dependencies for sched2. `find_free_reg` unions
`regs_live_at` over that widened interval. The header is allocated first;
`$v0/$v1` are unavailable to the predicate in the widened trial. `$a0` succeeds,
so the allocator **does not retry the true, non-overlapping interval**.
Neither quantity has a hard-register suggestion bypassing that trial.

With this partition, order, mode, and suggestion state fixed, `$v0` for the
predicate is unavailable through the ordinary priority path. This is a
compiler-source explanation of the two-word residual, not an all-C proof.
Changing those inputs reopens it; disabling sched2 is not justified.
Receipts: `build/localAllocationOracle/func_80019070/variants/` +
`ba9cfd7427d527f1-4f755259ee23ed30/analysis.json` and `events.jsonl`.
The oracle's raw indexed count is not the relocated-byte verdict; the latter
is the separately reproduced 79/81 result above.

**Bounded checks.** Reusing `header.value.len` for the predicate gives
`[0,0,0,9]` (also an earlier recorded experiment); assigning the predicate to a
whole integer-wide carrier gives `[0,0,0,15]`. An empty loop-note boundary ahead
of the palette load gives `[0,0,4,9]`. None is a repair.

**Retirement criterion.** A different clean representation or allocation state
that retains the proven order and puts this predicate in `$v0`, followed by SDK,
policy and full byte verification. The old claim that plain C cannot produce
the prologue order must not be used to reject that work.

### ovl_11_func_800EDF3C — coalescing, not just priority

**Existing exception.** Three pins (`p`, `off`, `field`) and a two-instruction
`ori 0x8000; addu` prefix in
`src/overlays/ovl_11/ovl_11_func_800EDF3C.c`. The target consumes the offset in
the `addu`; it is not a dead asm output. Calls and control are C.

**Invalid old premise.** The 2026-10-06 local-allocation UNSAT used an oracle
that then compiled overlays with executable `-G8` flags. The ledger's
2026-10-21 correction explicitly invalidates using that result for production
`-G0`. It is not exception evidence.

**Replacement evidence, measured with current flags.** Recompile the old v85
source with the witnessed declaration `s32 func_8001AF44(u32 arg0);`:
`edf3c-prototype.c`, SHA prefix `460455261656106c`. The declaration is byte-inert;
the result is `[0,0,0,4]`, 44/49. The current oracle replays **14/14** local
allocation decisions. In compiler block 10 it emits a `merge` event for
**q1 = [r112, r89]**, the address sum and the `0x8000` offset.

The target needs the offset in `$a0` and the sum in `$v1`. A single allocation
quantity cannot receive both registers. Consequently **no change of quantity
priorities alone can match this source state**, regardless of phantom-reference
budget. This is a direct partition contradiction, independent of the old SAT
result. `local-alloc.c:block_alloc` calls `combine_regs` on a dying input and
then propagates one hard-register assignment to all quantity members.
Receipt:
`build/localAllocationOracle/ovl_11_func_800EDF3C/variants/edf3c-prototype-394db5841269bbd8/events.jsonl`.

A natural local struct view of byte `+0xE647` gives `[0,0,0,6]`, byte-identical
to the older v50 family. The prefix itself is therefore expressible in C;
that test does not produce the target's allocation.

**Scope / retirement.** Change the quantity partition, dying-input choice,
operand/address family, or locality, not merely the ranking of the existing
quantities. This remains repair debt, not a proven whole-function exception.
See also [the repair note](human-needed-approvals/ovl_11_func_800EDF3C.repair.md).

### func_80016280 — coupled entry state, ordinary rendering loop

**Existing exception.** Eleven bindings, an entry/guard asm region, and an
asm-to-C state handoff; the complete rendering loop is active C. The guard
contains the literal branch word `0x05C0009F`. The historical account explains
that word as a workaround for an extra maspsx nop at the asm boundary, **not**
as evidence that the original function needed asm.

**Current reproduction.** Reconstructed the documented clean `work`/`tmp`/
`base`/`off` entry from
[the research note](research/func_80016280-web-parity-and-register-recurrence.md)
while retaining the live SDK loop. `16280-recovered-entry.c`, SHA prefix
`6f2b2d4ab321614e`, gives **`[0,0,2,5]`**. The byte comparison has 214 words on
each side; its aligned display reports 202/212. Only entry blocks 0 and 1
have staged residuals; the rendering suffix is clear. A count-up loop variant
was also measured, rather than assumed equivalent: `[0,5,6,15]`.

**Mechanism and limit of the claim.** The historical dumps show incoming
pointer saves scheduling late, keeping argument registers occupied when the
header/base temporaries need them. Parameter reuse can remove the scheduler's
single-set birth boost, but combine can merge the save with the entry copy,
removing the very dependency being sought. It also changes allocation in the
otherwise solved loop. This links it mechanistically to the *old scalar*
`80019070` problem, not to the two heap initializers' reload-copy problem.

The current reversal has an ambiguous delay-slot origin and a failed
pre-dbr-order round trip; its inferred sched1/sched2 ownership is provisional.
Neither that report nor the historical bounded shape searches proves that a
different clean entry representation cannot work. In particular, `80019070`'s
later SUBREG counterexample is a reason to distrust a universal scalar-web
ceiling, not a ready-made source transplant for these pointer parameters.

**Retirement criterion.** Reproduce the early saves and both entry blocks'
allocation without changing the already-matching loop. Until then the hybrid
is unresolved debt. The audit does not restore its currently revoked allowlist
entry or claim a policy pass.

### func_80012598 — reload-created pointer copies

**Existing exception.** Eighteen bindings at the start of this audit, now ten
(see the shared-helper follow-up below), with explicit base-copy asm,
split-address asm and empty operand carriers in `src/func_80012598.c`.

**Shared mechanism.** Its first initialization loop and `func_8001231C`'s have
the same store-role structure. The [historical investigation](research/func_80012598.md)
measured **4 register-copy sets at `.lreg`, 18 at `.greg`** in a candidate.
That demonstrates that our toolchain can introduce the fan-out during
allocation/reload; it is not a recovered dump of the original compiler run.
Calling the target copies reload rematerializations is a supported
interpretation, not direct observation of the target's pass history.

Manufacturing a separate source variable for each final copy changes
simultaneous register pressure, equivalence recognition and load elimination.
A single transparent base can instead let CSE remove reloads that the target
retains. Thus removing pins from the existing disassembly-shaped source and
permuting its statements does not test the relevant source family.

**Current checks.** The unpinned `local-cluster.c` candidate recompiles to
`[0,0,4,51]`. Adding the missing SDK `memset` declaration is byte-inert
(`12598-sdk-declaration.c`); it does not explain the residual. Triage's earlier
near-exact ledger entries included partially de-pinned/hybrid sources and must
not be described as a clean-C floor.

**Scope / retirement.** No toolchain impossibility established. The needed
source must produce the appropriate distinct, reloadable pointer values
without making all final copies simultaneously register-resident. Preserve
SDK/store semantics and verify the resulting reload behavior, not merely a
register permutation. Existing pins remain repair debt.

### func_8001231C — the same heap-initializer family

**Existing exception.** Eighteen bindings at the start of this audit, now ten,
and copy/address/carrier asm inherited from `func_80012598`, plus explicit
materialization of `memset` arguments.
See [the historical twin analysis](research/func_8001231C.md) and
[the later repair record](human-needed-approvals/func_8001231C.repair.md);
the former's opening "not yet attempted" status is historical, not current.

**Cross-function experiment.** Transferred the unpinned `80012598` loop rather
than the pinned implementation: changed `0x801FCFF8` to `0x801F7000`, work
stride `0x5EDC` to `0x2EE0`, and replaced the second phase with the witnessed
`func_8001E160()` call. `1231c-sibling-loop.c` gives **`[0,0,6,51]`**. Its loop
has 47 allocation differences, as does the donor's loop; the preheader has
four in each. This independently corroborates the shared reconstruction
problem and reaches zero staged population residual, where the prior repair
note's selected candidate had three. It is not a byte match or a claim of
semantic equivalence based solely on the metric.

**Scope / retirement.** Solve the source/reload relationship in either twin
and transfer the mechanism to the other. The common obstruction does not make
either function impossible in C, and a wholesale pin transplant is not
independent evidence for a second exception.

### Heap/copy helper follow-up — exact encapsulation and sixteen fewer pins

At the user's request, tested shared helpers for the two heap initializers.
`include/graphics_heap.h` now names the common pool layout and centralizes
only the existing `MATCH_COPY` and `MATCH_LOAD_ADDRESS` operations. Each
initializer uses twelve explicit register-value copies and one split symbol
address load. Neither helper embeds register numbers. The address macro keeps
its two separate asm statements and caller-supplied high-part temporary;
combining them would change the compiler's instruction/lifetime boundaries.
These macros are standalone-statement helpers, not memory-copy operations.

The heap constants describe independently witnessed roles: the main OT starts
at `+0x290`, primitive buffers at `+0x4290` with stride `0x17700`, and work
areas at `+0x33090`. The work strides are `0x2EE0` and `0x5EDC`. Thus the two
free-pointer values are `base + 0x33090 + 2 * workSize`, not unrelated magic
addresses. The large carve ends at offset `0x3EE48`, eight bytes before the
`0x3EE50` clear extent. `func_8001E160` independently corroborates the OT and
primitive pools; `func_800120C8` passes the work pointer to `GsSetWorkBase`.

**Measured separately from extraction:** removed each of the eighteen pins
in isolation, preserving the remaining asm. Nine single removals were exact
in `80012598`; eleven were exact in `8001231C`. Those results do **not** compose:
removing all individually redundant pins yields `[0,0,0,9]` and `[0,6,4,16]`,
respectively. Restoring the context pointers and context-offset binding
preserves the expected allocation while allowing these joint removals:

| Function | Bindings removed together | Remaining | Final words |
| --- | --- | ---: | ---: |
| `func_80012598` | `s2`–`s7`, `t8`, `t9` | 10 | 209/209 |
| `func_8001231C` | `s0`–`s7` | 10 | 159/159 |

Both retain two context-pointer bindings, the `t7` offset binding and seven
scratch bindings. In the large initializer, pipeline reversal located the
combined-removal residual entirely in preheader register allocation; retaining
`t7` alone restored the exact candidate. In the small initializer, retaining
both context pointers first removed the population/scheduling residual;
retaining `t7` then restored exactness. Candidate sources and measurements are
under `build/heap-copy/` and the two experiment ledgers. This is a bounded
reduction, not a proof that the remaining set is minimal or necessary for
another source formulation.

**Interpretation:** a common heap initializer or copy helper is plausible
original source organization. The matching extraction establishes that this
organization is usable now, not that these asm templates existed originally.
No vendored SDK heap-initializer macro or clean-C impossibility was established.
The empty operand carriers, countdown asm, small initializer's explicit
`memset` argument materialization and ten pins per function remain visible
repair debt. Existing policy exceptions remain; no new flag or exemption was
added. Old source comments claiming the original was certainly plain C, or
that a candidate's reload dump was the original's, have been corrected.

### func_80014CBC — audit the callee before declaring scheduling impossible

**Existing exception.** Entry/address/shift asm and empty operand carriers
steering scheduling and reference counts; byte-sized aggregate stack
parameters and a hand-addressed parameter home slot. The
[closing retro](retros/2026-08-14-func_80014CBC-retro.md) explicitly records
several prior "unreachable" walls falling to changes outside their assumed
source frame. That history is not a proof of a final clean-C ceiling.

**New premise check.** `psx_callee_truth` finds that `func_80021B20` reads **no**
incoming arguments. Its authored one-argument matched definition does not
prove that the original caller passed one. The target call sites do not
establish an upper arity bound; zero arity remains a hypothesis, not a shared
header correction made by this audit. Keep the vendored `CdSync` declaration:
its disputed return type is unused here and is not this issue.

**Measured consequence.** Start with archived no-asm scalar/volatile-parameter
source `e048dbc2faad1438` (in the experiment ledger), not the BLKmode/asm hybrid.
It recompiles to `[0,0,4,0]`. Change only:

```c
void func_80021B20(void);  /* instead of (s32 arg0) */
/* ... */
func_80021B20();          /* instead of func_80021B20(pos) */
```

`14cbc-no-debug-arg.c`, SHA prefix `99c6302e4b191301`, gives
**`[0,0,1,2]`**, with equal 117-word target/candidate lengths in the reversal.
The remaining differences are the `$s3/$s4` assignments for `one`/`a5` and
placement of the retry-path `D_8005E410 = 0` store. The ordinary C entry now
has zero staged schedule residual, without the entry asm's dummy dependencies.
This is progress over a premise, not a solved function. Replacing
`CdReadSync(1,0)` with `CdReadSync(one,0)` is byte-inert; hoisting `one = 1` to
its declaration changes population and does not fix it.

**Scope / retirement.** Audit the callback's other callers before changing its
shared declaration, then repair allocation and the retry store in the
no-argument branch. The historical statement that no clean spelling can place
the entry group is too broad. No whole-function impossibility or new source
exception is justified by this audit.
