# Case study: 80110944, dead bindings, and compiler history

**Function:** `ovl_11_func_80110944` (`ovl_11`).

**Study date:** 2026-10-23.

**Outcome:** byte-exact plain C, 233/233 target words; the authoritative finalizer passed exact diff, full build, scope, and clean-source gates. No assembly, register pinning, compiler patch, or per-function flag override was needed.

**Scope of this document:** an evidence-backed harness review and proposed improvements, not an implementation of those improvements. No new translation-unit grouping claim.

## Executive summary

The earlier investigation had largely found the right answer at the compiler-state level: the target needed a long-lived **high half of an address**, not a long-lived complete address. What it lacked was a source construction that created that state while leaving only one address construction in the final assembly.

The successful C contains two assignments whose assigned values are never used:

1. a second assignment of the large-array base to the existing `large` variable, after the outer loop;
2. an assignment of the state root to the existing `animal` variable, immediately before that variable is overwritten with the record address.

These assignments survive early optimization, influence address sharing and motion, and disappear by the flow dump. Their compiler effects do not disappear with them. In particular, GCSE shares the large-array HIGH, and reload later reconstructs that HIGH separately from its LOW.

This is not a lesson to insert arbitrary dead code into every near-match. It is a lesson to model **the history of eliminated source operations**, under explicit requirements and semantic constraints.

The harness helped enormously: it preserved previous research, measured complete sources, exposed GCC dumps, and required a real linked-byte and policy finish. However, several of its abstractions and messages discouraged precisely the successful route:

- **Equal final bytes were called the same experiment**, even when one source used assembly and another reached those bytes in plain C.
- **An allocation-shaped difference was described as a local-allocation ranking cause**, although the decisive intervention occurred earlier, in GCSE and in what survived until flow.
- **The global staged residual got much worse along the productive branch.** Fixing the address mechanism temporarily disturbed a different root-address mechanism; structural block misalignment then amplified the reported residual.
- **The trace parser has an actual pass-order defect:** it places `.addressof` after `.cse2`, although the compiler emits it before GCSE. This manufactures disappearance/reappearance transitions.
- **The search grammar can manipulate surviving operations much more readily than it can introduce a safe operation that is intentionally eliminated later.** Its existing administrative-copy rule is not the missing capability.
- **The notes-only completion prompt equates “no new grouping evidence” with “change nothing.”** This left the older “unmatched and parked” research narrative untouched even after a full match.

The highest-value response is not another general-purpose solver. It is a small set of changes to evidence identity, pass-history tracing, requirement-preserving branch retention, and guarded source construction, backed by this case and negative controls.

## 1. Evidence boundary and reproducibility

### 1.1 What was inspected

This review uses:

- the verified source, `src/overlays/ovl_11/ovl_11_func_80110944.c`;
- the historical diagnosis, `notes/research/ovl_11_func_80110944-untied-high-low.md`;
- the parked handoff, `notes/human-needed-approvals/ovl_11_func_80110944.md`;
- the preserved experiment ledger and complete candidate sources;
- the successful session's tool outputs, including the finalization result;
- saved GCC dumps and the saved exact-candidate trace report;
- the current diagnostic implementations, matching skill, and documentation prompt;
- the vendored compiler's actual pass order and early dead-instruction deletion rule.

The old research note is a **pre-solution historical snapshot**, not a current statement of impossibility. This case study supersedes its open question about whether a compiler modification or assembly workaround is necessary for this function. It does not erase the useful conditional reasoning in that note.

Concrete project/toolchain facts remain authoritative in `configs/project-profile.md` and the active Make configuration. The saved trace used the overlay's `-G0` column, not the executable's `-G8` column. Its extra `-da -dp` options request diagnostics; no function-specific optimization override was introduced.

### 1.2 Identities

| Item | Identity |
|---|---|
| First exact scratch source, `plain-c/dead-root-animal.c` | source SHA-256 `8fc1113b11f3808001e815fccd7d3fb521bccb1fbaa6ea2c02c35cd3295cf410` |
| Integrated, commented `s32` version used for the saved compiler trace | source SHA-256 `831766506eb62ce46b8d7908db0b83c2099cf191c10083e496fd9dcec53cc51e` |
| Final verified `s16` source | source SHA-256 `9634c54ded4b1cf8dbdbadc7ffb15426b5ca88efb1c1a9ca5705017d40949009` |
| Exact output's ledger fingerprint | `8d16553bfbf88d571720cddd2c26a4f7bad6382d39eade7e905bfc55b15dcf78` |
| Verification identity supplied by the completion handoff | `e9ef1fcc539fe4fc4d7f22ee045d16862553a50e3c13a348dee21036c53ea1de` |

These are different kinds of identity. In particular, the ledger output fingerprint hashes a representation of relocated instruction words; it is not the source hash, whole-overlay checksum, or full-workspace verification identity.

The saved trace is of the exact **`s32` predecessor**, not of the final `s16` spelling. The two were independently measured byte-exact. This licenses the trace as evidence that the mechanism is reachable, not an assertion that every intermediate dump of both spellings is identical.

### 1.3 Measured scale, not a reconstructed cost estimate

At the beginning of the successful session's first new measurement, the ledger contained **1,023 rows and 477 distinct output hashes**. The snapshot inspected for this study contains **1,035 rows and 483 distinct output hashes**.

The first exact candidate was the ninth newly recorded source in the successful session; six of those nine produced output hashes not already present in the earlier ledger. This does **not** mean nine compiles were the entire cost: startup diagnostics, repeated measurements, trace compiles, ablation, integration, and finalization are separate. Nor does the ledger establish total model tokens, cost, or the number of earlier human/agent sessions. Do not invent those numbers.

### 1.4 Evidence labels used below

- **Observed:** present in a saved measurement, raw dump, or session tool output.
- **Implementation finding:** supported by reading the current tool/compiler source.
- **Interpretation:** a causal explanation with its stated limits.
- **Proposal:** a change not implemented or benchmarked by this documentation task.

The case proves a successful construction under the measured context. It does not prove the original author wrote these redundant assignments, that this is the only matching C, or that all similar residuals have the same cause.

## 2. The technical problem the harness needed to represent

### 2.1 The two-word symptom

The large-animal arm computes `i * 0xB8`, then addresses the corresponding record. The problematic prefix was:

```asm
/* target, with the stride already in v0 */
lui   a0,%hi(D_800749F4)
addiu v1,a0,%lo(D_800749F4)
addu  a1,v0,v1
lh    a0,0x30(a1)

/* characteristic previous near-match */
lui   v1,%hi(D_800749F4)
addiu v1,v1,%lo(D_800749F4)
addu  a1,v0,v1
lh    a0,0x30(a1)
```

