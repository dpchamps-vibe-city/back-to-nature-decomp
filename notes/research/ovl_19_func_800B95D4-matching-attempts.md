# ovl_19_func_800B95D4 — matching attempts and unresolved allocation

## Resolution (2026-10-23, later session)

**Matched byte-exact, 238/238, with one user-authorized register binding.**
The source is the reset family of this note (`truth.c`, L081) with three
changes:
- the opponent is declared `register s16 *opponent asm("$19")`;
- the selected record is spelled `(s16 *)(choice * 72 + (s32)actors)`, which
  fixes the `addu s0,v1,s6` operand order (the `integer-base.c` finding);
- typed callee views, an early `continue` and no `actorAt` helper, which are
  output-identical tidy-ups.

The evidence, the priority arithmetic, the rejected levers and the condition
for retiring the binding are in `notes/asm-exception-evidence.md`. In short:
the strength-reduced actor pointer is capped near priority 6056, and every
clean opponent spelling scores above it. With s3 taken, the giv falls into s2
by itself.

The rest of this note is the record of the clean-C campaign that preceded the
match.

## Status and scope

**Unmatched. No clean-C candidate has passed the byte oracle or function
finalization.** This is an investigation record, not a solved-function retrospective
or a request/authorization for a source-policy exception.

This snapshot covers all **112 entries** in
`build/experimentLedger/ovl_19_func_800B95D4.jsonl`, ending at
`2026-10-23T18:45:36`, plus the preparation, searches, and diagnostics described
below. The ledger contains 112 distinct source hashes, **58 distinct compiled
output hashes**, and **zero exact results**. The other 54 entries reproduce an
output already recorded. Search-generated candidates are separate from these
112 entries; their counts must not be added as if they were unique ledger outputs.

The complete ledger SHA-256 at documentation time was
`06f5cde54946d0da36b994264d19e0f0035738c262e5402a5cd4cc351c210975`.
The appendix gives every entry a stable, local identifier, L001–L112, and records
its source hash, measurement, output-equivalence class, and source change.

At the end of the latest matching session, the live file was restored
byte-for-byte to that session's initial file: an `INCLUDE_ASM` with a disabled
attempt and an earlier diagnosis preserved below it. Its SHA-256 was
`3a544b545d08b18654d711a4e2acf25e4633434ba35cf6d92716f4f8311d8201`.
The subsequent `make check` reported that the payload matched **using the assembly
stub**. It did not validate any of the nonmatching C candidates. No pin, embedded
assembly, flag override, or new assembly stub was introduced as a solution.

## Best results: distinguish three different questions

Residuals below are `[control flow, population, schedule, allocation]`.

| Question | Recorded result | Source identity / location | Qualification |
| --- | --- | --- | --- |
| Lowest lexicographic residual | `[0,0,1,6]`; ledger reports `196/236` | L046, `c2529328f13cd5bd`; `build/experiments/ovl_19_func_800B95D4/current/preserved.c` | The separate oracle report says **196/239 actual words**, target 238: one extra instruction. This is the carried-opponent family, not a structural fix. |
| Highest historical word match in the correct-length, `[0,0,2,3]` family | **194/238**, about **81.5%** | L031, `3df7f1663151a8b5`; also L058, `ce6ee29c8d70bf4f`, retained as `build/experiments/ovl_19_func_800B95D4/baseline-typed.c` | A historical measurement, not a new oracle run performed for this note. |
| Convenient named, recently rechecked correct-length candidate | **193/238**, about **81.1%**, `[0,0,2,3]` | L070, `01893a5bfe0244d8`; `build/experiments/ovl_19_func_800B95D4/selected-opponent-p.c` | Angle uses the target register; current-actor/opponent roles remain swapped. |

**Correction to the preceding chat summary:** 193/238 was the named candidate's
result, not the highest historical correct-length result. The ledger also retains
the 194/238 family. Conversely, the `196/236` residual-report denominator must not
be interpreted as a 236-word machine function or converted into an 83% byte-match
claim. The preserved oracle and machine comparison establish 239 candidate words.
The appendix reproduces ledger fractions verbatim, rather than silently replacing
their alignment-derived denominators with machine instruction counts.

A byte score is not a distance. For example, correcting the opponent reset removes
the extra instruction but rotates registers and lowers the word score. Likewise,
different output hashes can share one residual key; do not deduplicate by key alone.

## Target facts and reconstructed behavior

The target is the `ovl_19` function at `0x800B95D4`, size `0x3B8` / 952 bytes /
238 machine words. Original assembly:

`build/ovl_19/asm/nonmatchings/ovl_19_func_800B95D4/ovl_19_func_800B95D4.s`

Frame-map evidence gives a `0x38` frame: `0x10` outgoing argument area, no stack
locals, and `0x28` saved-register area (`s0`–`s7`, `fp`, `ra`). No incoming argument
register is read before being overwritten. The caller `ovl_19_func_800B89C8`
discards the result, supporting the working `void function(void)` signature;
this does not prove that every possible original declaration returned void.

The working interpretation is a two-actor command/direction driver:

- Actor records begin at `D_800BF4D0` (`D_800BF4C0 + 0x10`) and have stride
  `0x48`. Witnessed record fields are halfwords at `+0x2` (command), `+0x4`
  (direction), `+0x6` (timer), `+0x8` (value), and a word flag at `+0xC`.
- Each iteration initializes the direction result to zero and selects the other
  actor. The early guard calls `800BA73C` and tests the separate state entry at
  `D_800BF4C0 + i*0x40 + 0xA2` against 2. Its success arm issues command 4 and
  stores the two `func_8001FABC(0x3B/0x44)` results at state offset `+0x202`.
- Command 4 gates on `timer == (timer / period) * period`, with signed division
  and the observed multiply-back. A rewrite using `% period` is not automatically
  the same instruction population. Positive timers perform reciprocal
  `800BA468` tests, choose an actor through `800BA2D4`, and issue commands 9/10.
  Nonpositive timers choose commands 6 and 7/8, or command 5, updating flags and
  the `0x3C` sound handle as observed.
