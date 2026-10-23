# ovl_11_func_80104EC8: CSE scope, local return joins, and trustworthy experiments

## Outcome and scope

`ovl_11_func_80104EC8` is now a clean-C, byte-identical reconstruction of a
1982-word function with a large switch over selectors `0..0x75`. The final
signature is `s32 ovl_11_func_80104EC8(s16 arg0)`.

The completed matching session passed the exact function diff, full configured
build, modification-scope check, and source-policy gate. The saved
`make -j8 check-all` log records matches for the executable and all 13 configured
overlays. The final relocated-word result is **1982/1982**, staged residual
`[0, 0, 0, 0]`, with **zero undetermined relocations**.

The solution uses ordinary C and one evidence-gated per-file override,
`-fno-cse-skip-blocks`. It does not use embedded assembly, register pinning, a
compiler patch, or disabled scheduling/GCSE. The override and its audit entry
were part of the verified integration, not introduced by this report.

This case study records what made the match possible and where the harness
made the evidence difficult to use. **The improvements below are proposals,
not implemented features.** This documentation task changes only this note;
it does not repeat the solver work or claim a new full-build verification.
The shorter [function research note](../ovl_11_func_80104EC8-cse-and-return-sharing.md)
records the accepted source mechanisms and integration result.

### Evidence identities

| Item | Identity |
| --- | --- |
| Verified source | `src/overlays/ovl_11/ovl_11_func_80104EC8.c` |
| Source SHA-256 | `197ba92ba863aaa3eaa48ff4c3cabb6cb7e9aa0c4d0a0a720e83d53c14f0d4fc` |
| Finalization handoff identity | `9a33c9b7c3220e3c32ba179c5b05ed031c99d720f3010bb09713140fb75ddcb8` |
| Final ledger output fingerprint | `15a11ac4741d6a9738bed3cc79cca00bf4a3eff1e6ac3dad8087769247b5bdc6` |
| Target hash in final flag probe | `f3a5e20d17d00d627b6351056b95836d243d1daff37e2bbb543967a1f777bb53` |
| Toolchain hash in that probe | `04227eef0ca052c77161d4b04c2cd8574a1827f6f601aa5657ea4d1df98aa3e6` |

These are different identity domains. In particular, the handoff identity is
not a hash of the C source, and the ledger output fingerprint is not the
linked overlay's checksum. General toolchain configuration remains documented
in `configs/project-profile.md`.

Throughout this report:

- **Measured** means a saved complete-source experiment or verification result.
- **Implementation evidence** means an inspected tool/compiler decision.
- **Interpretation** connects those observations without asserting unique
  historical source recovery.
- **Proposal** names a harness change and a test that would justify accepting it.

Scratch artifact IDs below are relative to
`build/ovl_11_func_80104EC8/inspection/`. An experiment ID such as
`inline-true-4` names its C snapshot and the corresponding directory under
`flag-experiments/`. These generated files are local evidence, not artifacts to
commit; build cleanup can remove them.

## 1. The problem was several independent problems, not one difficult allocator

The starting representation carried decompiler-introduced function-wide
variables and explicit sharing between switch arms. That obscured several
independent choices:

1. Which control-flow edges CSE could traverse while retaining an address.
2. Which logically unrelated queries belonged to different register webs.
3. Which output buffers were one source object rather than separately aligned
   objects.
4. Whether an address initialization was source work or loop-strength-reduction
   work.
5. Whether a shared return was authored in C or discovered later by GCC.
6. Whether the comparison had enough rodata context to resolve table addresses.

A single byte score could not distinguish these. Even a staged residual was
hard to interpret when the target and candidate CFGs no longer had comparable
block boundaries. The successful approach kept complete candidates, separated
flag experiments from production measurements, and localized differences to
switch cases before selecting a compiler mechanism.

The main lesson is not to enumerate more spellings. It is to establish **which
program, under which flags, with which correspondence and relocation context,
a diagnostic is describing**.