The earlier research established two reasons that ordinary local spelling changes were ineffective:

- local allocation can combine a dying local HIGH with the LOW's destination;
- reload reconstructs a spilled **full address** using the same register for both halves.

A promising alternative was a shared, long-lived **HIGH-only** pseudo, with a separate local LOW. The earlier note already identified multiple references as a route to that state. The unresolved assumption was that a second reference would have to leave a second visible address computation.

### 2.2 The matching source construction

The essential portions of the successful source are:

```c
/* Inside the large-animal arm. */
recordOffset = i * sizeof(Ovl30AnimalRecordB8);
state = Ovl11_SelectionState;
animal = (Ovl11AnimalSelection *)state;
large = Ovl30_AnimalRecords749F4;
animal = (Ovl11AnimalSelection *)(recordOffset + (u32)large);

/* ...existing selection logic and the rest of the outer loop... */

/* After the outer loop, before the result selection. */
large = Ovl30_AnimalRecords749F4;
```

The first `animal` value is overwritten before a read. The final `large` value is never read. Both bindings are ordinary automatic locals; neither is volatile, address-escaped, or pinned to a register.

There are three interacting requirements, not one magic line:

1. share the large-array HIGH without reconstructing a full spilled address;
2. compute the stride early enough to obtain the target's address/register order;
3. retain the state-root optimization behavior while leaving the inner scan's base independent.

The second dead binding solved requirement 3. Putting that binding in `innerBase` instead was a real experiment, not an equivalent transformation of the compiler state: it disturbed the inner scan's own base lifetime.

### 2.3 Why these dead assignments survive early cleanup

This is an especially useful compiler-source finding for future tooling.

`delete_trivially_dead_insns` in `tools/vendor/gcc/2.95.2/src/gcc/cse.c` (around lines 9176–9293 in the inspected tree) first counts uses of each register throughout the function. Its deletion test retains a SET when the destination pseudo has a nonzero use count. It is not yet performing a reaching-definition-sensitive proof that **this particular assignment** supplies no use.

Consequently:

- a fresh local that is never read can be removed before GCSE;
- a dead assignment to an **existing pseudo used elsewhere** can survive that cleanup;
- later flow analysis can remove the dead definition after earlier optimizations have acted on it.

The raw successful dumps demonstrate this for `large` and `animal`. The rule also explains why “assign to a brand-new unused sink” is not an adequate generic recipe. The session's fresh `rootLarge` probe failed to change the final output; the precise deletion boundary for that negative probe was not separately traced, so the rule is an explanation, not a claimed observed trace of that candidate.

This distinction belongs in a source-construction capability: **existing binding versus fresh unused binding**, and **which elimination pass must be crossed**, are material dimensions.

### 2.4 A witnessed pass history

The saved exact trace and raw dumps establish this path:

| Boundary | Witness in the exact `s32` candidate |
|---|---|
| RTL / first CSE | `large` is pseudo 97, assigned at UIDs 163 and 665. Only the first assignment supplies the record-address use at UID 166. The two address expansions initially use separate HIGH pseudos, 126 and 272. |
| GCSE | UID 784 defines shared HIGH pseudo 310. UIDs 162 and 664 become copies from it. The two LOW expressions initially still mention their separate HIGH-copy operands. |
| Loop / second CSE | The large-arm LOW at UID 163 uses pseudo 310. By `.cse2`, the trailing LOW at UID 665 also uses it. |
| Flow | UIDs 664/665 disappear as executable instructions. The large-arm LOW and the shared HIGH survive. The overwritten root assignment, UID 157 after first CSE, also disappears by this boundary. |
| Local allocation | Pseudo 310 carries `REG_EQUIV (high D_800749F4)` and spans the loop/call. Pseudo 97 remains a separate short-lived LOW and is assigned `v1`. |
| Global allocation / reload dump | New UID 871 reconstructs HIGH into `a0`; UID 163 forms the LOW result in `v1` from `a0`. The emitted prefix matches the target. |

A later cleanup removed the extra source operations, but did not undo all optimization history. That is the mechanism the harness needed to make searchable and observable.

Do not read this table using the current report's complete stage ordering without checking §5.1: the parser misorders `.addressof`. The table above follows the actual compiler order and the named raw dumps, not the spurious transitions caused by that ordering defect.

### 2.5 The decisive ablation

Removing only the trailing assignment from the exact scratch candidate produced:

- **231/233 target words**;
- staged key **`[0,0,0,1]`**;
- the remaining allocation difference in the large-arm address prefix.

This is `plain-c/ablation-no-trailing-base.c`, source hash `8ac3f8f68f9174eff1c161561ffa427b8394bbbf93c1c6696796c587a0bf3db0`.

This ablation supports the trailing binding's necessity **within this candidate family and context**. It is not a proof that every matching implementation needs a trailing assignment.

## 3. What the successful search actually did

All paths in this table are relative to `build/ovl_11_func_80110944/plain-c/`. Keys are the recorded `[control-flow, population, schedule, allocation]` values, not a universal distance. In particular, rows with a changed basic-block count carried a degraded-correspondence warning.

| Order | Candidate | Key | Observation / role |
|---|---|---|---|
| 1 | `literal-state.c` | `[1,135,0,10]` | Replacing the large-arm root expression with a literal address did not help. Not part of the winning route. |
| 2 | `dead-base-after-loop.c` | `[1,133,1,10]` | Direct large-array binding plus a trailing binding reached an untied HIGH/LOW pair, but with address/stride order wrong. |
| 3 | `dead-base-stride-first.c` | `[1,132,0,4]` | Explicit stride-first setup produced the correct large-arm prefix. Extra/different state-root code remained. |
| 4 | `dead-base-inner-hoist.c` | `[1,125,0,3]` | Moving the inner base initialization ahead of the outer loop did not resolve that residual. |
| 5 | `dead-derived-base.c` | `[0,6,1,7]` | Returning to a derived state-member base failed to retain the desired overall construction. |
| 6 | `dead-root-consumer.c` | `[1,132,0,4]` | A new unused `rootLarge` consumer compiled to the same output as row 3. |
| 7 | `root-available.c` | `[0,5,1,5]` | Making availability derive from the root changed other address code; not exact. |
| 8 | `dead-root-innerbase.c` | `[0,1,1,1]` | Binding the root through existing `innerBase` repaired surrounding branch/root behavior, but made the inner scan reuse the root differently. |
| 9 | `dead-root-animal.c` | `[0,0,0,0]` | Changing that overwritten binding's destination to existing `animal` reached byte exactness. |
| Control | `ablation-no-trailing-base.c` | `[0,0,0,1]` | Removing the trailing assignment restored the two-word address-allocation miss. |
| Integration | `caller-return-type.c` | `[0,0,0,0]` | The final `s16` return spelling also matched. Full finalization followed. |

