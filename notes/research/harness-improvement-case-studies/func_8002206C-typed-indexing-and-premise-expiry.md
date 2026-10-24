# Case study: func_8002206C, typed indexing, and expiring search premises

**Function:** `func_8002206C`, executable container.

**Study date:** 2026-10-24.

**Outcome:** byte-exact ordinary C, 200/200 target words, staged residual
`[0,0,0,0]`. The authoritative finalizer passed the exact diff, full build,
modification-scope check, and clean-source gate. No target flag override,
embedded assembly, hard-register pinning, compiler modification, or artificial
runtime computation was needed.

**Scope:** a retrospective and harness-improvement proposal. This review reads
existing sources, measurements, and compiler dumps; it does not rerun the
solver, alter build inputs, implement the proposed tools, or authorize a
commit. No new original translation-unit membership is inferred.

The shorter [function research note](../func_8002206C.md) records the accepted
implementation. This study explains why the last correction worked, why it
was easy to overlook, and how the harness could make the correction easier to
find without weakening its verification discipline.

## Executive summary

The successful continuation inherited a nearly solved function. Its remaining
problem appeared to be three scheduling differences and one register-allocation
difference in the card-dealing block. Earlier work had already recovered the
object layout, declarations, loop structure, position calculation, and weights.

The final fix replaced a reconstructed byte-address accumulator and an explicit
byte-offset expression with a pair of natural halfword-array accesses:

```c
D_800558A4[v * 2]
D_800558A4[(v * 2) | 1]
```

That single representation change made the whole function exact. A redundant
mode-value narrowing, previously useful for scheduling, could then be removed
without losing the match. Cross-block reuse of the colour-address scratch also
ceased to be necessary because that scratch was no longer used in the deal
loop.

The saved compiler dumps make the mechanism concrete. At the second CSE
boundary, the first access uses a shift by two, whereas the second uses
shift-by-one, OR-one, shift-by-one. At the combine boundary, the latter becomes
a *new* shift-by-two followed by OR-two. The final duplicate shifts therefore
do not require an explicitly overwritten source accumulator to defeat CSE.
The natural expressions also produce independent single-set address pseudos;
the scheduler promotes them, and ordinary local allocation assigns the target
registers.

There is an important correction to the tempting story that a later agent
simply thought of a natural expression that earlier agents missed:

> The exact same pair of halfword-indexed expressions was already present in
> `build/func_8002206C/native-color-indexing.c`, measured on 2026-10-22.
> That complete source was not exact. It still had the older manual-pointer,
> goto-based deal loop and other unresolved structure.

The winning construction was not new in isolation. It became successful in a
corrected surrounding program. The central harness lesson is thus **premise
expiry and transformation replay**, not merely “try array indexing.” A failed
complete program is not a permanent rejection of every local construction it
contains.

## 1. Evidence boundary and identities

### 1.1 What this review relies on

- Verified active source: `src/func_8002206C.c`.
- Supplied finalization result: exact diff, full build, scope, and clean-source
  gates all passed.
- Historical continuation and handoff under `build/func_8002206C/current/`.
  Their new resolution banners supersede the older open-problem text below.
- Complete candidate snapshots and `build/experimentLedger/func_8002206C.jsonl`.
- Saved per-candidate residual reports under
  `build/pipelineReversal/func_8002206C/objective/`.
- The near-match scheduler dumps under `build/schedulerTrace/func_8002206C/`.
- The exact candidate's dumps and provenance under
  `build/loopTrace/func_8002206C/`.
- The saved, source-scoped SAT output
  `build/func_8002206C/current/near-state-solver.txt`.
- `birthing_insn_p` and `adjust_priority` in the vendored compiler's `sched.c`.

The `build/` artifacts are local generated evidence and may disappear after a
clean. They are not artifacts to commit. Some diagnostic directories are
mutable latest-run locations: inspect their source identity before reusing
pseudo numbers or conclusions. In particular, the current state-solver
`summary.json` describes an older, different request set than the preserved
near-match text report. This study uses the latter for the near-match SAT
claim, rather than treating the directory name as a sufficient identity.