- The fallback calls `800BA750` and `800BA33C`. Cases 0/1 draw a direction with
  `Rand(4)`; case 3 loads the separate walking view; case 2 compares a signed
  reading of `Rand(101)` with `value*100/255`, optionally adds the angle-band
  result, and takes the signed remainder modulo 4. The resulting command is
  issued with the duration at state offset `+0x178`.

These names are working field/operation names, not recovered original identifiers
or a formal proof of complete semantic equivalence. Zero inventory/population
residual in the best families is supporting evidence, not a substitute for bytes.

The existing `notes/file-groupings.md` section headed
“`ovl_19` D_800BF4D0 overlay-local s16-state consumer cluster” already documents
shared data and calls. No new TU-membership claim is established by this note.
Same-container matched examples inspected across the sessions include `800B847C`,
`800B9454`, `800B9AB0`, `800BA2D4`, `800BA33C`, and the target's matched callees.

## Preparation and declaration audits

### Preparation did not supply a generated draft

Two preserved preparation directories are relevant:

- `build/preparation/ovl_19_func_800B95D4/76d1da0f71fb03965a9e1148e3165addf68f6aac41792f98142db1e32529e2a6/`
- `build/preparation/ovl_19_func_800B95D4/2133c2b842533d7cae1e3631b198506c55749ba8bb43d775f9bb9933836c4c53/`

Read their `handoff.md` and `evidence.md`, not an assumed successful m2c output.
The fresh handoff reported generation unsupported, compilation/comparison not
attempted, one blocker, and 16 unresolved facts. The concrete generation error
was `No container defines func_80012A34`. Type propagation reached a fixed point
across ten original functions without establishing a complete return-type
contract. There was no applicable paired static-chain census finding and no
header-asm-macro exception. The initial manual C attempt was authored separately.

The later session began from the disabled, already-existing attempt, extracted
it under `build/`, and preserved the initial live file as `current/live-original.c`.
It did not replace an existing attempt with a newly generated seed.

### Rand alias: repair the contract without changing the target comparison

`configs/symbols/exe.txt` identifies address `0x80012A34` as `Rand`.
Its independently matched definition in `src/Rand.c` is:

```c
u32 Rand(s32 arg0);
```

The overlay uses the `func_80012A34` symbol alias. Older drafts declared that alias
as returning `s32`. L056 tried the real `Rand` spelling and unsigned return with
an explicit `(s32)` cast at the `101` comparison; the output changed from 194 to
192 matching ledger words, without moving the residual. L057 restored the overlay
alias with the witnessed `u32` return and signed comparison, recovering L031's
output. L081 performed the same contract repair in the reset-family draft and
reproduced L022's output. This is a repaired declaration premise, not a solution
to allocation. Generated `functions.h` is not independent signature evidence.

### False void-return finding for func_8001FABC

`psx_callee_truth` / triage repeatedly reported that `func_8001FABC` could not
return a value because it contained no explicit definition of `v0`. That finding
is false for this forwarding wrapper: original code calls `func_800212A8` and
returns its `v0`; matched `src/func_8001FABC.c` explicitly returns that call's
result. The caller's halfword stores are real and must not be deleted or replaced
with a void declaration. This exception to the detector's conclusion is recorded
in the closed-direction ledger; it is **not** a clean-source policy exception.

The remaining direct call arities agree with matched definitions. L058 replaced
`void *` erasures for `800BA468`, `800BA5B4`, and `800BA750` with the matched record
views and casts; it reproduced L031. Some later scratch families again use
`void *` declarations, so declaration fidelity still needs reconciliation before
promoting one of those files. `psx_sdk_idioms` found no PSY-Q packet signature.
The historical addition of `extern s16 D_800BF4D0[]` belongs in
`include/globals_override.h`; no generated header should be hand-edited.

## Source experiments: what actually moved

### Initial construction and population repair (L001–L017)

The first draft combined indexed actor and state views, chose the opponent in the
loop, and ordered the fallback case bodies as 0/1, 2, 3. Its residual was
`[0,73,3,11]`. Introducing an origin pointer and sharing it more broadly did not
settle the layout; reusing the early origin as the later state pointer also
changed control flow. A record view alone was an identical-output spelling.

Walking the actor independently of the indexed state view reduced population to
62 (L006). Moving case 3 before case 2 eliminated the remaining population term
(L007). Splitting the initial status result from the later actor choice improved
allocation 17 to 13 (L008). Giving the constant one an explicit birth before the
base improved scheduling 6 to 5 (L009). An independently walking `view` improved
scheduling to 3 (L010); reusing the early origin for the selected actor improved
allocation to 7 (L011). Indexing `p` while explicitly walking `view` reached
`[0,0,2,7]` (L014).

Moving the selected-opponent initialization before the selection call was a
trade/worse scheduling family, not a retained solution (L015–L017). A ternary did
not produce a new output there.

### First allocation plateau and alternate lifetimes (L018–L055)

These experiments tested the base's birth site, actor-pointer construction,
short versus full-width locals, a named final-duration base, early-return versus
`else`, typed records, and independent scan pointers. Most successful-looking
respellings returned to L022 or L031 rather than making a new program.

The important retained improvement was the birth of `angle = 0` **between**
`q = base` and the opponent-selection conditional. Moving it after the conditional
first reached allocation 5 (L030); moving it between the two operations reached
allocation 3 (L031). This fixes the angle role without solving the actor/opponent
swap. A loop-local initialized `q` followed by `other = q` was not equivalent,
for allocation, to writing the body-wide `other` directly (L049 versus L050).

Carrying `q` across iterations, initializing it before the loop and resetting it
in the loop condition, produced `[0,0,1,6]` (L046). This is the lowest ledger key
but has an extra machine instruction. It must not displace the correct-reset
branch merely because its word score or schedule term is better. Changing to
`q += 36` with an `else` reset made population much worse (L048).

Source-name reuse was context-sensitive: reassigning an indexed `p` to a selected
actor/opponent often lost induction structure; the same reassignment with a
separate walking `scan` could reproduce the good family. A temporary affine
cursor walk, ordinary `register`, and pointer `const` were measured, not fixes.

### Contract repair, further control/representation trials, parking (L056–L078)