Two cautions prevent a misleading success story:

- Row 2 changed both the large-base origin and the second-reference structure relative to the simple prior source. Its result alone does not isolate the second reference. The later ablation and dump history provide that attribution.
- This was not a monotonic walk downhill on the global key. It followed a newly satisfied **named mechanism** while repairing collateral differences elsewhere. An automatic policy that retained only the globally best near-match could have discarded the productive branch.

## 4. What the harness already did well

These should be preserved, not replaced by a new opaque “solve everything” tool.

### 4.1 Persistent, inspectable evidence

The prior research supplied the HIGH-only requirement and conditional explanations of why familiar alternatives failed. The closed-directions record retained assumptions rather than only recording “impossible.” The ledger preserved complete source snapshots, not just hashes of vanished scratch files.

The session was effective because that expensive work existed. The correct lesson is not that the earlier agents should simply have guessed the final line sooner.

### 4.2 A terminal oracle separate from heuristic progress

The byte oracle, relocation handling, and final linked-build gate prevented an attractive intermediate state from being accepted as a match. The finalizer also enforced policy and scope. None of the proposed changes should weaken those gates.

### 4.3 Access to real compiler artifacts

Raw `.cse`, `.gcse`, `.loop`, `.flow`, `.lreg`, and `.greg` dumps made the actual history checkable. The source of the exact compiler provided the reason an unused assignment to an existing pseudo survives early cleanup.

### 4.4 Per-region inspection alongside whole-function summaries

The direct assembly diff made it possible to notice that row 3 had solved the large prefix even while its global residual looked very bad. The failed global ranking did not have to become a failed experiment.

### 4.5 Clean, non-destructive scratch work

The candidates remained under `build/` until an exact source was found. Existing unrelated work was not reset to make the workspace look clean. Final integration used ordinary C and was measured again.

### 4.6 A controlled ablation after the breakthrough

The one-line removal test turned an accidental-looking source oddity into a documented causal dependency. This is a good model for future evidence-backed matches.

## 5. Concrete harness findings and improvements

### 5.1 P0: correct the compiler trace's chronology

**Implementation finding.** `STAGE_ORDER` in `tools/agent/compilerTrace.ts` currently begins:

```text
rtl, jump, cse, gcse, loop, cse2, addressof, flow, ...
```

But `tools/vendor/gcc/2.95.2/src/gcc/toplev.c` emits `.addressof` after `purge_addressof` and `reg_scan`, **before** calling `gcse_main` (around lines 3890–3907). The correct relevant order is:

```text
rtl -> jump -> cse -> addressof -> gcse -> loop -> cse2 -> flow
```

The same table also places `dbr` before `mach`; the compiler's tail order documented by the reversal is `jump2 -> mach -> dbr`. Other stage entries should be audited against their actual dump sites rather than fixed by familiarity with a different GCC version.

**Observed consequence.** The saved trace describes pseudo 310 as appearing in GCSE, disappearing at the supposed `.cse2 -> .addressof` transition, then “first appearing” again at flow. Those are not real compiler events: the older `.addressof` snapshot was placed after newer snapshots. Pseudos 126 and 272 receive similarly misleading transitions.

**Proposal.** Make the compiler-version-specific pass/dump order an authoritative shared description, validated against the vendored pass driver. Use it in provenance and rendering. Preserve the distinction between a dump boundary and a single pass: some adjacent dumps contain several intervening transformations.

**Acceptance tests.** Extend `tools/agent/compiler-trace/compiler-trace.test.ts` with ordered fixtures where a pseudo is introduced only at GCSE. Assert that it is never reported as disappearing into an earlier dump or first appearing twice. Add an integration assertion over this saved case's raw boundaries. Test the final `mach`/`dbr` order separately.

**Priority rationale.** New causal tools built on a false timeline will produce convincing but false explanations. This repair should precede expanding causal synthesis.

### 5.2 P0: distinguish output equivalence from experiment equivalence

**Observed.** Row 3 produced the same output hash as the earlier `barriers/address-pair-a0-scratch.c`. That earlier source contains both a hard-register binding and inline instruction assembly. The new source contains neither.

The harness printed:

> same experiment, different spelling

The later clean ablation was likewise grouped with `barriers/record-area-a0-binding.c`, which pins the area value to `$4`.

The output equality was correct. The conclusion about experimental equivalence was not. Reaching previously assembly-produced words through plain C is evidence of **new source reachability**, even if it is not a new final instruction stream.

**Implementation finding.** `annotateRespellings`, `measurements`, and `priorMeasurement` in `tools/agent/experimentLedger.ts` group by final output hash. `recordAndWarn` in `tools/agent/residualObjective.ts` renders the categorical message. The ledger has no policy-eligibility or pass-state fingerprint field.

**Proposal.** Track separate relations:

1. same source under the same input identity;
2. same normalized state at the pass relevant to the hypothesis;
3. same relocated output;
4. same source-policy class and admissibility.

Continue caching byte comparison by output identity. Do not use that cache key as a universal statement that there is nothing new to learn. A suitable message is:

```text
SAME FINAL OUTPUT as <prior source>.
NEW CLEAN-C REALIZATION: prior source contains assembly/register binding.
Earlier-pass equivalence: not measured.
```

If the prior and current sources are both clean, earlier-pass equivalence is still unknown unless measured. Identical final code is not generally a congruence under future source transformations.

**Acceptance tests.** Extend `tools/agent/experiment-ledger.test.ts` with the two observed clean/assembly pairs. Assert output deduplication remains, policy novelty is visible, and no closed search direction is inferred solely from equal final words. Test a genuinely identical source/context as the cheap no-op control.

### 5.3 P0: select historical baselines by admissibility and freshness

**Observed.** Startup triage described the best historical residual as `[0,0,0,1]`; the corresponding earlier source was the register-pinned area variant. The clean baseline was still at `[0,0,0,2]`.

**Implementation finding.** `best(entries)` ranks exactness and keys without a source-policy dimension. `premiseSurvivalFrom` in `tools/agent/triage.ts` also walks output-distinct rows without such a filter. Ledger schema 2 preserves source/output hashes, but does not persist the complete compile-context, target, policy, and diagnostic identities.