### 1.2 Identity table

| Evidence | Identity |
| --- | --- |
| Final verified source SHA-256 | `934c9b6a1c074c67493bcfff50ee8943d5d6a0b05eb260ce958559911e6c25d9` |
| Supplied finalization identity | `7ab5212139dc6a720af711ff8d6aed5ed762ac7941ab4cd56216da543c54e85f` |
| First exact scratch source, `natural-colour-pair.c` | `fc26a73a0055e567591d89f30d6ae9b6a77012a0d8d3b50a6ef234e5986c9372` |
| Exact scratch source without narrowing, `natural-colour-no-narrow.c` | `d941b3d465dd0403af9f4328a4479b681b6e6c4f2f158808d07f18abff0d8606` |
| Exact relocated-output ledger fingerprint | `bf4dafb5b2cefc2182a541ee0deaf880e90396ca906910f9a43c71ce67244ff1` |
| Earlier natural-index source, `native-color-indexing.c` | `59fc81dac8d3c53bb47c916a993fdd9a9c6691a62e899fbede45e95039fa6817` |

These identify different things. The finalization identity is not the source
hash, and the output fingerprint is not the checksum of the linked executable.
Concrete toolchain and flag facts remain authoritative in
`configs/project-profile.md`.

The exact dump set is explicitly attributed by `build/loopTrace/func_8002206C/trace.json`
to `natural-colour-no-narrow.c`, with the matching source hash above. It is not
silently attributed to the final source with renamed locals and retained
historical comments. Both were independently measured exact.

### 1.3 Measured scale, not an invented cost comparison

Before the continuation's first newly recorded candidate, the inspected ledger
had **158 rows and 97 distinct output fingerprints**. After integration and the
final comment-only measurement, it had **170 rows and 103 output fingerprints**.
The first exact candidate was the **ninth newly recorded candidate** in that
continuation, not the first experiment.

These are ledger counts, not total compiler invocations, agent turns, tokens,
or historical cost. Diagnostic compiles and older work outside this ledger
are not counted. A quick final correction does not establish that the previous
work was unnecessary or that another agent would have solved the original
starting state equally quickly.

## 2. The valuable work inherited by the successful continuation

The function clears a 0x108-byte card-game state, deals three rows of six cards,
applies a special slot for mode 3, and computes weights from each row's largest
X position. The continuation inherited several important corrections:

1. **Real object and field declarations.** `CardGameState` describes the cleared
   object, and direct halfword symbol aliases describe the stride-14 field
   walks. Earlier generated array-backed declarations did not behave like the
   direct scalar declarations assumed in some preparation context. This
   affected address formation before allocation was even considered.
2. **Native loop structure.** The deal phase uses nested row, slot, and retry
   loops. The old manual field-pointer/goto reconstruction supplied a different
   loop-optimizer and PRE history. The maximum scan is indexed, with its reset
   inside the row loop.
3. **A consistent state root.** A single state pointer connects the saved mode,
   clearing call, duplicate test, and special-mode destination. This preserves
   the measured address-sharing and rematerialization behavior.
4. **The actual weight operation.** The accepted expression calls `Rand(4)`
   before reading the position and uses ordinary unsigned division by 1000,
   rather than treating the target's multiply-high sequence as a source-level
   primitive.
5. **Independent declaration auditing.** The continuation's callee audit found
   no contradicted signatures. The vendor's old-style `memset` declaration
   remained the declaration in use; no invented prototype or flag was needed
   to force the result.

Those wins left the live source at `[0,0,4,1]`. The best handed-off scratch
candidate, `word-mode-narrow.c`, was at `[0,0,3,1]`: only the accepted-card block
remained open in the staged comparison.