## 2. Recovering CSE scope without disabling unrelated optimization

### Target evidence and a discriminating matrix

The target repeatedly materializes the complete `D_8006C838` address after
guard diamonds. For example, the selector at `0x80105260` is followed by a fresh
`lui` at `0x80105288`; case 0x15 provides another pair at `0x80105674` and
`0x80105694`. Baseline compilation carried the address through these guards.

The governing compiler code is `cse_end_of_basic_block` in
`tools/vendor/gcc/2.95.2/src/gcc/cse.c`. Its skip-block and jump-following paths
are separate controls. That distinction supplied an experiment, not merely a
hope that less optimization would emit more instructions.

The final complete-source matrix, measured before installing the override:

| Additional flags | Emitted words | Result relevant to the hypothesis |
| --- | ---: | --- |
| None | 1793 | Does not retain the target's repeated materializations and guarded regions |
| `-fno-cse-follow-jumps` | 1794 | Does not repair the skip-block difference |
| `-fno-cse-skip-blocks` | 1982 | Relocated-byte EXACT after table attribution |
| Both flags | 1988 | Destroys sharing the target retains within case 0x15 |

The two-flag row is an important contrary witness. An earlier parked attempt
had used both flags; carrying that setting forward would have preserved a
wrong premise. Only skip-blocks was disabled in the accepted configuration.

Source-level control-flow changes were also informative. A guarded-goto form
could stop reuse of the full address, yet GCSE still shared its high half,
leaving a move where the target had a fresh `lui`. Thus “the base is reloaded”
was too coarse a requirement: the experiment had moved one pass's decision
without recovering the next pass's input.

Nearby matched `ovl_11_func_80103B24` supplies same-overlay precedent for the
accepted flag. It does **not** establish that these functions shared an
original translation unit. No new TU boundary is inferred here.

### Why the automated conclusion did not select the winning flag

The saved final flag probe reports `not-supported-current-source`, even though
its matrix includes the winning no-skip row. This is not evidence that the
matrix was absent or that exactness overrode a valid contrary test.

Implementation inspection explains the mismatch in scope:

- `tools/agent/flagProbe.ts:concludeMatrix` derives candidate verdicts only for
  flags nominated by target fingerprints.
- Its interval-based nested-loop fingerprints nominated no-GCSE combinations
  here. Those combinations did not beat baseline.
- The no-skip row appears in `dominantRows`, but was not one of those nominated
  candidates. The aggregate conclusion describes the nominated set, not every
  measured flag.
- The no-skip row reports 1772 masked matches, not the relocated oracle's 1982.
  Masked assembly comparison and resolved-byte comparison are different tests.

Backward branches into shared return blocks made the target look like nested
loops to the interval heuristic. The probe generated 27 `pre-fatal-shape`
fingerprints from this interpretation. A compiler CFG loop requires more than
an address interval containing another backward branch.

There was a separate source-selection trap earlier: the live file was an
`INCLUDE_ASM` stub with a disabled C attempt. Measuring that stub makes flag
columns tie on the original assembly. The CLI's `--with-source` rows are
advisory additions; `probe` still asks `concludeMatrix` about `current src`.
A tie on the stub cannot close a flag hypothesis about a different C candidate.

**Interpretation:** the probe supplied useful measurements, but its headline
could be read as a stronger rejection than its inputs justified. The remedy
is explicit scope and better loop evidence, not automatically accepting any
flag with a higher score.

## 3. Source repairs with distinct compiler effects

The following are representative measured changes, not a claim that every
intermediate spelling is necessary or that this is the original author's C.

