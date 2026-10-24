# Case study: 800F7230, spill-slot provenance and post-schedule allocation

**Function:** `ovl_11_func_800F7230` (`ovl_11`, 0x578 bytes, 350 words).

**Study date:** 2026-10-23.

**Outcome:** byte-exact plain C, 350/350, residual `[0,0,0,0]`. The function
finalization gate passed, and `make check-all` passed (13 overlays and the
EXE). The integrated source has no assembly, register pin, compiler patch or
flag override. Its one unusual construct is an empty `do { } while (0);`,
which 13 other matched sources already use.

**Scope:** what the problem was, how it was solved, and what the harness
should learn from it. The source comment in
`src/overlays/ovl_11/ovl_11_func_800F7230.c` records the three load-bearing
constructs. This document is about method.

## Executive summary

The function was parked on 2026-10-16 after the autonomous loop exhausted its
escalation ladder (67 ledgered measurements, best `[0,15,9,24]`). An earlier
session on 2026-10-23 added 123 measurements. Its best was `[0,0,9,15]`
(301/340), and that source faked two of the target's stack slots with a local
two-field struct, `TextureLoadBounds`. This session matched the function in 48
ledgered measurements, plus about ten unledgered screens.

Three readings solved it. None of them was a better spelling:

1. **Remove the fiction, and accept a worse key.** With plain `first` and
   `count` locals the key rose to `[1,38,6,5]`, but the prologue and the count
   load became exact. Every later step needed this base.
2. **Treat a held write as an allocation cause.** GCC 2.95 recomputes register
   usage after sched1 (`recompute_reg_usage`, before local-alloc).
   - sched1 floated `piece2 = NULL` above `func_80015704`. Its live length
     grew from 256 to 270 and its call count from 1 to 2.
   - That dropped piece2 below `tex` and `n` in global-alloc priority, which
     rotated s6/s7/fp.
   - An empty `do { } while (0);` directly after the call holds the clear
     below it. That fixed the s-register assignment in one step.
3. **Read the target's spill-slot order as evidence of where each value came
   from.**
   - reload assigns slots in pseudo-register order: source variables in
     declaration order, then GCSE temporaries, then registers loop.c created.
   - The target spills the request-list pointer *after* a GCSE temporary, so
     the pointer cannot be a source variable. It is a strength-reduced giv,
     and the original indexed the lists by `n`.
   - The caller-save threshold (`4 × calls < refs`) then limited the pointer
     to one increment. That ruled out the duplicated loop tail and pointed to
     a plain `for` loop, which matched.

**The lesson for the harness:** two pieces of evidence decided this function,
and no current tool reports either one. One is the stack frame read as
pseudo provenance. The other is the difference between register usage at
flow time and at allocation time. Both are mechanical to compute from
artifacts the harness already produces (§5).

## 1. The problem as it stood

### 1.1 Target shape

The function finishes a CD load of 0x50 sprite blocks, each 0x800 bytes. For
each id in the selected request list that falls inside the loaded range, it:

- parses the block (`func_80015704`);
- takes the last piece of two animation frames;
- claims a free record among 0x48 in `D_80129648`, after a `memset`;
- lays out VRAM and CLUT positions and uploads them (`func_80017200`).

The loop body crosses three calls, so register pressure is total. Every
callee-saved register is used, three values live in caller-saved registers
under caller-save, and five values are spilled:

| Value | Target home |
|---|---|
| `n` | fp |
| `id` | s3 |
| `i` | s2 |
| `i * 34`, record offset | s1 |
| `&D_80129648` | s4; its HIGH is held in t1 across `memset` |
| record address | s5 |
| `id - first`, first use | s0 |
| `tex` / `piece2` | s7 / s6 |
| `piece` / `tex2` | t2 / t3, caller-saved at 100(sp) / 104(sp) |
| `first` (= `arg0`) | 72(sp) |
| `count` | 76(sp) |
| `mode` | 80(sp) |
| `id - first`, for the second call | 84(sp) |
| request-list pointer | 88(sp) |
| t1, caller-save | 96(sp) |

### 1.2 The residual at the start

The best ledger source scored `[0,0,9,15]`. The differences were:

1. In the prologue, v0/v1 were swapped, and `first` was stored early.
2. `n` was in s7, `tex` in s6 and `piece2` in fp. The target has fp, s7 and
   s6.
3. `id - first` was computed into a1 and stored to 84(sp) at once. The target
   computes it into s0 and stores s0 *after* the call, in the delay slot of
   the first frame test.