After the Rand and record-view audits, experiments included a value-returning
signature, deriving the base from `scan - i`, directly merging opponent locals,
indexing the formerly walking view, reusing the opponent local as a view cursor,
and reusing `angle` as a byte offset. None was exact. Duplicating the final
`800BAC40` call into the switch arms worsened population to 25 (L074).

The named `selected-opponent-p.c` candidate (L070) keeps a separate scan BIV and
reuses its per-iteration alias for the selected opponent. It has `[0,0,2,3]`,
193/238, with angle correct and actor/opponent registers swapped. The auto-loop
parked the function on 2026-10-16 after escalation, preserving L046 instead of
the last 194/238 draft. The historical file
`notes/human-needed-approvals/ovl_19_func_800B95D4.md` records that event and the
196/239 oracle. Its title does not establish that a policy exception is necessary;
its policy-findings section recorded none.

L077 then corrected the opponent reset in the preserved source. L078 gave the
selected arm a separate opponent local. Both reproduced L022, `[0,0,2,7]`.

### Latest continuation (L079–L112)

All named files below are under `build/experiments/ovl_19_func_800B95D4/current/`.
The appendix identifies each source independently; experiments were branched from
appropriate baselines, not necessarily from the immediately preceding file.

- **Identical to the reset-family output L022:** narrowing `angle`, using `% 4`,
  repairing Rand's contract, flattening actor fields, changing the base label,
  explicitly assigning the remainders, an `if/else` opponent selection, a full-width
  temporary for the returned angle band, separating the selected opponent,
  typing the opponent pointer, making the early success arm `continue`, and a
  conditional index expression. These are not new unsuccessful compiler states.
- **Same residual, different output:** `split-turn.c` gives the command-4 sum a
  separate halfword local; `index-fields.c` indexes actor fields directly from
  the base. The latter changes measured reference counts but not the desired
  allocation order.
- **Carried-opponent variants:** `carry.c` and `carry-for.c` reproduce L046.
  `carry-walk.c` and `carry-selected-p.c` form another output class with the same
  key and score; no exact-count fix results.
- **Worse population/control:** overwriting `p` without a separate scan;
  inlining/removing the invariant base; recomputing `view`; deriving `p` from
  `view`; declaring the base inside the loop; a short loop index; a typed but
  indexed view; choosing a separate integer opponent index; and a conditional
  carried reset. In `sum-result.c`, storing the call result into a halfword before
  adding the field introduced population differences. Making that temporary
  full-width returned to L022 (`sum-result32.c`).
- **Operand-order gain only:** `integer-base.c` uses an integer byte-address base
  and forms the selected address as `choice*72 + base`. It fixes the operand
  order at `0x800B9710` and reaches 177/238, but remains `[0,0,2,7]`.
  Making the opponent or current actor address integer-valued, or explicitly
  assigning `base+72`, reproduces that output. Keep this as an operand-family
  result, not an allocation improvement.
- **Birth interaction:** `walk-birth.c` initializes `p` before deriving `view`
  from it; scheduling worsens to 5. This is already measured, not an open simple
  birth-order guess.

`reset.c` was also rechecked but did not add a new ledger row: it is byte-for-byte
source-identical to L077. `preserved.c` is source-identical to L046. Rechecking
`selected-opponent-p.c` and `integer-base.c` after restoring the stub likewise
confirmed existing measurements rather than creating new source hashes.

## Why the residual is an allocation problem in the measured near-misses

### Target roles and the reset correction

| Role | Target hard register |
| --- | --- |
| Early origin / selected actor | `s0` |
| Direction result | `s1` |
| Current actor scan | `s2` |
| Main and selected opponent | `s3` |
| State base | `s4` |
| Constant one | `s5` |
| Actor-array base | `s6` |
| Actor index | `s7` |
| Independent walking state view | `fp` |

The target loop top is:

```asm
800B9618  move  s1,zero
800B961C  bnez  s7,...
800B9620  move  s3,s6       # delay slot
800B9624  addiu s3,s3,72
```

The corrected source operation is to reset the opponent at the top of **each**
iteration and add one record only for actor zero. The preserved carried-`q`
source instead leaves a preheader copy and a tail reset. Its superior ledger key
does not erase that observed machine difference.

The `reset.c` / `flat.c` family rotates three roles:
`angle -> s2`, `p -> s3`, `other -> s1`. It also originally reversed the operands
of the selected-address `addu`. The separate scan/opponent family has angle in
`s1` already, leaving `p -> s3` and both opponent roles in `s2`.

### Candidate-side compiler measurements

Compiler source `gcc/global.c:allocno_compare` was read. Its priority is:

```text
trunc(floor_log2(n_refs) * n_refs / live_length * 10000 * allocno_size)
```

Ties use the lower allocno number. These are allocator statistics: references are GCC's loop-weighted counts,
not a direct count of source occurrences, and live lengths use the dump's units.
The tables describe **candidate** states; the
original's pseudo identities and priorities are not recoverable from the final
register names alone.

For the corrected body-wide-opponent family (`truth.c` / `flat.c`):

| Role | References | Live length | Priority | Assignment |
| --- | ---: | ---: | ---: | --- |
| Early origin / selected actor | 18 | 43 | 16744 | `s0` |
| Opponent | 34 | 119 | 14285 | `s1` |
| Angle | 42 | 170 | 12352 | `s2` |
| Actor GIV | 47 | 388 | 6056 | `s3` |

Evidence: `current/trace.txt`, plus
`build/allocatorCounterfactual/ovl_19_func_800B95D4/summary.txt` and its
`analysis.json`. The counterfactual verifies this candidate's allocation order;
its inferred target/candidate temporary correspondences should not all be treated
as exact. Under this fixed state, outranking the opponent with angle would need
at least 49 references at the same lifetime, or lifetime at most 147 with the same
references. Making the actor outrank angle would need at least 80 references, or
lifetime at most 190. These inequalities are **conditional**, not prescriptions
to add dummy operations and not proof that clean C is impossible.

For `selected-opponent-p.c`, the fresh trace in `current/selected-trace.txt` gives:

| Role / pseudo | References | Live length | Priority | Assignment |
| --- | ---: | ---: | ---: | --- |
| Origin / 86 | 18 | 43 | 16744 | `s0` |
| Angle / 82 | 42 | 168 | 12500 | `s1` |
| Selected opponent / 94 | 12 | 29 | 12413 | `s2` |
| Main opponent / 95 | 22 | 91 | 9670 | `s2` |
| Actor scan / 92 | 47 | 392 | 5994 | `s3` |

The two opponent values have separate, nonoverlapping lifetimes and can share
`s2`. Angle narrowly outranks the selected opponent. This explains why copying
just one spelling from a different baseline can lose the angle gain.

Additional traces were retained for two distinct families:

- `current/index-trace.txt`: direct base-indexed fields change opponent references
  from 34 to 32, its live length to 120, angle's to 172, and the actor GIV's to
  390. That is a real compiler-state change, but allocation remains wrong.
- `current/split-trace.txt`: the separate `turn` local has 20 references over 19
  instructions and is allocated to `v1`; the final angle result remains in `s2`.
  Splitting it does not reproduce the target's common `s1` role.

No global-allocation impossibility proof was obtained. The statement in the old
live comment that a pin would probably force a match was speculation: no pinned
candidate was tested or authorized in this continuation.

## Diagnostic and search results, with their actual limits

### Pipeline reversal, scheduling, and loop emission

On `reset.c`, reversal agrees at the masked `lreg` population waypoint and
identifies the global/local allocation stage as the oldest disagreement.
Machine counts are 238/238. The prologue's two transposed save slots coexist
with the register rotation; they must not automatically be treated as independent
source-statement-order problems.

The reconstruction's pre-delay-slot round trip **failed**: 15 of 225 reconstructed
instructions could not be placed in the compiler's `.mach` chain, first at
`div zero,a0,v0`. Consequently its ordering decisions are provisional. The
same comparison did validate the candidate pseudo population. See the source-
scoped reports under
`build/pipelineReversal/ovl_19_func_800B95D4/objective/`.

Target-schedule analysis replayed block 1's scheduler selections, but also warned
of web-parity/correspondence failures, including differently rendered symbol
aliases and unresolved target `jal` names. Its many proposed delay-slot moves
are not 119 independently proven source defects. This limited correspondence
also restricts what the subsequent synthesis can establish.

Several helper-bearing alternate sources also produced the diagnostic
`RTL dumps do not belong to the observed function`, leaving web analysis
unavailable. The single-function `flat.c` family avoids the helper. An unavailable
web analysis is neither a passing web-parity check nor proof of a semantic defect.

Loop-preheader triage gave an emission-order signal for a later source. The
older checkpoint records a source-scoped target-loop-emission distance of zero
on `c6031b4f`, and a hoist sweep unable to identify an attributable decline goal
or source window. These are different source-scoped observations, not a universal
contradiction or a proof that every loop spelling is exhausted. The retained
loop trace is under `build/loopTrace/ovl_19_func_800B95D4/`; its provenance must
be checked before importing a threshold into another candidate.

### Bounded construction and source-shape searches

| Attempt | Observed result | What it does **not** establish |
| --- | --- | --- |
| Reconstruction engine, requested budget 64 | Retained `result.json`: `unsupported-target`, symbolic-execution state budget 4096 exhausted, **0 candidates / 0 compiles** | No clean-C impossibility; the constructor did not reach a compilable domain. |
| Flexible family transfer | Earlier checkpoint records no eligible matched donor; no target-specific family artifact was located during this documentation pass | No assertion that a hand-authored reconstruction or future donor cannot work. |
| First source-shape synthesis | 18 alternatives + baseline, **19/19 products**, no exact result; coverage **3/136** requirements | Only the contiguous top-level prologue was modeled, stopping before the compound statement at line 37. |
| Latest source-shape synthesis | 18 alternatives + baseline, **19/19 products**, no exact result; coverage **4/119** requirements | Only the prologue, stopping at line 32; no exhaustive loop-body, declaration, or web-partition search. |
| Idiom search on target block 1, tier 0 | No rare-shape-run precedent in the matched corpus | A three-instruction-block query, not proof of a unique or handwritten function. |
| Residual-signature lookup | Returned generic prologue/store and loop examples, some broad multi-edit histories | No demonstrated donor edit closed this function's allocation fight. |

Construction evidence:

- `build/matchingReconstruction/ovl_19_func_800B95D4/result.json`
- `build/sourceShapeSynthesis/ovl_19_func_800B95D4/fde88c7fb43a1f68/summary.txt`
- `build/sourceShapeSearch/ovl_19_func_800B95D4/ff8fbb23b520c38f/summary.json`
- `build/sourceShapeSynthesis/ovl_19_func_800B95D4/0a6fece754520041/summary.txt`
- `build/sourceShapeSearch/ovl_19_func_800B95D4/3b89e5b0607c13f9/summary.json`

The syntheses used caps of 64 and 256 respectively, depth 2; neither cap turned
the small, conservative prologue grammar into an exhaustive source domain.
No allocator SAT proof, local-allocation phantom solver, scheduler SAT search,
or complete loop-body source grammar run is recorded in the inspected evidence.
None should be described as UNSAT.

### Flag matrices

No encoded target fingerprint nominated a flag. The matrices were run against
an earlier near-miss, the flat reset source, the carried selected-opponent
alternative, and `selected-opponent-p.c`. For the three latest sources:

| Flags | Flat reset | Carried selected-opponent | Named selected-opponent |
| --- | --- | --- | --- |
| Baseline | 175 masked, 238 instructions | 174 masked, 239 instructions | 193 masked, 238 instructions |
| `-fno-gcse` | Ties baseline | 5 masked, 247 instructions | 4 masked, 246 instructions |
| `-fno-schedule-insns -fno-schedule-insns2` | Worse | Worse | Worse |
| `-fno-schedule-insns2` | Worse | Worse | Worse |
| `-fno-gcse` plus both scheduling passes off | Worse | Worse | Worse |
| `-fno-rerun-cse-after-loop` | Ties baseline | Ties baseline | Ties baseline |
| `-mno-split-addresses` | Worse | Worse | Worse |
| `-fno-cse-follow-jumps` | Ties baseline | Ties baseline | Ties baseline |
| `-fno-cse-skip-blocks` | Ties baseline | Ties baseline | Ties baseline |
| Both CSE traversal flags off | Ties baseline | Ties baseline | Ties baseline |