| Experiment family | Source change | Mechanism recovered |
| --- | --- | --- |
| `direct-calls` | Undo explicit shared call-argument control flow between cases 7 and 0x4C | Let GCC construct call sharing instead of supplying a decompiled join |
| `local-query-webs` | Give each affection-query arm its own amount **and** threshold variables | Recover independent webs; splitting only the amount changed conflicts without fixing the family |
| `coordinate-blocks` | Use two four-halfword output buffers and one two-halfword buffer | Preserve actual object grouping and stack offsets, not artificial padding |
| `accumulator-1f`, `accumulator-25`, `accumulator-53` | Update the real accumulator instead of routing updates through fresh temporaries | Recover multiple-SET scheduling behavior and the placement of signed truncation |
| `indexed-25` | Express the five-element table scan by index | Let the loop optimizer emit the induction initialization after movables |
| `cursor-23`, `same-index-23`, `return-false-23` | Use a byte cursor for the first animal scan, reuse the index for the second, and return zero locally on failure | Recover the address base, counter allocation, and missing zero-valued delay-slot operation |
| `nested-date-tail` | Duplicate natural case logic and give the inner test the proper false join | Let GCC rediscover the shared tail without folding the guarded Boolean result |
| `zero-result-53` | Initialize the zero result outside the inner call guard | Recover the intended result web and branch orientation |
| `return-16`, `return-69` | Return literal one after the calls instead of initializing it before them | Avoid an unnecessary constant live across a call |
| `nested-1c`, `nested-51`, `nested-46`, `unshare-73-tail` | Change local decision structure rather than force registers | Recover branch/result form and compiler-owned tail sharing |
| `else-result-15` | Place the zero result inside the year-test's `else` arm | Recover the target's flags register; moving the zero outside the test did not do so |

### Storage grouping was semantic context, not frame-size tuning

Five independent `s16[2]` arrays each incurred eight-byte allocation rounding
in the measured representation. Replacing them with the observed two
`s16[4]` buffers and one `s16[2]` buffer removed sixteen excess stack bytes.
These were real coordinate outputs accompanying three records. The accepted
layout was not a dummy array added to influence saved registers.

This matters for a harness: frame evidence should describe **objects and their
uses**, not offer “add or remove N bytes” as an unconstrained matching knob.

### The two loop repairs were different experiments

Case 0x25 needed indexed table access so its initialization came from the loop
optimizer's induction handling. Case 0x23 instead needed a byte cursor stepping by the
animal-record size. The latter retained a base corresponding to state-root
plus `0x8000`, with large field displacements; a field-address GIV selected a
base corresponding to root plus `0xDEB8`.

These are not competing universal rules about pointer versus indexed loops.
They are separate requirements established by the target's address formation
and preheader order. A loop-shape catalogue should preserve both constructions
and explain which evidence distinguishes them.

## 4. The decisive repair: local joins, not a function-wide success label

### Why manually reproducing sharing was counterproductive

The earlier source routed dozens of successes to one label inside case 0xA.
The target's common true-return anchor is at `0x80105280`. In the candidate,
that block survived through scheduling but late cross-jumping could fold it
into a true-return block in another case, including 0x15, 0x2A, 0x53, or 0x71.
The resulting block movement contaminated comparisons well beyond that return.

Implementation evidence is unusually concrete:
`tools/vendor/gcc/2.95.2/src/gcc/jump.c:find_cross_jump` compares equivalent
instruction suffixes and reduces its minimum when it reaches a `CODE_LABEL`.
An initially two-instruction minimum can therefore permit sharing a single
constant-one SET in this situation. A label is not just a readability choice
for this compiler; it changes the late sharing opportunity.

Replacing global success/failure gotos with natural returns retained case
0xA's block, but was not itself the complete solution. Case 4 then collapsed
from the target's 34 words to 27. Restoring its jumps to the global success
label fixed case 4 but lost the case-0xA anchor again.

The discriminating experiment was to retain a **local** case-4 join:

1. A local true-return label at the end of case 4 retained case 0xA and the
   34-word case-4 population, but left case-4 instruction differences.
2. Moving that label onto the first successful `count < 5` return, with the
   other successful arm jumping to it, made case 4 match as well.