**Proposal.** Keep separate frontiers for:

- currently admissible clean candidates;
- explicitly approved exception candidates;
- diagnostic-only candidates, including unapproved assembly experiments;
- historical candidates whose context or eligibility is unknown.

Before treating a historical result as the current incumbent, validate its inputs or remeasure it. Never silently delete the diagnostic-only results: they can show a useful intermediate construction, as they did here.

A new ledger row should reference an immutable experiment receipt containing source and preprocessing hashes, transitive compile inputs, container/target identity, production flags/toolchain, policy classification, hypothesis, parent candidate, and optional pass-state fingerprints. Legacy rows remain useful, but unknown fields remain unknown.

**Acceptance tests.** A pinned `[0,0,0,1]` must not make a clean `[0,0,0,2]` search appear to have already attained that clean result. A changed header or flag set must make old reachability evidence historical rather than current.

### 5.4 P1: report the observed residual separately from its causal origin

**Observed.** The reversal of the 229-word near-match reported an allocation owner and suggested local quantity priority, reference count, and lifetime tuning. It also reported a failed pre-delay-slot round trip: 15 of 197 reconstructed instructions had no place in the candidate's `.mach` chain.

The report did warn that its reconstruction was provisional. That warning was valuable. However, the allocation decision still stated that agreement in instruction order meant the difference was “inside local-alloc's own ranking.”

**Implementation finding.** The allocation fallback in `tools/agent/pipeline-reversal/decisions.ts` emits that claim with `confidence: "exact"` when a block has allocation differences and no attributed schedule difference. `residualObjective.ts` calls the reversal with replay disabled during scoring.

**Why that is too strong.** Same final instruction shapes and order do not establish the same pre-reload pseudo topology, constant equivalences, reload ancestry, or erased source history. Here a source change before allocation made the relevant value a shared HIGH instead of a full address or a short-lived pair.

**Proposal.** Separate fields such as:

```text
observed_difference: hard-register assignment in address materialization
candidate_mechanism: full-address rematerialization | local tied pair | ...
causal_origin: earlier address sharing/lifetime construction, or undetermined
target_requirement: untied HIGH/LOW is required; its unique source is unknown
replay_validity: verified | diverged | unavailable | not-run
```

For address pairs, inspect candidate `REG_EQUAL`/`REG_EQUIV`, the original HIGH/LOW SETs, and reload-created instructions before prescribing local ranking changes. Attach validity per conclusion rather than allowing an early warning to coexist with an unqualified “exact cause.”

This should extend the existing reversal and compiler trace, not replace them with a model that pretends to recover the target's lost RTL uniquely.

**Acceptance tests.** Use this near-match and winner as a regression pair. Require the report to distinguish full-address reconstruction from shared-HIGH reconstruction, or honestly say unknown. A failed round trip must prevent unsupported causal certainty in machine-readable output as well as prose.

### 5.5 P1: preserve progress on a named requirement even when the global key worsens

**Observed.** The productive stride-first candidate had key `[1,132,0,4]`, far worse than the old clean near-match. Yet its large-arm prefix was exactly right. It had one extra block in the recovered comparison: target 66, candidate 67. Many subsequent block-index comparisons no longer described corresponding code.

The tool printed a `DEGRADED` warning, but also printed a large table and a `NEXT` recommendation built from those comparisons. That presentation encourages a reader to act on the numbers it then qualifies.

**Proposal, part A: validity-aware regional correspondence.** Do not recommend work from unpaired block indices. Match regions by witnessed anchors—call sites, symbol-bearing address constructions, load/control structure—with ambiguity kept explicit. Existing alignment and machine-IR facilities are a better starting point than another positional matcher.

**Proposal, part B: a protected-mechanism frontier.** Preserve candidates that newly meet a verified requirement even when their whole-function key worsens. For this case:

| Goal | Criterion |
|---|---|
| Large address | Untied HIGH/LOW with the target's operand relationship |
| Prefix order | Stride/address/load prefix agrees after relocation |
| Root branch setup | No extra root materialization along the affected paths |
| Inner scan | Independent base and original loop/body construction preserved |
| Terminal result | Full relocated bytes and final gates pass |

The frontier is not a new scalar “distance.” It is a bounded set of distinct, evidenced mechanisms. Keep one or a few representatives per goal combination, with the source and reason for keeping them.

The existing named web-fact tie-breaker in `residualObjective.ts` only breaks ties after the staged key; it cannot protect a mechanism-correct candidate with a worse earlier term. The loop requirement tools already demonstrate a more suitable pattern: score the mechanism's own goals and keep the byte oracle separate.

**Acceptance tests.** Starting with the old clean near-match, the harness must retain row 3 as a mechanism branch rather than discard it. It must not claim that population 132 is 132 independently established semantic errors when the correspondence is degraded.

### 5.6 P1: add guarded, phase-targeted dead-binding recipes

**Implementation finding.** The current `administrative-form` rule in `tools/agent/residual-source-search/rewrite-catalog.ts` is a witness-activated entry-block copy of a never-redefined parameter with subsequent reads redirected. That is useful, but it is not the construction used here. The existing source search does not gain this capability merely by increasing a web-partition or statement-order budget.

**Proposal.** First express a small explicit domain through the existing variant laboratory. Only after that domain has positive and negative tests should it become an automatic grammar rule. Two recipes are sufficient to start:

1. **Dead trailing address binding.** Assign an already-used local pointer a pure symbol address at a point after which that definition reaches no read.
2. **Overwritten root binding.** Assign an already-used local pointer a pure state-root address, followed by a guaranteed overwrite before any read or escape.

Dimensions should be tied to the diagnosed address origin: destination binding, legal insertion region, and symbol/root representation. Do not enumerate arbitrary constants, arbitrary dead arithmetic, or all local combinations.

Each candidate must carry two different claims:

- **semantic safety:** the added definition cannot affect observable behavior;
- **phase requirement:** the definition must survive to the relevant optimizer and disappear before final emission, while changing the named surviving value's history.

The safety gate must reject volatile or escaped destinations, global/member stores, calls or loads in the added RHS, possible traps, uninitialized reads, unsafe pointer arithmetic, and control paths that can read the added definition before overwrite. C89 scope and declaration rules still apply. A loop insertion needs reaching-definition analysis across backedges, not just text order.

A fresh unused sink is an important negative/control alternative. The early cleanup's function-wide use counts make it mechanistically different from an already-used binding. Unknown aliases and macro-hidden effects are refusals or explicit unresolved cases, not opportunities to guess.