The final patch succeeded on top of this context. It does not show that
substituting two array reads into the original parked attempt would have been
sufficient. The earlier natural-index experiment is direct evidence that it
was not.

## 3. The near-match and the misleading construction it encouraged

### 3.1 The residual was genuinely small

After the duplicate-test branch's first-colour shift, the target placed the
card-value store and its pointer/counter steps before the first table-address
materialization and sum. The near-match put that address work before the three
store/step instructions.

The first colour load and store also used the wrong register:

```text
Target:     first address in a1, first loaded colour in a2
Near-match: first address in a1, first loaded colour also in a1
```

The remaining staged key was `[0,0,3,1]`: no reported control-flow or population
difference, three scheduling differences, one allocation difference. The
report's aligned-word denominator was not the function's physical length;
the physical target and candidate each contained 200 words. This review uses
the staged key for the near-match and the terminal 200/200 result for exactness,
not an inferred count of edits from an alignment score.

### 3.2 A real explanation of one candidate became an assumption about all C

The source's first table read was implemented through an explicit accumulator:

```c
colorAddress = v;
colorAddress += colorAddress;
colorAddress <<= 1;
colorAddress += (s32)D_800558A4;
/* load through colorAddress */
```

The second read used:

```c
*(u16 *)((u32)D_800558A4 + ((v * 4) | 2))
```

The explicit accumulator was a multi-set pseudo. In the saved near-match
scheduler trace, its shift and address sum, UIDs 201 and 206, were plain
priority-1 instructions rather than promoted single-set births. Their order
and the later load's lifetime were understandable consequences of this source.

Earlier experiments had also found that reusing this scratch for the mode-3 Y
load improved allocation elsewhere. The handoff correctly preserved that
measured gain, but used language such as “genuinely multi-set” and “preserve it.”
Those are stronger claims than the experiment establishes. The correct scope
was:

> Within the current byte-address-accumulator family, this reuse improves the
> measured result.

It was not:

> Every matching source must have this accumulator or share it with mode Y.

The final source disproves the latter: its mode Y variable has no deal-loop
role at all.

### 3.3 The solver answered a conditional question, not the source question

The preserved near-match SAT report requested `pseudo140 -> a2` and found four
solutions. The smallest added one abstract quantity at allocation slot 8,
live over slots 22..24, with three references and hard register 5.

That is useful evidence about the measured local-allocation state. It is not
evidence that the original C contained a hidden computation, nor permission
to add a fake live value to occupy a register. The final source changed the
address and index webs instead; it needed no phantom quantity.

Likewise, the session's reverse-pipeline report localized the mismatch as
allocation/scheduling, but its pre-delay-slot replay failed for 17 of 193
reconstructed instructions. The report warned that the reconstruction was
provisional. The actual scheduler dump was therefore a better source for the
candidate's ordering mechanism than an unqualified inversion of the target.

The useful distinction is **where the difference is observed** versus **which
earlier source construction caused that difference**.

## 4. The decisive representation change

### 4.1 Treat the two reads as one operation family

The successful edit replaced both byte-address formulations with:

```c
*(u16 *)((u8 *)&D_8007AD54 + rows * 84 + n * 14) =
    D_800558A4[v * 2];
*(u16 *)((u8 *)&D_8007AD56 + rows * 84 + n * 14) =
    D_800558A4[(v * 2) | 1];
```

`D_800558A4` is a table of halfwords, with two halfwords per card. For a valid
card index, `v * 2` is even, so setting its low bit selects the second halfword.
Scaling the element indices by the two-byte element size yields the same
byte offsets as `v * 4` and `(v * 4) | 2`.

This was one hypothesis about a paired table operation, not an unrelated batch
of source edits. Changing only an isolated temporary while retaining the
other byte-oriented expression does not test the same representation.

There is also a crucial compiler distinction between algebraic equivalence
and early-RTL equivalence. The source should not be normalized into the
machine's final byte arithmetic before asking what this compiler emits.

### 4.2 The saved dumps show why duplicate shifts survive naturally