4. After the call the target keeps `move t3,zero; move s6,zero` (the `tex2`
   and `piece2` clears) below the call. Ours floated the `piece2` clear above
   it.
5. `anim` and `frames` were swapped between t0 and a3.
6. In the loop tail, the target stores `list + 6` before the compare and
   leaves a `nop` in the delay slot.

### 1.3 Why it resisted

- **The best source was built on a fiction.** `TextureLoadBounds` made two
  stack slots appear by making `first` and `count` memory. That reproduced the
  slot addresses and lowered the population count. But spills are reload
  output, not source structure. The struct also made `first` a MEM operand,
  which GCSE treats as killed by every call, so any probe of how the original
  reuses `id - first` was measured on a different program.
- **The held write had been read as a toolchain gap.** The 2026-10-23 corpus
  scan found only 800BB108 and this function with a constant write held below
  a call, and 800BB108 had needed a register pin. That framing treated the
  s-register rotation as a separate allocation problem.
- **The staged key preferred the fiction.** `[0,0,9,15]` ranks above
  `[1,38,6,5]`, so the ledger's best source was the wrong base.

## 2. What happened, in order

Every key below is measured with `residualObjective.ts`. The words column
is its count.

| Step | Source change | Key | Words |
|---|---|---|---|
| start | ledger best: `TextureLoadBounds` struct | `[0,0,9,15]` | 301/340 |
| v1 | plain `first` / `count` locals | `[1,38,6,5]` | 312/343 |
| v2 | v1 with a single `for` tail | `[1,475,3,40]` | 265/339 |
| v3 | v1 with `relative = id - first` after the call | `[1,50,5,5]` | 307/343 |
| b1 | v1 with `do { } while (0);` after the call | `[1,38,5,4]` | 327/344 |
| b | b1; the call computes `id - first` inline; `relative` assigned after the barrier | `[1,38,6,3]` | 328/345 |
| p91 | `relative` moved to just before the first frame test | `[1,38,4,4]` | 329/344 |
| decl | `frames` declared before `anim` | `[1,38,4,2]` | 333/344 |
| if1_t | `relative` after the first frame lookup; `tex2` cleared before `piece2` | `[1,50,1,1]` | 333/348 |
| dupg | request lists indexed by `n`, duplicated tails | `[1,55,5,4]` | 284/340 |
| L4 | indexed lists, plain `for (n = 0; n < count; n++)` | `[0,0,0,0]` | 350/350 |

The population term of 38 that persists from v1 to `decl` is the duplicated
loop tail. Under plain locals, cross-jumping no longer merges its two copies.

### 2.1 Removing the fiction

Replacing the struct with two plain locals fixed the prologue and the count
load in one edit. That is the target's `sw v1,72(sp)` after the register
saves, with v0 and v1 the right way round. `first` and `count` are spilled by
reload in declaration order, which puts them at 72 and 76 with no help from
the source.

### 2.2 A loop shape that cost the counter its register

Collapsing the duplicated tail into one `for` tail (v2) spilled `n`
(population 475).

- `n`'s refs fell from 17 to 11 over a live length of 486, which gives a
  priority of 679.
- That lost to every callee-saved candidate.
- 11 refs over 3 calls fails the caller-save test (`4 × 3 < 11` is false).

So the `for` loop looked wrong. It was right only together with step 2.6,
where the live length of `n` halved. The cause of that halving was not traced
(§6).

### 2.3 The `relative` copy: why the obvious spellings fail

Assigning `relative` after the call from the same `id - first` (v3) made cse1
turn it into a copy of the pre-call temporary. GCSE's copy propagation then
replaced the second call's use with that temporary, and `relative` died.

Any explicit copy is undone the same way. For the target's shape, the copy has
to be created after copy propagation runs.

### 2.4 The barrier, and why a held write is an allocation cause

One empty `do { } while (0);` after `func_80015704` (b1) gave the target's
s6/s7/fp at once. The diagnostic `cc1`'s priority tables show why:

| Pseudo | v1 refs / live / calls | v1 priority | b1 priority | Target |
|---|---|---|---|---|
| `piece2` | 12 / 270 / 2 | 1333 (fp) | 1406 (s6) | s6 |
| `tex` | 10 / 214 / 2 | 1402 (s6) | 1402 (s7) | s7 |
| `n` | 17 / 490 / 3 | 1388 (s7) | 1388 (fp) | fp |
| `piece` | 10 / 226 / 2 | 1327 (t2) | 1327 (t2) | t2 |
| `tex2` | 10 / 228 / 1 | 1316 (t3) | 1316 (t3) | t3 |