**Acceptance tests.** The domain must generate an equivalent of the trailing `large` assignment and the overwritten `animal` root assignment from supported, witnessed inputs. It must distinguish an existing binding from a new never-read sink. It must refuse an escaped or volatile binding and any path with an intervening read. Exact candidates still require full finalization; a solver or grammar witness is never promoted by itself.

### 5.7 P1: trace eliminated operations and surviving effects together

**Problem.** A tool can report “assignment deleted” accurately and still miss the fact that the assignment caused a shared pseudo to be introduced earlier. Conversely, a surviving HIGH can look unexplained if the source operation that induced its lifetime no longer exists.

**Proposal.** Extend compiler provenance with a narrowly scoped **survivor-history** view. For one symbol/expression, report:

- source binding and verified source/UID association;
- SETs and uses at each real dump boundary;
- copies/shared expressions introduced by a pass;
- the interval in which each added definition disappears;
- surviving pseudo lifetime/equivalence and allocation/reload result;
- an ablation link when removal of the source operation was measured.

Label observed UID continuity separately from inferred causal edges. A source-line mapping must use the project's verified diagnostic compile technique, not assume line numbers or pseudo numbers are stable across arbitrary edits.

For this case the compact answer would be:

```text
large: two definitions survive first CSE
GCSE: their HIGHs share pseudo 310
flow boundary: trailing definition removed
lreg: shared HIGH remains global; LOW remains local
reload: a0 supplies HIGH; v1 receives LOW
ablation: removing trailing definition restores the address miss
```

The constructor in §5.6 and observer here are complementary. A good observer without a realizable source move still leaves the agent to guess; a constructor without an observer degenerates into blind dead-code insertion.

### 5.8 P0/P1: make reports question-sized and truncation-safe

**Observed.** The initial experiment-ledger response was truncated to its tail. The closed directions and headline summary, intentionally printed first, were lost. The loop trace printed the running threshold evidence across 26 functions and hundreds of constraints even though the immediate question concerned one address in one function.

**Implementation finding.** `renderLedger` in `tools/agent/experimentLedger.ts` prints the entire history, with expensive closure knowledge at the front. `truncateTail` in `.pi/extensions/psx-decomp/tools/shared.ts` preserves only the last 2,000 lines / 50 KiB. These policies directly conflict. The loop renderer prints all threshold witnesses by default. <!-- doc-ref-ignore: shared.ts is an existing extension-rooted path under .pi/. -->

**Proposal.** Default summaries should preserve, in order:

1. source/context identity and evidence validity;
2. exact result or blockers;
3. the active named requirement and what changed;
4. the admissible incumbent and protected mechanism branches;
5. relevant conditional closures;
6. a bounded recent-history list and paths to full artifacts.

Add explicit history filters/pagination and symbol/pseudo/pass focus. Print the current function's threshold arithmetic by default; make the cross-function proof an explicit request. Apply truncation to optional sections, never silently to the decision/evidence header. Persist a full artifact when the wrapper has to truncate.

**Acceptance tests.** A synthetic 1,000-row ledger must retain the latest incumbent, policy status, and closure premises within the wrapper budget. A focused loop query must not expand into an unrelated corpus report. The summary must name omitted sections and how to retrieve them.

### 5.9 P1: carry an explicit candidate incumbent through parked-stub work

**Observed.** Preparation returned no generated source for this parked function, although a compiling preserved attempt and useful scratch candidates already existed. The live file was an `INCLUDE_ASM` stub with a disabled attempt. Subsequent single-candidate residual calls showed `STUB` and `scored`, not a comparison against the actual research incumbent.

**Implementation finding.** `prepareFunction.ts` recognizes an existing active definition only when the target does not also have an `INCLUDE_ASM` placeholder. Its resume handling preserves edited m2c drafts, but that is not a general selection of the best admissible parked candidate. `residualObjective.ts` takes the live source as baseline; its stub fallback ranks only the candidates supplied in that call.

**Proposal.** A function work item should explicitly carry:

```text
live integration source: stub or compiled C
active research incumbent: immutable candidate + input receipt
protected mechanism branches: candidate identities + reasons
```

On resume, offer preserved candidates with origin, policy, and freshness checks; do not silently activate the highest old key or replace a user's work. A disabled attempt must be extracted through the existing AST/preprocessor guard, not by chopping text around `#if 0`.

Add an explicit baseline-source option or equivalent experiment manifest so a scratch candidate can be measured against its actual parent without touching live source. Continue refusing to score a stub as if it were a compiled solution.

**Acceptance tests.** A parked stub with an admissible compiling preserved candidate should yield a usable measured incumbent even if m2c generation fails. A higher-scoring pinned historical candidate must not be selected as clean. A failed extraction must preserve all original files.

### 5.10 P1: make blocker severity depend on the claim it invalidates

**Observed.** Triage marked the five nested `do { } while (0)` scopes in the 229-word candidate as `phony-loop` blockers. The final match needed none of them, which is a good simplification. But “the loop optimizer did not scan this range” and “this candidate cannot be a valid plain-C result” are different statements.

**Implementation finding.** `phonyFindingsFrom` promotes a phony-loop finding to a blocker based on the existence of an open residual in a nested-loop region; an unknown residual location also retains the blocker. It does not require a witnessed dependency between the discarded loop and the particular hoist claim being made. The matching skill treats blockers as mandatory preconditions to further allocation/scheduling work.

**Proposal.** Classify findings as:

- compilation/semantic/policy failure: cannot promote;
- invalid precondition for a particular analysis: do not use that analysis's conclusion;
- mechanism opportunity or source-quality concern: advisory.

A phony range should block a claim about hoisting *inside that range*. It should not by itself prohibit a different measured construction elsewhere. Recognize one-shot/macro-shaped loops separately from real iteration whose entry was accidentally disrupted. Likewise, an inventory mismatch is evidence to investigate, not automatically a proof of semantic inequivalence when addresses are represented differently.

**Acceptance tests.** Include a genuine missed-loop-analysis case, a one-shot macro loop with an unrelated residual, and an unknown mapping. Preserve the strong warning in the first case; make the other conclusions appropriately scoped.

### 5.11 P1/P2: make address inventory relocation-aware

**Observed.** Triage on the byte-exact scratch candidate still reported a candidate-only constant `0x7AB4`. Memory offsets and shifts agreed. This was not an actual semantic defect in a source whose relocated bytes matched exactly.

**Implementation finding.** `tools/agent/inventory.ts` counts parsed assembly immediates and folds selected `lui`/`ori` forms. That representation-level inventory is not a proof that two relocated address expressions differ in meaning.