The following table is read from the exact `natural-colour-no-narrow.c` dumps.
Pseudo numbers and UIDs are local to that compile.

| Boundary | First access | Second access |
| --- | --- | --- |
| RTL expansion | UID 225: `r141 = v << 2`; UID 227 forms its address | UID 264: `r160 = v << 1`; UID 266: `r161 = r160 | 1`; UID 270: `r163 = r161 << 1`; UID 272 forms its address |
| First CSE | First shift and address remain; table root is shared | Shift-one / OR-one / shift-one remain |
| Second CSE | UID 225 is still shift-two | UIDs 264/266/270 still represent shift-one / OR-one / shift-one |
| Combine | First shift-two remains | UID 264 is deleted; UID 266 becomes `r161 = v << 2`; UID 270 becomes `r163 = r161 | 2` |

The two final shift-by-two instructions have **different construction
histories**. One was already a shift-two during CSE. The other acquired that
form at the combine boundary. The saved output retains both.

This directly explains why “CSE would share the two shifts unless the source
explicitly overwrote an accumulator” was an unnecessarily restrictive premise.
It describes one way to obtain the final population, not the only way.

The review does not claim a unique original source or a complete trace of each
internal combine rewrite. The named input/output dump boundaries are enough
to witness the relevant transformation.

### 4.3 Single-set address webs recover scheduling

The natural first access has separate pseudos for the scaled index, address,
and loaded halfword. In the exact `.lreg` dump:

```text
r141: index, one SET
r142: address, one SET
r143: loaded halfword, one SET
```

The exact `.sched` ready lists show the shift and sum promoted to
`0x7f000001`, unlike the near-match's plain accumulator operations. The
pre-reload emitted sequence is:

```text
store card value
advance card-value pointer
increment slot counter
first index shift
first address sum
second index shift and OR
...
```

The governing compiler rule is visible in
`tools/vendor/gcc/2.95.2/src/gcc/sched.c`, in `birthing_insn_p` and
`adjust_priority`: before reload, an eligible live destination with
`REG_N_SETS == 1` receives the birthing priority boost. The scheduler works
backwards, so the promoted arithmetic can settle later in emitted order than
the unpromoted store/steps. The final assembly places the first index shift in
the preceding branch's delay slot while retaining the desired store-before-
address order afterward.

This is a witnessed mechanism, not an appeal to how a modern compiler might
schedule equivalent C.

### 4.4 Ordinary local allocation then supplies the target registers

The exact `.lreg` register assignments are:

```text
Register 141 in 5.   /* scaled index: a1 */
Register 142 in 5.   /* first address: a1 */
Register 143 in 6.   /* loaded halfword: a2 */
```

The `.greg` dump agrees: the first shift and address sum use register 5; the
first halfword load uses register 6. This is the required relationship without
register pinning, an extra computation, or the earlier cross-block scratch
reuse.

This review does not reconstruct every local-allocator ranking decision. The
saved assignments and exact final output establish that the desired state is
reachable through the simpler source construction. Solving the old web
partition more exhaustively was not a prerequisite.

## 5. What the successful continuation actually tried

All paths in this table are relative to `build/func_8002206C/current/`. Keys
are recorded `[control-flow, population, schedule, allocation]` tuples. They
are comparison outputs, not a count of source defects or a universal distance.