Masked flag-probe scores are not the relocated-byte oracle's word matches.
The full latest matrix is preserved in `current/alternate-flags.txt`; the
single `build/flagProbe/ovl_19_func_800B95D4/report.json` path is overwriteable.
There was no dominant flag column and no override was justified or installed.
These experiments close only their measured source/flag columns. In particular,
`-fno-strength-reduce` was **not** measured by these matrices. Unrun flag or
compiler-version speculation is not an attempted solution.

## What remains open / what not to repeat

1. Preserve the correct-reset and angle-correct branches separately. The lowest
   ledger key belongs to the extra-instruction family; do not promote it merely
   by ranking that key or its raw match percentage.
2. The unexplained decision is still how clean source yields the target's
   simultaneous current-actor/opponent roles. Allocation priorities above specify
   the measured candidate's problem, not the original's source-variable partition.
   Source representation, alias/induction structure, and meaningful sequential
   local reuse remain premises to challenge; no one of them has been proved absent.
3. Recheck the source hash before using an allocation statistic or a search
   closure. Paths such as `draft.c`, compiler-trace directories, target-schedule
   directories, and flag reports have been reused and can describe another source.
4. Do not repeat the identical-output families in the appendix as new experiments.
   In particular, `%` versus multiply-back remainder, pointer qualifiers, ordinary
   `register`, ternary selection, typed/flat views, or integer-address spelling
   alone have already been measured in the indicated baselines. Equality of final
   output does not prove equality of every intermediate compiler state, but it
   does not count as byte progress either.
5. The generator's alias error and sound-wrapper void detector have concrete
   explanations. Do not erase sound-handle stores, invent a static-chain contract,
   or infer a policy exception from those diagnostics.
6. There is positive evidence for an allocation-owned residual, but no general
   clean-C no-go theorem and no demonstrated need for register pinning, inline
   assembly, or a per-file flag change. The earlier “human decision needed” and
   “not a compiler quirk” labels should not be read as stronger proofs than the
   evidence recorded here.

## Artifact map and reproducibility

Project/toolchain facts belong to `configs/project-profile.md`. The measured
overlay column is GCC 2.95.2-psx, `-O2 -G0`, with the configured assembler; do not
substitute the executable's small-data column or raw cc1 text for the byte oracle.

- Immutable measured source snapshots:
  `build/experimentLedger/sources/ovl_19_func_800B95D4/<16-digit-source-hash>.c`.
  The appendix uses this filename stem. All 112 snapshots were present and their
  full SHA-256 hashes were checked against the ledger when this note was written.
- Named historical branches: `build/experiments/ovl_19_func_800B95D4/`.
  `baseline-typed.c` = L058; `draft.c` = L017; `dual-pointer.c` = L010;
  `indexed-view.c` = L009; `live-baseline.c` = L022;
  `selected-default-flip.c` = L073; `selected-opponent-p.c` = L070;
  `duplicate-switch-tails.c` = L074; `inline-actor-index.c` = L076.
- Latest experiment files and retained trace transcripts:
  `build/experiments/ovl_19_func_800B95D4/current/`.
  `preserved.c` = L046; `reset.c` = L077; all other measured `.c` files are named
  in the appendix. `live-original.c` is the backup of the live stub/disabled
  attempt, not a compiling-C experiment.
- Earlier narrative/checkpoint: `build/experiments/ovl_19_func_800B95D4/checkpoint.md`.
  It describes an intermediate state and must not be treated as the current live
  source. Its suggested Rand audit was subsequently performed (L056–L058, L081).
- Historical parking note: `notes/human-needed-approvals/ovl_19_func_800B95D4.md`.
- Latest closed-direction entries are printed by
  `psx_experiment_ledger` / `psx_record_closed`; each closure is conditioned on
  its source and search bounds, as summarized above.

`build/` evidence is local and may be cleaned or overwritten; this note preserves
its important results and conditions, not a promise that every scratch artifact
will exist in a fresh checkout. No binary or generated compiler artifact is
included in this documentation change.

## Complete measurement appendix

The following tables are transcribed from the 112-row ledger. “Output” is the
first entry having that exact compiled-output hash (`new` means first occurrence),
not a similarity judgment. Source-change descriptions come from preserved C diffs;
where a historical change also reverted another branch, the description says so
rather than claiming a single-variable causal experiment. Fractions are the
ledger's recorded alignment scores, **not verified machine instruction counts**.

### 2026-10-16: initial matching campaign (L001–L076)