**Proposal.** Separate arithmetic literals from symbol-plus-addend address formation, using the same relocation/symbol evidence as the terminal oracle. Preserve both the original expression and resolved address; do not merge arbitrary symbols merely because their names look related. If an inventory comparison is at a different stage from the byte oracle, label its stage explicitly.

On an exact candidate, a disagreement should become a diagnostic-consistency finding, not advice to “fix” the source. Semantic claims should require normalized access/effect evidence, not only a raw immediate multiset.

**Acceptance tests.** This exact candidate must not be reported as semantically defective because of the address addend. A genuinely changed field offset or bit mask must still be detected. Unresolved relocation remains unknown, not equal.

### 5.12 P1: replace categorical search instructions with conditional evidence rules

**Observed in the active matching skill.** The skill says that identical compiled words are not a new experiment, that `ALREADY MEASURED` should be believed and abandoned, and that after classification no second diagnostic may precede an edit/measurement. It also directs reversal of worse variants and presents the staged key as the iteration metric.

Those rules have reasonable purposes: prevent repetitive speculation, force actual experiments, and avoid byte-score hill climbing. Applied literally, however, they can prevent the successful behavior in this case:

- a clean realization of an assembly-only output is new admissible reachability;
- a worse global residual can contain a newly satisfied address requirement;
- following a HIGH from CSE through GCSE, flow, and reload may require several views before a responsible source edit;
- a larger number of unsuccessful outputs does not prove the remaining answer is outside source spelling or that a compiler defect is likely.

**Proposal.** Keep the observe–hypothesize–edit–measure discipline, but define the unit as a **bounded causal question**, not a tool-call quota. Before further diagnostics, name the unresolved decision and the evidence needed to distinguish its alternatives. Stop reading once those alternatives are distinguished. Conversely, do not force an arbitrary edit merely to earn another tool call.

Permit a branch to survive a worse key only with a recorded new mechanism fact and a bounded follow-up question. Condition all “closed” claims on context, grammar, origin representation, and phase state. A rule read from the compiler can close one state, not all source programs that reach the same final instruction shapes.

The earlier closed-directions records already model conditional premises better than the absolute prompt language. Build on that practice rather than weakening scientific discipline.

**Acceptance tests.** Prompt snapshots should no longer equate final-output equality with universal experimental equivalence. A recorded mechanism improvement must survive the incumbent-selection path. Fresh completed prototype/SDK audits should be acknowledged rather than repeatedly listed under “what has not been audited.”

### 5.13 P1: finish the research record independently of grouping

**Observed.** After the successful finalizer, the documentation role said that if there was no new grouping evidence it should “say so and change nothing.” The agent complied. The earlier research note consequently still begins “Unmatched and parked,” and the historical human-needed note still describes a parked stub.

**Implementation finding.** That instruction is produced by `groupingsMessage` in `.pi/extensions/psx-decomp/autoloop/prompts.ts`. The notes-only role correctly protects the verified build inputs, but conflates three independent tasks:

1. updating same-TU membership, only when new evidence exists;
2. recording the mechanism and ablation of a difficult match;
3. resolving stale status/approval pointers.

**Proposal.** Separate those tasks. “No new grouping evidence” should mean **do not change the grouping ledger**, not “leave a known-obsolete problem statement as current.” Keep historical measurements and conditional closures intact; add a resolution banner and verified-source/receipt link. If the notes were user-authored or concurrently changed, use an explicit pending documentation item rather than overwriting them.

For unusual codegen-sensitive source constructs, finalization should request a short rationale and at least the available ablation evidence. This is documentation, not an automatic permission to insert source workarounds or to commit.

**Acceptance tests.** An exact function with no grouping update should still be able to resolve its old research status. Documentation must leave all build-input fingerprints unchanged. Failed documentation must remain distinct from failed machine verification.

### 5.14 P2: separate function-local policy from workspace-wide findings

**Observed.** A scoped source-policy call for this function initially failed on `src/overlays/ovl_19/ovl_19_func_800B95D4.c`, an unrelated concurrently changing file. Later full finalization passed. This session did not need to alter that other source to solve 80110944.

**Implementation finding.** `tools/agent/sourcePolicy.ts` combines a selected function scan with changed-file/patch auditing over the workspace. That can be a legitimate safety check, but the text “1 function(s) scanned” obscures the second scope.

**Proposal.** Report separate outcomes for the requested candidate, worker-attributed changes, and unrelated workspace changes. Preserve all required workspace-wide checks in the final gate. Attribute findings using a session baseline and content identity; do not silently reset another worker's files or auto-grant an exception.

**Acceptance tests.** A scoped clean-C candidate plus an unrelated forbidden edit must produce both facts clearly. The final workspace gate must still fail when the configured policy requires it. This proposal improves attribution, not permission.

## 6. A small, implementable architecture

The aim is to connect existing capabilities, not add another sprawling search stack.

### 6.1 Minimal evidence record

A proposed versioned measurement receipt could contain:

```text
candidate
  source hash + preserved source
  parent candidate and exact transformation
  compile/target/context/policy identities

question
  target region and symbol/expression
  observed symptom
  mechanism alternatives
  phase requirement and semantic invariants

measurement
  terminal oracle result
  staged residual + correspondence/replay validity
  requirement observations: met / not-met / unknown
  relevant pass fingerprints + raw artifact hashes
  policy eligibility

novelty
  source-equivalent?
  relevant-pass-equivalent?
  output-equivalent?
  newly admissible realization?

disposition
  incumbent / protected mechanism branch / rejected / historical
  reason and bounded follow-up
```

Unknown fields must remain explicit. Absence of a dump is not equality of pass states. A source with identical output but a different unmeasured history is not automatically a new useful branch either: preserve it when the hypothesis or policy distinction supplies a reason.

### 6.2 Reuse points