This is evidence for label scope and placement as compiler inputs, **not** a
rule to ban `goto`. Both “share every success” and “replace every goto with a
return” failed in distinct, useful ways.

### Measured final sequence

Every row below used the same additional flag, `-fno-cse-skip-blocks`.
The key is `[controlFlow, population, schedule, allocation]`, as defined in
`tools/agent/pipeline-reversal/objective.ts`; it is not a count of wrong words.
The scratch helper measured explicit objects with replay disabled, so these
keys are localization evidence, not a round-trip-certified proof of pass
ownership. The final byte oracle is the acceptance authority.

| Complete candidate ID | Staged key | What the comparison established |
| --- | --- | --- |
| `natural-both-returns` | `[3, 2840, 2, 156]` | Case 0xA retained; case 4 folded too far and other local tails remained open |
| `share-4-only` | `[2, 2535, 0, 140]` | Global sharing repaired case 4 but removed the desired case-0xA anchor again |
| `local-true-4` | `[0, 28, 0, 6]` | Local sharing retained both regions; case 4 still differed |
| `inline-true-4` | `[0, 22, 0, 6]` | Moving the local join into the first successful arm closed case 4 |
| `invert-21-tail` | `[0, 16, 0, 6]` | Inverting the last call test and exchanging return arms closed case 0x21 |
| `invert-3e-tail` | `[0, 10, 0, 5]` | Positive OR/success form instead of negative AND/failure form closed case 0x3E |
| `false-label-71-2a` | `[0, 5, 0, 3]` | Local false join preserved one guarded date tail |
| `false-label-71-2c` | `[0, 1, 0, 2]` | Applying the corresponding false join to the other tail closed case 0x71 |
| `local-true-72` | `[0, 0, 0, 0]` | Local true join closed the last instruction residual; six words still undetermined |
| `final-context` | `[0, 0, 0, 0]` | Clean declarations and derived table context; zero undetermined words, EXACT |

The large early population terms do not mean thousands of semantic operations
were missing. Shared-tail placement and block correspondence were part of the
problem. Once control flow aligned, the remaining differences became local.

The final source move was particularly small. Within case 0x72, the first
successful date test acquired a label and the second success jumped to it:

```c
if (((Ovl11ConditionState *) D_8006C838)->date[1] != 0) {
date_ready:
    return 1;
}
if (((Ovl11ConditionState *) D_8006C838)->date[2] != 7) {
    goto date_ready;
}
```

Before that edit the second arm also said `return 1`. The local join recovered
the returned-constant/comparison web separation, registers, and delay slot.
It did not add a new runtime operation. The zero-key candidate's source hash
was `2d2478239825dfc307bf7b7f955b9ee1b0644b2ae93de2de5828ae1cfd49ee5a`;
it was not yet an exact candidate because relocation context was incomplete.

## 5. Diagnostic lessons: preserve distinctions that the headline can hide

### Switch-region alignment helped, but was deliberately not a proof

A scratch comparator used the main jump table's selector-to-address relation
to align physical case regions, then compared instruction sequences. It
retained registers but normalized local branch destinations. It also excluded
the shared true-return entry associated with selector 0x26 from region
boundaries; otherwise a moving shared return split unrelated cases into false
population differences.

This recovered useful questions such as “is case 4 fixed while case 0xA moves?”
when basic-block numbering was unstable. It did **not** prove equal branch
behavior: normalizing every local destination can hide a wrong branch target.
Zero region differences were never substituted for the relocated-word oracle
or the final linked-overlay check.

A production version needs dispatch-key/CFG correspondence with explicit
shared-tail ownership. The scratch interval comparator is evidence for that
capability, not an implementation to promote unchanged.

### Measurements must belong to the object and flags actually tested

Ordinary `residualObjective` calls compiled saved candidates under the then
configured production flags. During the counterfactual work those were still
baseline flags. Meanwhile the separate flag helper was compiling no-skip
objects. Baseline incumbent/stagnation messages therefore did not describe
progress in the winning flag experiment.