| ID / UTC time | Source hash / snapshot | Source experiment or restored branch | Residual | Ledger score | Output |
| --- | --- | --- | --- | --- | --- |
| L001 05:48:24 | `169d06fbafce79e1` | Initial manual draft; indexed actors/state, switch bodies 0/1,2,3 | `[0,73,3,11]` | 159/228 | new |
| L002 05:49:03 | `661211558f2a3f9d` | Named early origin and call result; reuse origin for fallback indexed reads | `[0,80,7,22]` | 97/218 | new |
| L003 05:49:48 | `7cecdad90749bec0` | Reuse state pointer for early origin; fallback base-derived reads | `[1,80,5,20]` | 104/211 | new |
| L004 05:50:05 | `06a7140b33715501` | Separate early origin again, retaining base-derived indexed fallback | `[1,104,5,23]` | 119/213 | new |
| L005 05:50:42 | `5bac21caf046ef3e` | Wrap fallback indexed reads in a 36-halfword record type | `[1,104,5,23]` | 119/213 | L004 |
| L006 05:51:00 | `ee6a1905624e90db` | Walk actor pointer independently of indexed state view | `[0,62,6,21]` | 127/222 | new |
| L007 05:51:10 | `c6031b4f56a62a92` | Move switch case 3 body before case 2 | `[0,0,6,17]` | 128/225 | new |
| L008 05:52:31 | `7b18374dcac1fc29` | Separate initial `status` from later `choice` | `[0,0,6,13]` | 148/233 | new |
| L009 05:53:07 | `895d52d1a94139cd` | Explicit index zero / constant one births before invariant base | `[0,0,5,12]` | 146/238 | new |
| L010 05:53:22 | `b2a13fef000c70a5` | Independently walk fallback `view` alongside actor pointer | `[0,0,3,11]` | 156/237 | new |
| L011 05:53:54 | `a503466d99e03759` | Reuse early origin for selected actor instead of fresh `selected` | `[0,0,3,7]` | 175/237 | new |
| L012 05:54:09 | `c681d3b7da8755f2` | Separate selected opponent from main `q` | `[0,0,3,7]` | 175/237 | L011 |
| L013 05:54:27 | `ad9c3afb80e36733` | `q = base + 36` instead of `q += 36` | `[0,0,3,7]` | 175/237 | L011 |
| L014 05:54:39 | `985d716e22dad023` | Index current actor each iteration while explicitly walking `view` | `[0,0,2,7]` | 175/238 | new |
| L015 05:55:36 | `372807403548c2a3` | Birth selected opponent before the selection call | `[0,0,5,5]` | 182/236 | new |
| L016 05:56:16 | `3bc144da554fa1f7` | Reuse `q` for that early-born selected opponent | `[0,0,5,9]` | 164/236 | new |
| L017 05:57:24 | `20b325b1359cd8fd` | Ternary main-opponent selection in L016 family | `[0,0,5,9]` | 164/236 | L016 |
| L018 06:00:25 | `9e0c64a10a597c29` | In-loop actor base with externally initialized view; restore selected-opponent local | `[0,19,6,7]` | 134/234 | new |
| L019 06:01:02 | `bb207d96483ea674` | Restore external base and derive view from `D_800BF4C0 + 8` | `[0,0,2,7]` | 175/238 | L014 |
| L020 06:01:14 | `6a8cc7a75c674c7c` | Full-width angle with explicit halfword truncations | `[0,0,2,7]` | 175/238 | L014 |
| L021 06:01:24 | `f8f757c9766c224c` | Two-step `p = base; p += i*36` | `[2,146,6,24]` | 127/234 | new |
| L022 06:02:03 | `0af17a72192e6b14` | Restore one-expression indexed `p`; use do/while | `[0,0,2,7]` | 176/238 | new |
| L023 06:02:12 | `89d6b374b0a22e82` | Explicitly walk `p` with `view` in L022 family | `[0,0,2,7]` | 176/238 | L022 |
| L024 06:02:36 | `5b3b574a55f98528` | Ternary selected-opponent choice | `[0,0,2,7]` | 176/238 | L022 |
| L025 06:03:04 | `08b7ffdfd06d23e7` | Restore indexed `p`, then overwrite it with selected actor | `[0,15,6,10]` | 140/238 | new |
| L026 06:03:17 | `764318c112c9b2af` | Restore selected actor to `origin` | `[0,0,2,7]` | 176/238 | L022 |
| L027 06:03:42 | `0304233c72d041eb` | Earlier selected-opponent birth plus direct global selected-actor origin | `[0,1,5,5]` | 168/232 | new |
| L028 06:04:21 | `5ab5750a20ea95df` | Restore origin/birth placement; spell remainders with divide/multiply/subtract | `[0,0,2,7]` | 176/238 | L022 |
| L029 06:05:24 | `67c395f86854f012` | Reuse `q` as final state/duration base | `[0,6,2,11]` | 163/236 | new |
| L030 06:06:09 | `a75f1e2060fea7c4` | Restore duration expression; move zero-angle assignment after opponent conditional | `[0,0,2,5]` | 194/237 | new |
| L031 06:07:06 | `3df7f1663151a8b5` | Place zero-angle assignment between default `q` birth and conditional update | `[0,0,2,3]` | 194/238 | new |
| L032 06:07:15 | `5324ae0f0cc99f2d` | Early success uses `else` wrapper instead of `continue` | `[0,0,2,3]` | 194/238 | L031 |
| L033 06:07:35 | `86633a3bd2d5fb3c` | Ordinary, unpinned `register` actor pointer | `[0,0,2,3]` | 194/238 | L031 |
| L034 06:08:57 | `22da96d6aa389901` | Overwrite indexed actor pointer with selected opponent | `[2,146,6,24]` | 128/234 | new |
| L035 06:09:42 | `394b49d3f7507378` | Restore separate opponent; use named typed actor fields | `[0,0,2,3]` | 194/238 | L031 |
| L036 06:10:04 | `f554c41cd482d816` | Short actor-loop index | `[0,2,2,8]` | 173/238 | new |
| L037 06:11:06 | `eafb0973403cdd1e` | Restore full-width index; loop-local initialized `p` and `q` | `[0,0,2,3]` | 194/238 | L031 |
| L038 06:11:48 | `f8a23ce0758ba9a3` | Base inside loop and direct global indexed fallback reads | `[0,21,6,15]` | 103/232 | new |
| L039 06:12:51 | `e5b5c9c24cd3e241` | Restore base/view; reuse one explicit offset for `i*36` and `i*32` | `[0,0,2,7]` | 176/238 | new |
| L040 06:15:31 | `25bfad26684706ea` | Separate walking scan plus loop-local alias; remove offset reuse | `[0,0,2,3]` | 194/238 | L031 |
| L041 06:15:54 | `6ef7086a9d82e4e2` | Reuse per-iteration alias as selected actor, retaining scan | `[0,0,2,3]` | 194/238 | L031 |
| L042 06:16:42 | `ff92722f31641be8` | Reuse `q` for final state and recomputed final actor argument | `[0,22,7,10]` | 153/225 | new |
| L043 06:17:10 | `d0332af67ffe8ace` | Restore final call; use `other = q` alias throughout main body | `[0,0,2,3]` | 194/238 | L031 |
| L044 06:18:53 | `8d8fbfd096037d46` | Inline `actorAt` helper without scan; overwrite `p` for selected actor | `[0,15,6,10]` | 140/237 | new |
| L045 06:19:10 | `ec2c898a1bc77c02` | Keep helper but restore selected actor in `origin` | `[0,0,2,3]` | 194/238 | L031 |
| L046 06:21:21 | `c2529328f13cd5bd` | Carry `q` across iterations with preheader initialization and tail reset | `[0,0,1,6]` | 196/236 | new |
| L047 06:21:32 | `be3ce105f5e0e74a` | Remove inline helper from carried-`q` family | `[0,0,1,6]` | 196/236 | L046 |
| L048 06:22:07 | `b3b84d77f0eaf316` | Carried `q += 36`, reset in `else`, no tail reset | `[0,21,6,12]` | 121/234 | new |
| L049 06:22:18 | `9fa9a6dd00194945` | Restore loop-local initialized `q` and explicit `base+36` | `[0,0,2,3]` | 194/238 | L031 |
| L050 06:23:13 | `1989de00a72caf8a` | Merge fresh `q` into directly assigned body-wide `other` | `[0,0,2,7]` | 176/238 | L039 |
| L051 06:26:31 | `e08d3a9f1df729dd` | Integer byte-address actor macro; restore initialized `q` plus alias | `[0,0,2,3]` | 194/238 | L031 |
| L052 06:27:23 | `d7e75c4b32c0afa1` | Remove address macro; direct `D_800BF4D0` base label | `[0,0,2,3]` | 194/238 | L031 |
| L053 06:28:35 | `2371b3f8c7c652f0` | Walking scan and per-iteration actor alias | `[0,0,2,3]` | 194/238 | L031 |
| L054 06:28:53 | `e290c48aad1db370` | Transient actor-pointer advance/read/restore for early angle load | `[0,0,2,3]` | 194/238 | L031 |
| L055 06:29:35 | `b029907ec0adac23` | Restore direct angle field/indexed actor; make loop-local pointer `const` | `[0,0,2,3]` | 194/238 | L031 |
| L056 06:34:23 | `3a40872fe6ddcfab` | Actual `Rand` name / unsigned return; explicit signed comparison | `[0,0,2,3]` | 192/238 | new |
| L057 06:34:42 | `acf157932f4afc31` | Restore overlay alias, retain witnessed unsigned return and signed comparison | `[0,0,2,3]` | 194/238 | L031 |
| L058 06:35:50 | `ce6ee29c8d70bf4f` | Matched callee record views/casts instead of `void *` erasures | `[0,0,2,3]` | 194/238 | L031 |
| L059 06:36:23 | `994523076e24ba82` | Value-returning target signature, returning `i < 2` | `[0,1,2,3]` | 194/238 | new |
| L060 06:38:15 | `89e6718a8f0dfaed` | Restore void; walking scan; reassign actor alias in selection arm | `[0,0,2,3]` | 193/238 | new |
| L061 06:38:52 | `a68132500084f777` | Derive default opponent base from `scan - i` | `[0,17,6,3]` | 165/234 | new |
| L062 06:39:15 | `b661f2772d31e261` | Remove `q` alias; assign body-wide opponent directly | `[0,0,2,11]` | 156/238 | new |
| L063 06:39:57 | `c2286eca4d15e55a` | Explicit main-opponent `if/else` assignments | `[0,0,2,11]` | 156/238 | new |
| L064 06:41:02 | `a38cf43025e65556` | Index `view` instead of walking it | `[0,0,3,11]` | 156/236 | new |
| L065 06:43:19 | `6b73566146aa27e0` | Restore fresh `q` plus alias within indexed-view family | `[0,0,3,3]` | 193/236 | new |
| L066 06:44:30 | `c8b1852754eba310` | Reuse `q` for selected opponent, then copy into `other` | `[0,0,3,11]` | 156/236 | new |
| L067 06:45:48 | `a7d2ddc6e505cec4` | Carry walking view through `other`; restore indexed actor/selected origin | `[0,1,5,4]` | 172/229 | new |
| L068 06:46:07 | `584c2b82e43c88e3` | Add explicit actor scan to L067 view-carry family | `[0,1,5,4]` | 172/229 | new |
| L069 06:48:23 | `6550221556efc03b` | Reuse angle as `i*72` byte offset; restore independent view | `[2,147,6,24]` | 127/234 | new |
| L070 06:51:53 | `01893a5bfe0244d8` | `selected-opponent-p.c`: scan plus alias reused as selected opponent | `[0,0,2,3]` | 193/238 | L060 |
| L071 06:54:38 | `1de5a29bcd95be63` | Indexed actor; selected opponent update via `other += 36` | `[0,2,2,7]` | 176/238 | new |
| L072 06:55:01 | `10d4057f218529d1` | Restore selected opponent as `state + 0x2C` | `[0,0,2,3]` | 194/238 | L031 |
| L073 06:55:10 | `0b425444bc5e765e` | Main fresh `q += 36` instead of `q = base + 36` | `[0,0,2,3]` | 194/238 | L031 |
| L074 06:55:39 | `a26eff22263f2770` | Duplicate final command call into switch arms and default | `[0,25,5,19]` | 150/228 | new |
| L075 06:58:00 | `61ee3872317d20c1` | Restore shared switch tail; unused saved-command local is eliminated | `[0,0,2,3]` | 194/238 | L031 |
| L076 07:02:19 | `5b1b7778e767d7eb` | Inline actor-index expressions at all uses instead of named `p` | `[0,0,2,7]` | 176/238 | new |