| Capability | Existing implementation to extend |
|---|---|
| Pass order and pseudo survival | `tools/agent/compilerTrace.ts`, `tools/agent/compiler-trace/pseudo-provenance.ts` |
| Residual observation, validity, regional comparison | `tools/agent/pipeline-reversal/decisions.ts`, `tools/agent/pipeline-reversal/compare.ts`, `tools/agent/residualObjective.ts` |
| Input identity and historical candidates | `tools/agent/provenance.ts`, `tools/agent/experimentLedger.ts`, `tools/agent/closedDirections.ts` |
| Complete-source mechanism probes | `tools/agent/fuzzVariants.ts`, `tools/agent/variant-lab/` |
| Guarded AST transformations and finite grammar | `tools/agent/residual-source-search/semantic-graph.ts`, `tools/agent/residual-source-search/rewrite-catalog.ts` |
| Candidate effects and source eligibility | existing C source guard and source-policy helpers |
| Focused output | tool-specific renderers and `.pi/extensions/psx-decomp/tools/shared.ts` <!-- doc-ref-ignore: existing extension-rooted path --> |
| Parked-source handoff | `tools/agent/prepareFunction.ts`, `.pi/extensions/psx-decomp/tools/prepared-attempt.ts` <!-- doc-ref-ignore: existing extension-rooted path --> |
| Completion and research resolution | `.pi/extensions/psx-decomp/tools/finalization.ts`, `.pi/extensions/psx-decomp/autoloop/prompts.ts` <!-- doc-ref-ignore: existing extension-rooted paths --> |

The current administrative-copy and scheduler-witness machinery should not be advertised as supporting dead bindings until its grammar and tests actually do so. Conversely, the source recipe need not wait for an abstract allocator SAT solution: this case already supplies a compiler-source-backed, bounded construction hypothesis.

### 6.3 A better decision path for this symptom

A proposed future route would be:

1. Load the best fresh admissible candidate and relevant conditional closures.
2. Recognize the address-pair mismatch; inspect whether the candidate reconstructs a full address or a HIGH alone.
3. State the target requirement without claiming a unique original source.
4. Check the early-elimination rule and generate a small safe second-reference domain, including existing versus fresh bindings.
5. Trace only candidates that cross the desired phase boundary; measure all through the byte oracle.
6. Retain a candidate with the correct address prefix even if another region regresses, with that regression explicitly located.
7. Repair the separate state-root effect while protecting the achieved HIGH/LOW requirement.
8. Run the ablation, integrate, and finalize normally.
9. Record the verified mechanism and resolve obsolete research status, independently of TU grouping.

This is a counterfactual workflow proposal, not a measured claim that the harness would solve the function in a particular number of compiles. Benchmark it before claiming a speedup.

## 7. Regression and evaluation plan

### 7.1 Use this case as a replay, not as the whole benchmark

Freeze text fixtures and their input identities for:

- the earlier clean common-animal near-match;
- `dead-base-after-loop.c`;
- `dead-base-stride-first.c`;
- `dead-root-consumer.c`;
- `dead-root-innerbase.c`;
- the first exact `s32` winner;
- its no-trailing-binding ablation;
- the final verified `s16` source;
- the two historical assembly/pinned sources that share output hashes with clean candidates.

Keep binary artifacts out of version control. Tiny text/dump fixtures can test parsing and presentation; production-compiler integration tests can regenerate complete artifacts under `build/`. Guard fixture/context drift rather than silently updating expected numbers.

The scratch `s32` source is the correct baseline for replaying the historical ablation. The final `s16` source has equal final bytes, but that alone does not prove edits to both sources will preserve equal intermediate behavior—the exact equivalence mistake this study warns against.

### 7.2 Required positive and negative tests

| Test | Expected result |
|---|---|
| GCSE-only pseudo introduction with the real pass order | One introduction; no fabricated disappearance/reappearance through `.addressof` |
| Existing used local receives a dead trailing address | Can survive early cleanup; observer reports the actual boundary, not a guaranteed effect |
| Fresh never-read local receives the same address | Negative/control route; no assertion that it preserves the same sharing opportunity |
| Overwritten binding with an intervening read on one path | Recipe refused |
| Volatile, escaped, or nonlocal destination | Recipe refused |
| RHS containing a call, load, trap risk, or unsupported expression | Recipe refused or explicitly unresolved |
| Clean source and assembly source emit identical words | Output-equivalent, not equivalent admissibility |
| Same source/context measured again | Cheap repeat; no artificial progress |
| Same final output with different earlier-pass fingerprints | Both histories remain distinguishable for relevant follow-up hypotheses |
| Correct address prefix plus degraded whole-CFG correspondence | Preserve the verified local goal; do not recommend a wrong ordinal block |
| Exact candidate with raw address-addend inventory difference | Diagnostic-consistency warning, not a source semantic blocker |
| Thousand-row history through bounded wrapper | Decision header and closure premises survive; history is paged |
| Parked stub plus preserved clean candidate plus failed m2c generation | Existing research candidate remains available and separately measured |
| No new TU grouping after a match | Grouping ledger unchanged; research resolution can still complete |
| Unrelated workspace policy finding | Correct scope attribution; no weakening of final policy enforcement |

### 7.3 Generalization cohort

Do not train and evaluate solely on 80110944. Include:

- a known multi-reference HIGH-sharing case, such as the historical note's `ovl_11_func_80116980` precedent;
- ordinary tied local address pairs that should remain ordinary, not trigger a dead-binding search;
- full-address rematerialization cases;
- a genuine semantic mismatch in an offset or mask;
- a scheduler-only or local-priority residual for which the earlier address-history route is irrelevant;
- a one-shot macro loop and a genuinely disrupted loop;
- at least one held-out function with calls and register pressure.

Measure capability coverage and negative behavior, not only exact matches. An unsupported target or unresolved source-safety condition belongs in the denominator and must be named.

### 7.4 Useful metrics

- Time/compiles until the correct mechanism is identified, separately from time until exactness.
- False confident pass attribution, especially local-ranking claims for earlier address-sharing causes.
- Fraction of generated candidates with proved source-safety conditions.
- Distinct relevant pass states reached, alongside distinct outputs and distinct policy classes.
- Mechanism-correct branches incorrectly discarded by ranking/deduplication.
- Evidence delivered within the default report budget; whether decision headers survive.
- Repeated audits whose inputs have not changed.
- Research-status links still reporting an unmatched function after verified completion.

Do not optimize merely for more “novel experiments”: the point is useful distinctions, not inflating a counter by turning off deduplication.

## 8. Delivery priorities and non-goals

### Phase A — repair the evidence before enlarging the search

1. Correct and test pass/dump chronology.
2. Change “same experiment” into properly scoped output-equivalence reporting; add policy-aware historical selection.
3. Preserve report headers and paginate history.
4. Stop issuing actionable ordinal-block recommendations from degraded correspondence; expose validity in JSON.
5. Split research-resolution duties from grouping duties.

These are comparatively bounded changes with direct counterexamples in this case. They can improve existing agents without adding a new search engine.

### Phase B — connect observation to a safe source move

1. Add the symbol-focused survivor-history view.
2. Build a small explicit dead-binding probe domain with the existing variant laboratory.
3. Add semantic-safety guards and a protected mechanism frontier.
4. Integrate those capabilities into resume/preparation and only then into an automatic grammar.