Priority is `floor_log2(refs) × refs / live × 10^4`.

The source order was the same in v1 and b1. Only sched1's output differed.
Because `recompute_reg_usage` runs after sched1, the moved clear changed the
numbers global-alloc reads.

The barrier works through loop notes:
- sched.c treats a loop note as a dependence on everything before it, so the
  clears cannot cross the call.
- cse.c ends its block at `NOTE_INSN_LOOP_END` before loop.c runs
  (`cse.c:8528`). That changes GCSE's inputs too (§2.5).

A shape the harness can detect: in the no-barrier final source, `tex2` crosses
one call in the `.flow` dump and two in `.lreg`. The matching source has no
such change. §5 turns this into a detector.

### 2.5 Placing the copy

With the barrier in place, computing `id - first` inline at the first call
and assigning `relative` later (b) put the subtraction in s0 and the store
after the call. That is the target's shape. Here is what GCSE did, read from
the `.gcse` dump:

1. Copy propagation replaced `first` with `arg0` in every `id - first`.
2. PRE inserted `(set R (minus id arg0))` at the end of the post-call block:
   "PRE/HOIST: end of bb 18 … copying expression 49".
3. PRE made the second call's computation redundant on `R`.
4. cse2 turned the inserted computation into a copy of s0.
5. `R` is a GCSE temporary, spilled to 84(sp). It is stored in the delay slot
   because it is the last insn before the branch.

Two further steps finished the post-call block:
- Declaring `frames` before `anim`. They tie in global-alloc priority, and
  the tie goes to the lower allocno number.
- Clearing `tex2` before `piece2`.

The final source has no `relative` variable at all. The second call writes
`id - first` again, and PRE does the rest.

### 2.6 Reading the frame: the list pointer belongs to loop.c

After step 2.5 the call block matched, but the two slots came out swapped:
the copy at 88 and the list pointer at 84. reload's `alter_reg` assigns slots
in ascending register number. The GCSE temporary now numbered above every
source variable, so a pointer-walk `list` variable had to land below it. The
target puts the pointer *after* the GCSE temporary, and only loop.c creates
registers numbered that high. So the pointer is a reduced giv, and the
original indexed the lists.

The repository already had the view
`Ovl11TextureRequests { u8 pad[0x450A]; u16 lists[3][64][3]; }`. Indexing
`lists[k][n][0]` made loop.c combine the three address givs into one pointer
stepped by 6. The other two lists are reached at -0x180 and -0x300, as in the
target.

With the duplicated tails (dupg), the pointer got two increments: 15 refs over
3 calls. Caller-save was then profitable, and it was kept in t4 with saves
around every call. The target spills it, which needs at most 12 refs, which
means one increment. A small generator of seven loop shapes found two exact
ones: the plain `for` (L4) and an equivalent `while` (L6).

### 2.7 Ablation and cleanup

Each row was measured from the matching L4 source:

| Change | Result |
|---|---|
| remove the barrier | `[0,2,4,1]`, 335/349 |
| `relative` assigned before the call | `[0,0,4,1]`, 344/349 |
| `relative` right after the barrier | `[0,0,5,0]`, 345/350 |
| `piece2` cleared before `tex2` | 349/350; key `[0,0,0,0]` but not exact (§5, P1) |
| `anim` declared before `frames` | `[0,0,0,2]`, 344/350 |
| barrier spelled as `do { func_80015704(...); } while (0);` | `[0,0,1,0]`, 349/349 |
| `sprite.field` instead of `src = &sprite` | `[0,21,0,12]`, 300/349 |
| a direct `return -1` instead of `goto failed` | `[1,525,1,39]`, 334/348 |

These simplifications kept the match, and are in the integrated source:
- no `relative` variable;
- `tex = NULL` beside `piece = NULL`;
- one combined range test;
- `if (i == 0x48) goto failed;`;
- the true prototype `void func_80015704(SpriteSourceData *, SpriteDataHeader *)`
  from `src/func_80015704.c`;
- unused locals removed.

### 2.8 Closed directions

- **`-fno-schedule-insns` as a probe** that the original did not run sched1:
  196/360 or worse on every base. sched1 runs.
- **GCSE PRE as the sole source of the copy, without a barrier:** PRE copies
  after the first computation, and sched1 keeps that copy before the call.