There was also an operator error in the first scratch helper: it supplied
`object` where `reversePipeline` expects `objectPath`. The intended explicit
object was not selected, and the comparison used a baseline compilation.
The helper was corrected and affected direct-call rows rerun. This was not a
failure of the residual arithmetic; it was failure to bind the measurement
to the intended experiment at the API boundary.

The final helper records the actual `cc1Flags`, preserves the complete source
and object, and passes `objectPath` explicitly. Its example should motivate a
typed, checked receipt rather than more ad hoc wrappers around the compiler.

### A displacement inventory is not an effective-address inventory

Earlier raw inventories reported target-only offsets such as `0x5E30` and
`0x5EB8` where candidate forms used a different base and displacements such as
`-0x88` or zero. Those require address-expression analysis before concluding
that an access is missing. The exact integrated source still produces two
count-only inventory differences associated with address/table forms.

Normalization must retain load width, signedness, memory effects, and unknown
bases. The lesson is not to silence inventory findings: it is to distinguish
“different encoding of the same access” from “missing access” and “unresolved.”

### Loop diagnoses belonged to different kinds of evidence

Two superficially similar warnings must not be conflated:

- The **target-side interval heuristic** treated branches to shared returns as
  evidence of nested loops. Even integrated triage retained this signal.
- The **candidate-side compiler dump**, under the old baseline flag setting,
  reported three phony loops. That was an observation about that compilation,
  not a fact about the no-skip object being compared separately.

Integrated production triage had **no blockers**. It would be inaccurate to
say that a blocker was simply ignored because a candidate looked exact.
The lesson is to bind diagnostics to source/flags/object identities and to
prefer CFG/dominance evidence over address intervals for target loop claims.

Large diagnostic outputs also worked against inspection: nested-loop listings
could exceed twelve thousand lines, while bounded tool responses hid parts of
the report. A short decision summary plus a complete artifact is more useful
than printing the full inferred nesting tree and truncating its explanation.

## 6. Exact instructions were not the end of integration

### Six unknown words were unresolved context, not six wrong instructions

The three jump tables occupy the original rodata window `0x26F0..0x2930`:
576 bytes, comprising 118, 8, and 18 entries. Their target addresses are
`0x800BA510`, `0x800BA6E8`, and `0x800BA708`.

While the live function was an assembly stub, the tables remained generic
rodata. Six high/low address-reference words could not be resolved by the
candidate oracle. A zero staged key therefore correctly remained non-exact.

The integration compiled the real C, derived table ownership with
`tools/build/deriveRodataSplits.ts --container ovl_11 --write`, and replayed the
flagged object against the updated context. That resolved the six words
without inventing symbol values or treating unknowns as equal. The final
linked check also verified the actual table contents.

A subsequent build exposed stale generated linker state: deriving the config
alone had not refreshed the split/linker artifacts, and the old rodata object
referenced internal labels no longer supplied by the C object. Running
`make split-ovl_11` regenerated the linker inputs. That split also rewrote
`include/gte_macros.inc` without its existing `rtir` alias, breaking an unrelated
assembly function. Only that generated change was restored to its known-good
tracked state before the successful full verification.

These were integration dependency/ownership problems, not reasons to hand-edit
jump-table addresses or weaken build checks.

### Equal bytes did not validate missing prototypes

A cleanup candidate removed the forward declarations and still emitted the
same instructions. `calleeTruth` then exposed eleven implicit declarations:
`common.h` does not supply these function prototypes merely because generated
context headers exist elsewhere.

The accepted cleanup restored the eleven actually used declarations,
corroborated against matched callee definitions, and removed unused context.
The byte-selector caller `ovl_11_func_800F1AA8` had its local prototype
reconciled to the `s16` parameter and remained 14/14 words exact; the generated
overlay context was published through the context exporter.

This is an especially useful negative control: **output equivalence is not
context correctness**. Signature checks and source-policy checks must survive
a successful byte comparison.