| Order | Candidate | Parent / question | Recorded key |
| --- | --- | --- | --- |
| Handoff | `word-mode-narrow.c` | Best inherited branch, including signed shift-based narrowing | `[0,0,3,1]` |
| 1 | `unsigned-mode-narrow.c` | Make the narrowing's left-shift unsigned | `[0,0,3,1]` |
| 2 | `cast-mode-narrow.c` | Replace the two shifts with an ordinary `u16` conversion | `[0,0,4,1]` |
| 3 | `fresh-colour-sum.c` | Split a fresh address from the index scratch | `[0,1,1,15]` |
| 4 | `fresh-sum-global-load.c` | On row 3, assign the loaded colour back to the shared scratch | `[0,0,1,19]` |
| 5 | `narrow-colour-load.c` | On row 1, narrow the first loaded colour through unsigned shifts | `[0,2,4,15]` |
| 6 | `signed-mode-convert.c` | On row 1, use a signed halfword conversion in the mode block | `[0,2,3,1]` |
| 7 | `shared-colour-offsets.c` | On row 3, reuse the scratch for both colour offsets | `[0,5,5,17]` |
| 8 | `colour-from-stored-card.c` | On row 1, derive the first index from the just-stored card | `[0,0,3,1]` |
| 9 | `natural-colour-pair.c` | On row 1, replace both reads with natural halfword indexing | **`[0,0,0,0]`, EXACT** |
| Ablation | `natural-colour-no-narrow.c` | Remove the mode narrowing from row 9 | **`[0,0,0,0]`, EXACT** |
| Integration | `src/func_8002206C.c` | Rename mode locals, retain historical comment/attempt | **`[0,0,0,0]`, EXACT** |

Three details matter when interpreting this sequence:

- **There were unsuccessful experiments.** The continuation was not an
  immediate oracle-like guess. It initially followed the same tempting web
  and scheduling levers as the earlier work, then changed representation.
- **Safety progress need not change the output.** Row 1 emitted the same words
  as the handoff, but removed the signed-left-shift overflow concern in that
  experimental narrowing. Equal output did not make the safety check useless.
- **A smaller schedule term was not the winning construction.** Row 4 was
  ranked better than the live baseline because schedule fell to one, although
  allocation rose to nineteen. It was a legitimate branch to preserve, not
  proof that the source interpretation was closer to the original.

The first exact candidate still contained the unsigned narrowing. Its removal
was then measured separately. This is a useful ablation: the new representation
had made a prior compensation unnecessary. It does not prove that narrowing
can never affect scheduling in another source; the earlier measurements show
that it can.

## 6. The important historical counterexample: the winning expression was already tried

The ledger row dated 2026-10-22 for
`build/func_8002206C/native-color-indexing.c` records `[0,10,24,45]`. That source
already contains exactly:

```c
*pColor1 = D_800558A4[v * 2];
*pColor2 = D_800558A4[(v * 2) | 1];
```

But the surrounding source is materially different:

- explicit `pVal`, `pColor1`, `pColor2`, `pPos1`, and `pPos2` walking pointers;
- a `loop_card` label and goto-based retry/slot flow;
- no integrated `CardGameState *state` root;
- the older pointer-walking maximum scan and reset placement;
- an explicit pre-call square computation in the weight loop.

The later handoff also records intervening declaration corrections. The
complete historical header snapshot is not reconstructed here, so this study
does not attribute the old result to one particular header change. The source
differences alone are sufficient to establish that these were not the same
complete-source experiment.

The correct retained conclusion from the earlier row was:

> This complete source, in that measured context, was not exact.

It was not:

> Halfword-array indexing is a closed direction for this function.

Nor would recompiling the *entire old source* against current headers test the
right follow-up question. The useful operation is to replay the **local
transformation** on the current, structurally repaired incumbent.

This is the strongest additional finding of the retrospective. It makes the
case less about an agent overlooking obvious C and more about a harness
needing to distinguish **historical candidates**, **reusable transformations**,
and **the premises under which they were measured**.

## 7. What worked well in the harness

### Persistent complete sources and conditional evidence

The continuation could preserve the earlier gains rather than start from a
fresh decompiler transcription. The historical natural-index candidate was
recoverable because the ledger retained a source snapshot, not just a score.
The saved near-match SAT report included its source hash and exact request.

### A residual separate from the terminal oracle

Per-block residuals localized the unfinished work. The terminal exact oracle
and full finalization prevented a plausible near-match from being mistaken for
completion. Neither the retrospective nor the proposed changes should weaken
those gates.