## 3. Methodology that worked

- **Rank a source by whether its premises are true, not by its key.** The
  plain-locals base scored worse and was the only one that could match.
- **Turn the target's assignment into inequalities.** The target's s6/s7/fp
  order requires `piece2 > tex > n > {piece, tex2}`. Comparing measured
  priorities against that inequality named the pseudo to move and by how much
  (`piece2` needs live ≤ 256 or refs ≥ 13 to pass `tex`).
- **Use thresholds as constraints on structure.** "The pointer is spilled, not
  caller-saved" means at most 12 refs over 3 calls, which means one increment.
  That is a statement about the loop's shape, derived without a single
  compile.
- **Read the frame as provenance.** Slot order fixed which pass made each
  spilled value. That was the decisive inference.
- **Falsify cheaply.** One flag probe closed "sched1 did not run" in five
  compiles.

### Mistakes worth designing out

- **Theory before cheap probes.** A long analysis of GCSE copy propagation
  preceded the barrier probe, and that probe took one compile and resolved
  most of it. When a hypothesis has a one-compile test, run the test first.
- **A throwaway generator instead of the repository's.** The loop-shape
  variants were written ad hoc. `controlShapeSweep.ts` covers if/return
  control tails, not loop shapes, so that axis is missing anyway (§5, P2).
- **The final step was enumerated, not derived.** That `n` keeps fp under a
  single `for` tail once the lists are indexed, because its live length falls
  from 486 to 244, was not predicted.
- **Load-bearing tools lived in scratch.** The diagnostic `cc1` and the
  priority script exist only in the session scratchpad.

## 4. Mechanism catalogue

| Mechanism | Where | Effect here |
|---|---|---|
| Register usage is recomputed after sched1 | `toplev.c:4159`, `recompute_reg_usage` before `local_alloc` | A clear floated above a call re-ranks global-alloc |
| Caller-save profitability, `4 × calls < refs` | `regs.h:201`, used at `global.c:1109` | Decides caller-saved t-register versus spill for a value that crosses calls |
| Spill slots in register-number order | `reload1.c:797`, the `alter_reg (i, -1)` loop | Slot order shows source variable versus GCSE versus loop.c origin |
| Priority ties go to the lower allocno number | `global.c`, `allocno_compare` (`return v1 - v2`) | Declaration order decides `frames` a3 versus `anim` t0 |
| cse1 ends its block at `NOTE_INSN_LOOP_END` | `cse.c:8528` | The barrier separates the post-call code for cse1 |
| Loop notes are a sched1 barrier | `sched.c:1713`, loop notes in `sched_analyze` | Clears held below the call |
| GCSE copy propagation, then PRE insertion at a block end, then cse2's copy | `gcse.c`; `.gcse` "PRE/HOIST: end of bb" | Creates the s0 to 84(sp) copy after the call |
| Combined DEST_ADDR givs with constant offsets | `loop.c`: `combine_givs` | Three lists become one pointer stepped by 6 |

## 5. Harness proposals

### P0: spill-slot provenance in `frameMap` and triage

`frameMap.ts` reports "0x18..0x6F locals and spills (88 bytes)" and stops. It
should classify each target slot:
- caller-save area: a t-register stored before a `jal` and reloaded after;
- spilled pseudo;
- address-taken local.

It should print the slots in address order with the rule "slot order =
pseudo-register order: source variables in declaration order, then expand
temporaries, then GCSE temporaries, then loop.c registers". A triage detector
should compare the candidate's slot owners with the target's and report the
first disagreement. For example: "88(sp) follows a GCSE temporary in the
target, so it cannot be a source variable; check for an indexed access that
loop.c reduces". That one line, read at the start, would have put indexed
access on the table from the first variant, not the tenth.

### P0: a post-schedule call-crossing detector

Compare each pseudo's `Register N used … crosses K calls` line in the `.flow`
dump with the same line in `.lreg`. Both dumps already exist in
`build/compilerTrace/<fn>/`.

A pseudo whose call count rises means sched1 moved one of its insns across a
call, and global-alloc sees the moved version. Report the pseudo, the call
and the moved insn, and route to the schedule sheet instead of to allocation
spellings.

Measured on this function: the matching source shows no changes. The
no-barrier final source shows exactly one: `tex2`, 1 call becoming 2, and live
137 becoming 252.

### P1: a requirement report from the target's assignment