## 7. Proposed harness improvements, in priority order

These changes should extend existing tools rather than create a parallel
matching framework. Each proposal includes a bounded acceptance test.

### P0 — Make the measured experiment an explicit, validated object

**Observed:** the wrong object option silently selected a different compile;
production and counterfactual measurements described different flag columns;
a stub could be the primary source in a flag conclusion.

**Proposal:** have compilation return a typed receipt containing source and
preprocessed-source identities, target/container identity, toolchain and flag
fingerprints, object path/hash, and relocation-context identity. Comparison
should consume that receipt or an explicitly validated external-object mode.
Reject unknown option keys and report any fallback compilation prominently.
Give flag probing an explicit primary candidate; classify an assembly stub as
unavailable for C flag inference. Partition incumbents and closure claims by
compatible experiment context, retaining source hashes within each lineage.

**Acceptance tests:**

- Supplying the misspelled object option fails instead of silently compiling.
- Comparing one source under baseline and no-skip yields separate, named
  measurement lineages; a baseline stall cannot close the no-skip experiment.
- A live stub plus an explicitly selected C candidate produces conclusions
  about the C candidate, never an “all flags tie” inference from the stub.
- A changed object or context invalidates a cached comparison receipt.

### P1 — Make flag conclusions column-specific and evidence-specific

**Observed:** the aggregate verdict rejected nominated no-GCSE candidates
while the full matrix contained an un-nominated, ultimately exact no-skip row.

**Proposal:** separate target fingerprints, per-column measurements, nominated
candidate verdicts, and unresolved conflicts in the report schema and headline.
Expose “measured dominant but not fingerprint-nominated” as a request for
mechanism evidence, not automatic flag approval. Include relocated-oracle
status separately from masked similarity and retain contrary witnesses.

**Acceptance test:** replay the saved final matrix. The report must reject the
measured no-GCSE hypothesis, surface the no-skip result for investigation, and
retain the two-flag case-0x15 regression. It must neither reject all overrides
nor authorize the winner from instruction count alone.

### P1 — Share CFG evidence between loop detection and switch correspondence

**Observed:** shared returns became false nested loops and unstable region
boundaries. Case-aware localization helped where ordinal block pairing did not.

**Proposal:** use the existing original-word CFG/SSA machinery for dominance,
natural-loop/SCC classification, dispatch edges, and shared tails. Align switch
regions by selector and effects, allowing many selectors to share one entry
and one case to reach a common tail. Report ambiguous correspondence rather
than manufacture a population defect. Keep branch destinations in the proof.

**Acceptance tests:** branches backward to a shared return do not become natural
loops; genuine counted loops remain detected. Moving the common true return
between cases changes tail placement, not the inferred semantics of every
intervening case. Deliberately swapping a branch destination must still fail.

### P1 — Add label placement to bounded control-shape experiments

**Observed:** global sharing, duplicated returns, a trailing local join, and an
inline local join produced different outcomes. Return orientation fixed cases
0x21 and 0x3E; local false joins prevented unwanted Boolean folds in case 0x71.

**Proposal:** extend control-shape analysis with semantically checked local
true/false joins and their placement, bounded to one decision region. Preserve
side-effect order and reject jumps across required initialization. Attribute
the first meaningful divergence through jump, scheduling, and late cross-jump
dumps. Explain a downstream register change as a consequence when control
shape changed first; do not default to an allocation search.

**Acceptance tests:** retain the case-4 four-way comparison and the last
case-0x72 pair as fixtures. The tool should expose why the local inline join is
a distinct experiment, retain the case-0xA anchor as a requirement, and recover
the measured final move without trying arbitrary function-wide goto rewrites.

### P1 — Make table/context integration one checked transaction

**Observed:** zero instruction residual still left six unresolved references;
config derivation without a refreshed split left stale linker inputs; splitting
rewrote an unrelated generated include.