### Real compiler evidence

The scheduler's ready lists explained the multi-set accumulator's behavior.
The preserved exact dumps now show the expression's history through CSE,
combine, scheduling, and allocation. These are stronger witnesses than a
speculative account based only on final instruction order.

### Non-destructive scratch experiments and final simplification

Candidates stayed under `build/` until exact. The verified source was integrated
and measured again. Unrelated concurrently modified files were not reset.
After the breakthrough, redundant narrowing was removed through a measured
ablation rather than retained merely because the first exact source had it.

## 8. Harness improvements suggested by this case

All changes in this section are **proposals**, not implemented or benchmarked
features. The goal is to connect existing evidence and transformation machinery,
not add an unrestricted search engine.

### 8.1 P0: closures must expire when their material premises change

A useful closure should identify:

```text
transformation family: paired halfword-table indexing
region / operation: the two colour-table reads
measured parent source and full compile context
source assumptions: manual pointer/goto deal loop, old scan/weight structure
result: non-exact complete source, with its actual residual
scope: no claim about the same transformation on a repaired incumbent
```

After a declaration, loop-structure, storage-origin, or operation-boundary
correction, the harness should flag relevant historical transformations as
**eligible for replay**, not silently treat them as permanently rejected.

This must be bounded. Do not rerun every old candidate after every rename.
Associate a transformation with the source regions, declarations, and compiler
state it depends on. Replay a small compatible shortlist when those inputs
materially change. Where dependencies cannot be proved, say the scope is
unknown rather than inventing a broad closure.

**Acceptance test:** retain the old `native-color-indexing.c` measurement as
non-exact, then apply only its paired indexing transformation to the corrected
near-match. The system must distinguish that new question from recompiling the
old function or repeating a previously identical candidate. It must not claim
this construction was exact in the old source.

### 8.2 P0: separate observations, useful interventions, and necessity claims

A handoff should distinguish three evidence strengths:

| Claim | What supports it |
| --- | --- |
| “This candidate's address pseudo is multi-set.” | Candidate dump |
| “Sharing this scratch with mode Y improved the residual.” | Paired source measurements |
| “Every matching source must share the scratch.” | Not established by either of the above |

Language such as “genuine,” “must,” and “preserve” should carry a scope. An
intervention can be worth retaining as an incumbent without being a recovered
fact about the original program.

**Acceptance test:** summarize the pre-solution handoff with the cross-block
reuse marked as a measured, representation-scoped intervention. When natural
indexing removes it and reaches exactness, retire that intervention's necessity
claim without invalidating its historical measurement.

### 8.3 P1: add paired, type-aware representation probes before expensive state solving

For related accesses over a witnessed typed table, the cheap domain should
include complete operation families:

- byte-address arithmetic;
- typed element indexing;
- base-plus-element-offset forms;
- a witnessed record/field view, when layout evidence supports one.

Here the meaningful unit was two accesses with a shared card index. The second
index's OR-one is performed in **halfword units**, not byte units. A probe that
normalizes it immediately to shift-two / OR-two destroys the compiler-history
distinction the test is supposed to measure.

Preserve width, signedness, overflow conditions, access order, side effects,
and object bounds. Require evidence for the element type and layout; do not
invent a struct or change a global declaration merely to search for better
codegen. A volatile or side-effecting index must not be duplicated silently.

**Acceptance test:** from the current byte-oriented pair and the witnessed
`u16` table, generate the successful indexed pair as a complete-source variant.
Verify the source transformation's preconditions, then let the production
compiler and byte oracle decide the result. Include negative cases with
unknown element size, side-effecting indices, and unsupported aliasing.

### 8.4 P1: show when a repeated final operation was born

The exact source's duplicate shift is a compact example of an important
question:

> Were these operations already equal when CSE ran, or did a later pass make
> them equal?

An expression-provenance view should show the earliest witnessed form of each
repeated operation and its later rewrite boundaries. For this case it could
print:

```text
first shift-two: present before first CSE
second shift-two: shift-one / OR-one / shift-one through cse2
combine boundary: second chain becomes shift-two / OR-two
sched: first index/address are promoted single-set pseudos
lreg: index/address a1; loaded value a2
```

This should use raw compiler evidence and verified dump chronology. It should
not infer intermediate rewrites that no dump records, or pretend that the
original binary uniquely determines its lost RTL.

**Acceptance test:** a fixture from this exact dump set distinguishes the two
shift origins. A negative fixture with both shifts already equal before CSE
must not receive the same explanation. See also the pass-history concerns in
[the 80110944 case study](ovl_11_func_80110944-dead-bindings-and-compiler-history.md).

### 8.5 P1: report residual location separately from causal certainty

An allocation-shaped mismatch should not automatically become “the source
needs a local-allocation priority intervention.” The current candidate may
have the right final population but the wrong pseudo topology or earlier
expression history.

A useful report would separate:

```text
observed mismatch: first loaded colour uses a1 instead of a2
candidate cause witnessed: multi-set address web and its scheduled lifetime
target requirement: the emitted load must use a2
unique source cause: unknown
reverse replay: failed / verified / not run
```

Failed replay must qualify derived conclusions at their use sites, including
machine-readable results. A scoped SAT solution should name its fixed source
webs and what it does not search.

**Acceptance test:** the near-match still receives useful allocation and
schedule diagnostics, but no assertion that a phantom reference or shared
scratch is required by all clean C. The natural-index winner must remain an
admissible alternative outside that fixed-web model.

### 8.6 P1: carry the actual research incumbent and parent transformation

During the continuation, live source remained at `[0,0,4,1]` while the research
incumbent was `[0,0,3,1]`. The residual tool repeatedly compared new scratch
sources to the live source. Consequently an output identical to the research
incumbent could still be labelled “better” than baseline.

That is not a false measurement, but it is easy to misread as new progress.
Track three separate identities:

1. live integration source;
2. active research incumbent;
3. the candidate's actual parent and transformation.

Rank against the intended incumbent while retaining the live-source comparison
for integration purposes. A lower scheduling term with a larger allocation
term can be a useful branch, but should not automatically establish a better
source interpretation.

**Acceptance test:** `colour-from-stored-card.c` is reported as the same final
output as the research incumbent, even though it improves on the unchanged
live source. `fresh-sum-global-load.c` remains an inspectable branch with its
schedule/allocation trade clearly stated.

### 8.7 P1: output equivalence is not source-safety or pass-history equivalence

The tool's “same experiment, different spelling” message is based on final
output equality. `tools/agent/residualObjective.ts` emits that wording;
`tools/agent/experimentLedger.ts` groups respellings by output fingerprint.

That relation is useful for avoiding repeated byte comparisons. It is too
strong as a universal statement about what was learned. The signed-to-unsigned
narrowing experiment preserved output while removing a language-level concern.
More generally, two sources with the same final code may differ at the pass
relevant to a later transformation.

Prefer messages such as:

```text
SAME FINAL OUTPUT.
Source-safety change: unsigned shift replaces potentially overflowing signed shift.
Relevant intermediate-state equivalence: not measured.
```

Do not count every safe respelling as codegen progress. Preserve the distinction
between output novelty, admissibility/safety, and mechanism novelty.

**Acceptance test:** the unsigned narrowing is recognized as a safety check,
not a new instruction-stream result. Repeating an unchanged source/context
remains a cheap no-op. This complements the clean-source versus pinned-source
examples in the 80110944 study rather than replacing output deduplication.

### 8.8 P1: make post-breakthrough ablation a small explicit finishing step

After a representation change, inspect compensations introduced only to make
the previous representation work: scratch sharing, narrowing, temporary
lifetime extensions, or unusual statement placement. Remove one at a time
and measure.