### Phase C — prove generality

Run the replay and held-out cohort, report unsupported cases and false positives, and revise the recipe bounds from measured results. Preserve baseline compile costs and input identities so improvements are comparable.

### Non-goals

- Do not patch the compiler for this solved function.
- Do not weaken exact-byte, linked-build, source-policy, or scope gates.
- Do not treat an arbitrary dead assignment as evidence of original source history.
- Do not count a policy workaround as the clean incumbent because it has a better residual.
- Do not replace one misleading scalar with another universal “distance.”
- Do not search unlimited dead code or use undefined behavior to perturb compilation.
- Do not auto-edit another worker's files, auto-grant exceptions, or auto-commit a match.

## 9. Source quality and honest reconstruction claims

“Plain C” and “obviously idiomatic original source” are not synonyms. The accepted function contains deliberate, redundant local bindings. They are legal, side-effect-free source constructions, but a casual cleanup would reasonably try to remove them.

The harness should make that tradeoff explicit:

- retain a short source comment and a durable case-study link;
- preserve the controlled removal measurement;
- distinguish codegen necessity in this representation from logical necessity;
- allow future refinement to replace the construction if a more natural exact source is found;
- never describe the final C as proven historical source merely because the bytes match.

The final source already comments on the bindings. Its older linked research note needs a resolution entry in a separately authorized documentation update; this study intentionally does not rewrite that historical file or the grouping ledger.

The broad lesson is not that enough persistence always finds a spelling. It is that a model of only the surviving program can exclude the very source operations that caused the surviving program to look that way. Here, prior work had correctly identified the desired state. The successful session supplied a phase-sensitive construction and kept it alive through temporarily worse global measurements.

**For future functions, the harness should preserve causal history, admissibility, and named requirements—not just final instruction similarity.**

## Appendix A. Artifact map

These are local generated artifacts, not files to commit. Their contents can be removed by build cleanup; the source snapshots and hashes above identify what needs preserving for a regression fixture.

| Artifact | Purpose |
|---|---|
| `build/experimentLedger/ovl_11_func_80110944.jsonl` | Timestamped source/output identities and staged keys |
| `build/experimentLedger/sources/ovl_11_func_80110944/` | Content-addressed complete C snapshots |
| `build/ovl_11_func_80110944/plain-c/` | Successful-session candidates, selected diffs, and saved trace JSON |
| `build/ovl_11_func_80110944/plain-c/exact-trace.json` | Saved structured trace of the exact commented `s32` version; subject to the chronology defect described above |
| `build/compilerTrace/ovl_11_func_80110944/` | Raw production GCC dumps and generated assembly/object; mutable per-function workspace, so verify identity before reuse |
| `build/ovl_11/asm/nonmatchings/ovl_11_func_80110944/ovl_11_func_80110944.s` | Original assembly used by the session |

Two important output-equivalence pairs, observed in the ledger:

| Earlier diagnostic source | Later clean source | Shared output fingerprint |
|---|---|---|
| `barriers/address-pair-a0-scratch.c` | `plain-c/dead-base-stride-first.c` | `5e4bf43c5aa0c59f7c132904f00cc4532f925259402ff768b7f459af2c7bbfe6` |
| `barriers/record-area-a0-binding.c` | `plain-c/ablation-no-trailing-base.c` | `90cd56b2d589efab940bcec9f61a1330819e94df3335bb1dc931ba0934d786e7` |

The paths in that last table are relative to `build/ovl_11_func_80110944/`. The earlier files visibly contain the assembly/pinning constructs; the shared hashes establish final-output equality, not policy equivalence or identical compiler histories.

## Appendix B. Inspection anchors

Line numbers below refer to the tree inspected for this study; use the named functions if files move.

- `tools/vendor/gcc/2.95.2/src/gcc/toplev.c`: `.addressof` before `gcse_main`, approximately 3890–3907; loop and second-CSE sequencing immediately follows.
- `tools/vendor/gcc/2.95.2/src/gcc/cse.c`: `delete_trivially_dead_insns`, approximately 9176–9293; function-wide use counts versus particular-definition liveness.
- `tools/agent/compilerTrace.ts`: `STAGE_ORDER`, `dumpFiles`, `buildTraceReportFromArtifacts`.
- `tools/agent/compiler-trace/pseudo-provenance.ts`: stage-to-stage pseudo transition construction.
- `tools/agent/experimentLedger.ts`: `annotateRespellings`, `measurements`, `priorMeasurement`, `best`, `renderLedger`.
- `tools/agent/residualObjective.ts`: `score`, `render`, `recordAndWarn`, stub-baseline handling.
- `tools/agent/pipeline-reversal/decisions.ts`: `reduceToDecisions`, allocation-only fallback.
- `tools/agent/pipeline-reversal/render-text.ts`: round-trip warning and decision rendering.
- `tools/agent/triage.ts`: `premiseSurvivalFrom`, `residualNest`, `phonyFindingsFrom`.
- `tools/agent/loop-trace/render.ts`: running threshold witness rendering.
- `tools/agent/residual-source-search/rewrite-catalog.ts`: administrative-copy rule and its witness gating; `tools/agent/residual-source-search/README.md` states the rule's current bounds.
- `tools/agent/prepareFunction.ts`: active-definition detection and `existing-attempt` selection.
- `.pi/extensions/psx-decomp/tools/shared.ts`: `truncateTail`. <!-- doc-ref-ignore: existing extension-rooted path -->
- `.pi/extensions/psx-decomp/autoloop/prompts.ts`: `groupingsMessage`.
- `.pi/extensions/psx-decomp/tools/prepared-attempt.ts`: `documentCompletion` input-identity protection. <!-- doc-ref-ignore: existing extension-rooted path -->
- `.pi/skills/psx-decompile-function/SKILL.md`: experiment-equivalence, diagnostic cadence, and variant-retention instructions.

## Appendix C. Validation of this documentation change

This study does not claim that any proposed harness feature has been implemented or tested. The function's successful finalization is prior supplied evidence. The documentation reference checker examined 72 references and reported zero stale paths; five extension/skill paths were additionally checked directly because the reference scanner misreads nested extension-rooted `tools/` paths. Code fences and trailing whitespace were checked, and the verified function's source hash remains `9634c54ded4b1cf8dbdbadc7ffb15426b5ca88efb1c1a9ca5705017d40949009`. No source, header, build configuration, tool implementation, policy allowlist, or generated binary was changed by this documentation task.