**Proposal:** coordinate source staging, object compilation, rodata derivation,
split/linker refresh, context publication, scope checks, and final verification.
Track generated-output ownership and pre-existing edits. Report the exact next
missing dependency when code is ready but relocation context is not. Never
restore a file merely because it is dirty; restore only a transaction-owned
change whose prior contents were captured.

**Acceptance tests:** a candidate with local tables transitions from
zero-key/unknown-context to exact without manual address insertion. Stale
linker state is detected after config derivation. An unrelated generated-include
change is isolated without overwriting another worker's edits. Full binary and
policy checks remain mandatory.

### P2 — Normalize memory effects and preserve object evidence

**Observed:** displacement multisets confused alternative address bases with
missing accesses; independent small arrays introduced different alignment.

**Proposal:** augment, rather than discard, the raw inventory with affine base
plus offset analysis and typed access widths. Carry observed stack-object use
ranges into preparation so output-pair grouping is an evidence-backed choice.
Keep uncertain pointer origins explicit. Use query-family web partitions as
bounded source hypotheses, not hard-register-name assignments.

**Acceptance tests:** equivalent root/base/displacement forms compare as the
same access; a changed field or access width remains a defect. The two-four-
halfword/one-two-halfword layout is distinguishable from five independent
arrays by real output uses, not just an attractive frame size. Splitting only
amounts and splitting both amounts/thresholds remain separate experiments.

### P2 — Keep context checks independent of output equality

**Observed:** removing declarations preserved output but introduced eleven
implicit calls. The existing callee audit correctly rejected that cleanup.

**Proposal:** retain callee-truth auditing in finalization and carry declaration
provenance into cleanup tooling. Do not infer availability from a generated
header that the translation unit does not actually include.

**Acceptance test:** the declaration-free output-equivalent candidate fails the
context gate; the explicitly corroborated declarations pass. A stale caller
prototype is found even if that caller's current instructions happen to match.

### P2 — Make bounded output useful before truncation

**Observed:** large loop reports and matrix detail could obscure the active
question and the scope of a conclusion.

**Proposal:** lead with experiment identity, verdict scope, blockers/unknowns,
and the next bounded action. Deduplicate repeated fingerprints, summarize
counts, then link the complete structured artifact. Preserve an explicit
truncation notice and never lose a blocker or uncertainty in the omitted tail.

**Acceptance test:** feed the large switch and repeated interval fingerprints
through the normal response limit. The primary candidate, flags, unknown-word
count, and disagreement between nominated verdicts and other matrix columns
must remain visible without reading thousands of lines.

## 8. Reusable workflow and limits of the evidence

For another large dispatch function, this case supports the following order:

1. Confirm signatures, object layouts, and real output storage before studying
   allocation. A byte-equivalent prototype cleanup can still be wrong.
2. Identify the measured source explicitly, especially while live source is a
   stub. Test a flag only with target evidence and contrary witnesses.
3. Localize by dispatch keys and shared-tail structure when block correspondence
   is unstable. Do not treat normalized-region equality as exactness.
4. Separate independent query webs, accumulator reuse, loop construction, and
   local return structure into distinct complete-source experiments.
5. When equivalent spellings leave a stable residual, read the compiler
   decision. Here the CSE boundary and cross-jump label rule named useful moves.
6. Keep unknown relocations separate from instruction defects. Derive context,
   refresh generated dependencies, then run the original full verification gates.

The final result establishes a sufficient clean-C reconstruction under the
accepted configuration. It does not prove the original variable names,
particular label spelling, or a unique historical source form. The failed
baseline source forms do not prove that every imaginable baseline program is
impossible. The nearby override does not prove a shared original TU. The
measurements also do not support a reliable total time, token cost, or speedup
estimate for a proposed harness.

The transferable result is narrower and stronger: **local source structure,
compiler-pass boundaries, and measurement identity must stay visible together.**
Otherwise a valid observation about one compilation can close the wrong search,
and a decompiler's attempt to preserve machine-level sharing can prevent the
compiler from recreating that same sharing naturally.