### 2026-10-23: reset diagnosis and continuation (L077–L112)

`current/` filenames in this table are relative to
`build/experiments/ovl_19_func_800B95D4/`. L077–L078 originally used temporary
scratchpad paths; their immutable snapshots, and L077's `current/reset.c`, are
preferable to those temporary paths.

| ID / UTC time | Source hash / snapshot | Source experiment / named file | Residual | Ledger score | Output |
| --- | --- | --- | --- | --- | --- |
| L077 17:56:59 | `1446f821f51af8cb` | Correct top-of-iteration opponent reset in L046; `current/reset.c` | `[0,0,2,7]` | 176/238 | L022 |
| L078 17:58:27 | `a03af95cc57ef4d7` | Separate selected-arm `loser` in L077 | `[0,0,2,7]` | 176/238 | L022 |
| L079 18:08:24 | `46f3e8a7aebbe65d` | `current/angle16.c`: halfword angle in reset family | `[0,0,2,7]` | 176/238 | L022 |
| L080 18:10:18 | `87dc09747e36f432` | `current/mod.c`: use `% 4` for remainders | `[0,0,2,7]` | 176/238 | L022 |
| L081 18:10:43 | `a8da2416cbd6ad81` | `current/truth.c`: correct Rand alias return, preserve signed comparison | `[0,0,2,7]` | 176/238 | L022 |
| L082 18:11:49 | `b636f76825719a76` | `current/flat.c`: flat actor fields/indexing, remove inline helper | `[0,0,2,7]` | 176/238 | L022 |
| L083 18:12:13 | `a3c89440f84f4a8b` | `current/walk.c`: explicitly walk flat actor pointer | `[0,0,2,7]` | 175/238 | L014 |
| L084 18:12:50 | `7f43b9226983b07e` | `current/carry.c`: initialize opponent before loop and reset at tail | `[0,0,1,6]` | 196/236 | L046 |
| L085 18:17:58 | `0fb1297232c08435` | `current/reuse-p.c`: overwrite indexed actor with selected actor | `[0,15,6,11]` | 131/238 | new |
| L086 18:18:47 | `f43438b45a81aa62` | `current/label.c`: direct `D_800BF4D0` actor base | `[0,0,2,7]` | 176/238 | L022 |
| L087 18:19:41 | `18bb8e1170dc145a` | `current/assign-mod.c`: assign first two remainders back to angle | `[0,0,2,7]` | 176/238 | L022 |
| L088 18:20:19 | `b8be38fe444c28c1` | `current/inline-base.c`: inline all invariant actor-base uses | `[0,4,4,13]` | 144/236 | new |
| L089 18:20:48 | `6b2733e3cb9512b1` | `current/if-other.c`: explicit opponent `if/else` instead of reset/update | `[0,0,2,7]` | 176/238 | L022 |
| L090 18:21:46 | `2b356d21f64520a9` | `current/split-turn.c`: separate command-4 sum local | `[0,0,2,7]` | 176/238 | new |
| L091 18:23:06 | `86fbc3cf80e67fce` | `current/index-view.c`: recompute view from indexed base instead of walking | `[1,118,6,16]` | 132/227 | new |
| L092 18:23:53 | `41ec94143c6094b8` | `current/carry-for.c`: for-loop version of carried opponent | `[0,0,1,6]` | 196/236 | L046 |
| L093 18:24:42 | `a0320243da9de158` | `current/index-fields.c`: actor field addresses indexed directly from base | `[0,0,2,7]` | 176/238 | new |
| L094 18:25:01 | `220204aba03875b6` | `current/sum-result.c`: halfword call result, then add direction field | `[0,6,2,3]` | 171/234 | new |
| L095 18:25:13 | `6b252fea8880ff50` | `current/sum-result32.c`: full-width intermediate in L094 | `[0,0,2,7]` | 176/238 | L022 |
| L096 18:25:52 | `deddcee9f7ff1381` | `current/split-other.c`: selected opponent gets separate local | `[0,0,2,7]` | 176/238 | L022 |
| L097 18:26:41 | `20044f2670ff75c9` | `current/typed-other.c`: typed opponent record pointer in L081 family | `[0,0,2,7]` | 176/238 | L022 |
| L098 18:27:33 | `f662a74b20451ad3` | `current/carry-walk.c`: explicit actor walk plus carried opponent | `[0,0,1,6]` | 196/236 | new |
| L099 18:28:12 | `efd892a8b32bbe35` | `current/p-from-view.c`: derive actor as `view + 8` | `[1,101,0,12]` | 179/232 | new |
| L100 18:31:22 | `3f50cc7697e4faca` | `current/guard.c`: early success `continue` rather than `else` wrapper | `[0,0,2,7]` | 176/238 | L022 |
| L101 18:34:16 | `34351df85b661923` | `current/inner-base.c`: actor base inside loop, initial view at global base | `[0,4,4,13]` | 144/236 | new |
| L102 18:35:13 | `9f8e3eac7b928aad` | `current/i16.c`: halfword loop index | `[0,2,2,8]` | 173/238 | new |
| L103 18:36:53 | `d062da25bdacc3d7` | `current/typed-index-view.c`: typed actor plus recomputed indexed view | `[1,118,6,16]` | 132/227 | L091 |
| L104 18:37:41 | `3954a2bf0fb1d8c9` | `current/conditional-index.c`: `base + (i == 0 ? 36 : 0)` | `[0,0,2,7]` | 176/238 | L022 |
| L105 18:38:00 | `7413604aa831722c` | `current/other-index-local.c`: choose integer opponent index in separate statements | `[2,302,8,61]` | 175/234 | new |
| L106 18:40:54 | `304c9656b18ec22d` | `current/integer-base.c`: integer byte-address base and selected offset-first sum | `[0,0,2,7]` | 177/238 | new |
| L107 18:41:12 | `f5562c6d251f5352` | `current/integer-other.c`: opponent address integer-valued too | `[0,0,2,7]` | 177/238 | L106 |
| L108 18:41:31 | `18f0c0f315ac308d` | `current/integer-p.c`: current actor address integer-valued | `[0,0,2,7]` | 177/238 | L106 |
| L109 18:43:07 | `817264dfa59937a7` | `current/walk-birth.c`: initialize walking actor before deriving view from it | `[0,0,5,8]` | 164/238 | new |
| L110 18:43:40 | `b257a7ab38b4eab4` | `current/explicit-base.c`: explicit `base + 72` opponent update in integer-base family | `[0,0,2,7]` | 177/238 | L106 |
| L111 18:44:20 | `26f18a6baef6b117` | `current/carry-selected-p.c`: carry `q` in L070's scan/selected-opponent family | `[0,0,1,6]` | 196/236 | L098 |
| L112 18:45:36 | `254f8764444b11fb` | `current/conditional-reset.c`: carried `q += 36`, reset in `else`, no tail reset | `[0,21,6,12]` | 121/234 | new |