This is not permission for an open-ended refinement campaign. It is a bounded
check of documented workarounds whose premise has just disappeared. Here the
mode narrowing was the obvious candidate; its removal stayed exact.

**Acceptance test:** the first exact candidate is preserved, its no-narrowing
ablation is measured independently, and only an exact clean result is
integrated. Full finalization remains mandatory afterward.

## 9. A bounded future workflow and evaluation plan

A better route for this family of residuals would be:

1. Load the current admissible candidate and the earlier work's scoped facts.
2. Audit declarations and operation boundaries before treating the residual as
   a pure allocator problem; reuse fresh audits rather than repeating them.
3. Identify the typed operation represented by the mismatching address webs.
4. Retrieve related *transformations* from historical experiments, including
   non-exact candidates whose surrounding source has since changed.
5. Replay a small type-aware operation-family domain on the current incumbent.
6. Trace the first materially different candidate across the relevant earlier
   boundaries before escalating to fixed-web scheduling/allocation solvers.
7. Preserve useful branches with their named mechanism, not just a scalar key.
8. On exactness, ablate obsolete compensations, integrate, and run the full gate.
9. Update the research record independently of any original-TU grouping claim.

This is a proposed workflow, not a measured promise of a particular number of
compiles or a speedup. The current session supplies one successful case, not
a benchmark of general superiority.

### Suggested regression fixtures

Keep text fixtures and provenance, not generated binaries, for:

- the earlier natural-index source with the manual-pointer/goto structure;
- the inherited signed-narrowing near-match;
- its unsigned counterpart;
- the fresh-sum/global-load branch;
- the exact natural pair with narrowing;
- the exact no-narrowing ablation;
- the verified integrated source;
- narrowly extracted CSE/combine/scheduler/allocation witnesses.

Production-compiler integration tests should regenerate artifacts under
`build/`, validate context identity, and use the relocated-byte oracle.
The old complete source should remain a non-exact historical measurement;
only replaying the local transformation on the repaired parent is expected
to demonstrate the successful route.

| Test | Required behavior |
| --- | --- |
| Old natural pair on old structural source | Preserve non-exact result; do not turn it into an unconditional family rejection |
| Same pair replayed on repaired near-match | New complete-source experiment; exactness decided by oracle |
| Shift forms before and after combine | Distinguish late-born duplication from two already-equal CSE inputs |
| Multi-set accumulator versus fresh address webs | Report the candidate's actual SET counts and scheduler priorities |
| SAT phantom witness | Retain source/web scope; never promote as C or as a necessity theorem |
| Unsigned safety rewrite with equal bytes | Report equal output and separate safety significance |
| Research incumbent differs from live source | Compare against both with explicit roles; avoid false progress labels |
| Narrowing removal from exact pair | Independently confirm exactness; preserve first winner as fallback |
| Documentation-only review | Leave verified source/header/configuration fingerprints unchanged |

## 10. Conclusions and limits

The final correction was small because the earlier work had made the rest of
the function correct. It was effective because it changed the compiler's input
representation, not because it found a more ingenious way to force a register.

The deeper lesson is stronger than “write natural C”:

- The natural expression had already been tried in a different complete source.
- Its earlier failure did not survive the later structural and declaration
  corrections as an unconditional conclusion.
- The target's duplicate instructions were produced by different optimization
  histories, not necessarily by explicit source-level duplication or destructive
  scratch updates.
- Useful near-match interventions can become obsolete when their underlying
  representation is replaced.
- A solver's accurate answer about one pseudo topology does not establish that
  the topology is the original program's.

Nothing here proves the exact spelling of the original author's C, or that
array indexing is universally superior to pointer walking. The accepted
construction is ordinary, evidence-supported C that the production toolchain
compiled to the original bytes and that passed the full finalization gate.

For the harness, the most valuable next step is not a larger search over the
same fixed accumulator. It is **premise-aware replay of small, typed source
transformations, coupled to observable compiler history and an uncompromised
terminal oracle**.