## Appendix: artifacts and implementation anchors

### Local generated evidence

All paths in this table are relative to
`build/ovl_11_func_80104EC8/inspection/`; they are not promised to survive cleanup.

| Artifact | Purpose |
| --- | --- |
| `final-context.c` | Accepted complete source; hash equals the integrated C <!-- doc-ref-ignore: generated artifact, checked relative to inspection/ --> |
| `final-flag-probe.json` | Final source identity, full flag matrix, fingerprints, and scoped conclusions <!-- doc-ref-ignore: generated artifact, checked relative to inspection/ --> |
| `flag-experiment.ts` | Corrected explicit-object measurement helper <!-- doc-ref-ignore: generated artifact, checked relative to inspection/ --> |
| `flag-experiments/final-context--fno-cse-skip-blocks/measurement.json` | Flags, zero staged key, zero unknowns, exact result <!-- doc-ref-ignore: generated artifact, checked relative to inspection/ --> |
| `flag-experiments/<ID>--fno-cse-skip-blocks/` | Complete source, preprocessing, GCC dumps, assembly/object, and measurement for the experiment IDs above |
| `case-diff.ts` | Scratch selector/physical-region comparator; normalization limits described above <!-- doc-ref-ignore: generated artifact, checked relative to inspection/ --> |
| `final-context-triage.json` | Baseline-flags triage, including the three-phony-loop finding <!-- doc-ref-ignore: generated artifact, checked relative to inspection/ --> |
| `integrated-triage.json` | Production-flags triage; no blockers <!-- doc-ref-ignore: generated artifact, checked relative to inspection/ --> |
| `check-all.log` | Successful executable and 13-overlay verification |

The ledger is `build/experimentLedger/ovl_11_func_80104EC8.jsonl`, with
content-addressed sources under the adjacent `sources/` directory. It is useful
for source/output identity, but earlier rows must not be interpreted as if they
all belonged to the final flag configuration.

### Repository anchors for follow-up work

- `tools/vendor/gcc/2.95.2/src/gcc/cse.c`: `cse_end_of_basic_block`, skip-block and
  jump-following decisions.
- `tools/vendor/gcc/2.95.2/src/gcc/jump.c`: `find_cross_jump`, especially the
  `CODE_LABEL` reduction of the minimum shared suffix.
- `tools/agent/flagProbe.ts`: `probe`, `CURRENT_SOURCE_LABEL`, `concludeMatrix`.
- `tools/agent/decompToolchain.ts`: `compileSource` and `extraCc1Flags`.
- `tools/agent/pipeline-reversal/reverse.ts`: `ReverseOptions`, `objectPath`, and
  explicit-object provenance.
- `tools/agent/pipeline-reversal/objective.ts`: staged-key semantics.
- `tools/agent/residualObjective.ts` and `tools/agent/experimentLedger.ts`:
  measurement lineage and incumbent reporting.
- `tools/build/deriveRodataSplits.ts` and `Makefile`: attribution, split, and
  overlay verification dependencies.
- `configs/flag_overrides.mk` and `.pi/autoloop.json`: accepted override and
  source-policy audit, established before this documentation task.

The earlier
[dead-bindings case study](ovl_11_func_80110944-dead-bindings-and-compiler-history.md)
provides a complementary example: source operations that disappear can still
change later compiler behavior. Here, source labels and apparently equivalent
return forms changed which sharing survived. Both argue for preserving the
causal experiment, not only the winning output.

### Documentation validation

This note was checked with `tools/diagnostics/checkDocReferences.ts` scoped to
this file, including bare basenames. Generated artifact references excluded
from that repository-file index were checked directly. Local Markdown links,
code-fence balance, and whitespace were checked separately. The verified C
source still has the SHA-256 recorded above. No source, header, configuration,
policy, tooling, or generated binary was changed by this documentation task;
the full-build result cited here belongs to the completed matching session.