Promote the scratch `prio.mjs`. It reads the `.lreg` statistics and the
`.greg` assignments and prints global-alloc's priority table. Then:
- derive, from the target's hard registers, the priority inequalities and
  caller-save thresholds the assignment implies;
- print each unmet inequality with its levers, as in "piece2 must outrank
  tex: 1333 against 1402; needs live ≤ 256 or refs ≥ 13".

That turns a gauge into a gradient. The diagnostic `cc1` it relies on for
allocno order should be checked in as proposed in the 800B954C case study
(its §5, P0).

### P1: document the loop-note barrier, and detect held writes

`prompts/reference/schedule.md` §6 presents only `__asm__ volatile` barriers,
as a governed last resort. That steers agents toward asm when plain C works.
The sheet should describe `do { } while (0);` as its own construct:
- its loop notes are a sched1 barrier;
- its `NOTE_INSN_LOOP_END` ends cse1's block before loop.c, which can move
  GCSE's insertion points;
- a non-empty one raises flow's loop-depth weighting for the blocks inside it;
- 13 matched sources use it without an exception entry.

The policy question (allowed, or tracked debt?) should be answered in the
sheet. Separately, `build/bb108/scan/heldWrites.mts` should become a triage
detector: "constant write held below a call in the target, floated in the
candidate: try one empty `do { } while (0);` directly after the call before
any pin".

### P1: a fiction detector, and a caveat on ledger ranking

Flag a local struct whose fields are used only as independent scalars,
especially when its address is never taken except to reach a field. Its
likely purpose is to imitate spill slots. The stuck sheet should name the
pattern ("spills are reload output, not source structure").

The ledger's best-source selection should also not hand such a source to the
next session as the incumbent without saying so. Its key ranked first here,
and the premise was false.

### P1: same-shape transpositions are invisible to the staged key

The ablation that cleared `piece2` before `tex2` swapped two adjacent
instructions, `move t3,zero` and `move s6,zero`. `residualObjective.ts`
reported key `[0,0,0,0]`, words 349/349 and verdict "scored". `diffFunc.ts`
reported 349/350 and "MISMATCH — 0 word(s) differ". So an
adjacent swap of two instructions with the same shape and different registers
is counted on none of the four axes. Both tools should count it, as schedule
or allocation, and a key of zero should never coexist with a non-exact
verdict.

### P2: a loop-shape axis for sweeps

Add a loop-shape sweep covering:
- a guarded versus an unguarded `for`;
- a single versus a duplicated tail;
- pointer walk versus indexed access.

The last pair is the one that decides whether loop.c creates the pointer.
`controlShapeSweep.ts` covers if/return control tails only.

## 6. Honest limitations

- Why `n`'s live length halves (486 to 244) when the lists are indexed was not
  traced. The match was found by enumeration at that step.
- Why PRE inserts at the end of the post-call block only when the barrier is
  present was not fully traced. No set of `id` or `arg0` lies between the two
  computations. The cse1 block split at `NOTE_INSN_LOOP_END` is the
  difference that was observed.
- The reading that the original had a statement expanding to loop notes after
  the call (for example a compiled-out debug macro) is a hypothesis. The
  wrapped form `do { call(); } while (0);` is one schedule word off.
- For 800BB108 the same barrier was tried at 308 placements and reached
  `[0,0,4,3]`. Its register pin stays.

## 7. Artifacts (scratch, not tracked)

Under the session scratchpad
`/tmp/claude-1000/-home-dave-Documents-code-btn-decompilation/358c474a-25bd-4785-bef8-c019c70501b8/scratchpad/`:

- `f7/`: every variant in §2, with `.diff` files:
  - `v1.c`, `v2.c`, `v3.c`, `b1.c`;
  - `rel/`, `rel2/`, `r3/`, `r4/`, `g1/`, `L/` (with the generator
    `L/gen.mjs`);
  - `ab/` (ablations) and `fin/` (cleanup);
  - `match_L4.c`, the first exact source.
- `f7/prio.mjs`: the priority-table script over diagnostic dumps.
- `f7/d_*/`: diagnostic `cc1` dumps for the variants in §2.4 and §2.6.
- `gccdiag/`: the `global.c` diagnostic `cc1` (see the 800B954C case study).
- `r2/q.sh`, `r2/m.sh`, `r2/mx.sh`: measurement wrappers.

Ledger: `build/experimentLedger/ovl_11_func_800F7230.jsonl` (238 entries; the
first exact entry is at 2026-10-23T23:20:29Z).
