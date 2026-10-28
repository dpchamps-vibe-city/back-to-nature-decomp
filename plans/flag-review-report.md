# Review of standing compiler-flag exceptions

Reviewed tree: `6cfec884` (the snapshot inventoried in `plans/flag-review.md`).
This is an evidence review, not a flag-removal patch. No C source, compiler
configuration or policy allowlist was changed.

## Conclusion

**The concern is justified. The standing exceptions are not all established
facts about the original build.** Some are now demonstrably unnecessary;
others explain the current reconstruction but have weak or conflicting
translation-unit evidence. Several are reasonable historical hypotheses,
particularly where a specific optimization asymmetry recurs in a coherent
local group. None of the reviewed evidence supplies the original game's
build command or makefile proving the particular switch.

The actionable results are:

1. **Two complete overrides can be retired without changing the current C:**
   `func_80022794` and `ovl_11_func_800F1678` match at baseline.
2. **Two overrides are broader than necessary:** `func_8002470C` and
   `func_8002495C` need only `-fno-schedule-insns` for their current source.
   Removing `-fno-schedule-insns2` preserves exact matches.
3. **One allowlist entry is stale:** `func_80019610` has no override and
   freshly matches at baseline.
4. **The strongest candidates for reconstruction-related flag debt are the
   two counter functions, the two `ovl_21` exceptions, and
   `ovl_15_func_801335F8`.** Their present flags affect the output, but
   neighboring, related functions contradict a straightforward common-TU
   explanation. `ovl_17_func_800B9158` also has particularly weak historical
   evidence: the entire baseline discrepancy is one commutative operand order.
5. **Do not remove the remaining flags indiscriminately.** Twenty-four of
   the 26 current override-bearing functions fail the baseline byte oracle.
   A weak historical explanation is not permission to break a matching build.

The mechanical cleanup alone would reduce **26 override entries / 32 option
instances to 24 entries / 28 option instances**, and 27 flag-allowlisted
functions to 24. This review has not applied that cleanup.

## What was actually tested

I reviewed every active entry and evidence comment in
`configs/flag_overrides.mk`, reconciled them with `.pi/autoloop.json`, read
relevant source and grouping/research notes, and performed **254 fresh
compile-and-relocate comparisons across 110 distinct functions**:

- **65 exception checks:** baseline and configured settings for all 26
  entries; both individual settings for each of the six two-flag entries;
  and baseline for the stale allowlist entry.
- **189 regional checks:** the current settings for 88 distinct functions,
  plus 101 distinct regional flag applications. These include available
  immediate link neighbors for every exception and selected stronger
  shared-data/caller/group witnesses. All 88 current-setting controls match;
  **31 of the 101 regional flag applications change the target bytes**.
- Every compilation explicitly used `useOverrides: false`, then supplied
  the intended options. Executable and overlay container defaults remained
  separate (`-G8` and `-G0`). Regional controls retained any existing override
  belonging to the witness; regional probes added the investigated flags.
- Verdicts came from `compareFunction`, the relocated-byte oracle, not the
  flag probe's masked score. There were no compile errors or undetermined
  oracle results.
- **`make -j8 check-all` passed:** the executable and all 13 configured
  overlays still match their original bytes.

Local artifacts, including actual flags, source/preprocessed/object hashes,
objects and instruction diffs:

- `build/flagReview/audit.json`, `audit.log`, `audit.ts`
- `build/flagReview/regional.json`, `regional.log`, `regional-links.json`
- `build/flagReview/regional-requests.json`, `regions.ts`
- `build/flagReview/check-all.log`

These are ignored diagnostic artifacts, not committed deliverables. The
report below preserves the substantive results if those artifacts are cleaned.
No source-rewrite search was performed, and no original-TU boundary was proved.

## Subsequent object-archive TU resolution

The complete eleven-entry ovl_11:800E499C–800E5230 unit now folds as
`modules/object_program_install.c` at baseline -O2/-G0. Directly indexing
E4BA4's archive halfword tags, instead of separately stepping a pointer,
reproduces the one-hoisted-address target with both loop passes enabled.
`foldTranslationUnit.ts --write` confirmed every member and the linked overlay.
The -fno-rerun-loop-opt override and its permission are removed, not promoted
to a module setting. Earlier pointer-walk/local-copy/s16-index failures were
conditional on those source inputs; they did not establish baseline
unreachability for the indexed header representation. The grouping ledger
records the complete membership, interfaces and evidence-limited boundaries.

## Subsequent scene and slot-menu TU resolutions

The complete ovl_17 code span (61 entries, 800B7E38–800BB454) now compiles as
`modules/record_scene.c` at baseline. Using the existing queue entry-member
lvalues at 9158 preserves the distinct word/halfword address-expression orders
lost by raw byte-pointer arithmetic. Its -fno-cse-skip-blocks and permission
are retired, not moved to the whole unit. Earlier alternate-anchor and
loop-guard failures were conditional on those source inputs.

The complete twelve-entry ovl_11:800F787C–800F84F0 menu/slot unit similarly
folds at baseline as `modules/record_slot_menu.c`. Explicit compactor
backedges preserve the original recomputed index addresses without the
LOOP_BEG introduced by the former outer do-while. Its -fno-strength-reduce
and permission are retired. The earlier matrix measured that loop-shaped
reconstruction, not every baseline C source.

Both checked folds prove all member text and the linked container. The
subsequent batch gate passes the EXE, all thirteen overlays, associated data,
interfaces, source policy, convergence and exception non-growth at 2464/2625
folded functions. No module flag or new permission was needed. Membership
and conservative protocol boundaries are recorded in the grouping ledger.

## Subsequent condition-record and simulation findings

The complete 28-entry condition-evaluation protocol now includes F0E00's
installer. Three source assignments form its count*36 offset before the
integer address addition, retaining the target's indexed scan with both loop
passes enabled. Its no-strength-reduce override and permission are retired.
The complete seven-entry EEECC–EFA1C dated-condition protocol also folds at
baseline: EF8BC's structured rejection returns, case breaks and shared success
return retain its low-half rematerializations without no-skip. Both checked
folds preserve every member and the linked overlay. The following full batch
gate passes all eight integrity checks at 2472/2625 folded, 469 valid modules,
including EXE/all thirteen overlays and data. No module flag replaces either.

The 98CC simulation predicate's inverted branches and shared result return
likewise preserve its two store/return tails at baseline; no-schedule-insns
and its permission are retired. The whole-46 scene trial proves its 16 words
and 44 other members exact, with only BA698 still differing at that stage. That helper's
held-base form differs in five allocation words under baseline; no-GCSE is
exact. A loop-local/direct-field base lets PRE hoist the full address and is
two instructions short. Reading a genuine typed field through the held base
is neutral, not a fix; the second asm-name proposal was rejected. The
42-member scene/state/draw merge and three query entries are therefore a
mechanically bounded integration of the same unfinished original-file family,
not evidence of original cuts around BA698. Its original override stays
local and unchanged, with no exception growth.

For ovl_15, the 344E8 interface conflict is independently resolved by a
halfword result/count plus word scan index and direct if/else return. The
checked twelve-member selection fold and its s16-declaring caller remain
exact. At that stage the supported renderer extension 335F8 was blocked: natural
narrow coordinates remove the alleged PRE necessity but change allocation
and next-X placement; uniform no-GCSE changes 33808 and 331C4. Its existing
setting is not generalized and its temporary integration edge is not claimed
as historical. Earlier claims of universal baseline unreachability for all
these functions exceeded their measured source premises.

## Subsequent complete option-row TU resolution

Both complete runs now fold at baseline: eleven prompt_option_rows entries
(8011A9CC–8011B6C4) and five price_option_rows entries
(80113CD0–80114604). The last gated row must complete the unrolled append:
advance its text cursor at the branch join, then post-increment each of the
three parallel field cursors on its store. This retains all target pointer
walks. Advancing text inside both branch arms still folds them, while separate
pre-store increments change the row being addressed; constant-index struct
fields also fold. The source preserves the ordinary complete row append, not
an isolated arithmetic/alias knob. Every member and the linked overlay pass
both checked folds. Four two-option overrides (AA64, AC30, ADF4, 13FF0) and
their permissions are retired; no module setting replaces them. Old blanket
unreachability claims were conditional on the measured incomplete cursor
lifecycles. The grouping section records full membership, callback-word and
boundary evidence, canonical identities and the three actual C-call interfaces.

Earlier campaign verification after names, headers and caller reconciliation:
**2509/2625 folded (112 above the 2397 receipt), 472 valid modules**; all eight
integrity gates PASS, including linked EXE/all thirteen overlays and associated
data, source policy and exception non-growth. The receipt listed 24 override
entries; 12 remain, with no module-scope replacement. The unreconciled renderer
335F8 and range helper BA698 are explicitly documented mechanical limits, not
asserted original TU boundaries. No commit was made.

## Complete ovl_15 slot-selection resolution

The entire thirteen-entry 801335F8–80134770 unit now folds into
modules/save_slot_selection.c at BASELINE. 335F8 is named Ovl15DrawSlotBanks;
its public header documents the two banks, occupied-only vertical compaction,
pixel coordinates and unchecked record-index prerequisites. Its caller uses
the published interface, and the row helper is no longer exposed as a separate
public service. Names/addresses, linkage and all ABI types remain consistent.

Two source reconstructions explain the old exception. First, compute nextX
once at the base-selection JOIN; GCC itself duplicates the add into both
arms, while keeping the signed X conversion at the latch. The former two
source assignments allowed PRE to lift half that conversion. Second, cache
the FULL mark-table address before the outer loop; a bank byte offset is
then added to that cached address, offset first. This removes the HIGH-only
hoist/lifetime and closes the preheader register residual. Typed pointer
addition reverses one commutative operand pair, so the readable source retains
the documented offset-first integer address expression, over the real table.
The cached-address dump reports zero PRE substitutions/insertions in this
renderer. No literal RAM address, fabricated alias, dead store or pin is used.

The renamed/readable complete source is exact for all thirteen members at
baseline, and foldTranslationUnit's checked extension proves the linked
ovl_15 too. The -fno-gcse entry and its permission are REMOVED, not extended
to a module setting. No companion rewrite or alternative profile is required;
the scratch row-helper recordType experiment is not integrated. The old
12/13, 6/13 and 3/13 matrices below are valid controls for the superseded
source, not current blockers or proofs of original flags. The grouping
ledger records the complete membership and protocol-boundary evidence.

Final post-integration gate: **2510/2625 folded, 472 valid modules**, all eight
integrity categories PASS, including complete linked EXE/all thirteen overlays
and data, policy and exception non-growth. This adds the last renderer to the
unit (113 new folds above the campaign receipt); eleven overrides remain
versus the receipt's twenty-four. The full gate follows canonical naming,
public-header/caller reconciliation and stale internal-comment cleanup.
No generated output was hand-edited and no commit was made.

## Complete ovl_21 scene-simulation resolution

The whole supported 46-entry 800B7E3C–800BA9F8 run is now one
modules/scene_simulation.c, including BA698 and all three group queries.
The checked fold proves all 46 relocated functions and linked ovl_21 exact
at baseline. BA698 is Ovl21FindFirstProjectileInRange: first active projectile
within the signed/narrowed X/Z distance threshold, returning its writable
active word or NULL without consuming it. Its two callers remain inside the
unit. The shared origin names x/z retain the ABI/layout; a corroborated biased
search view replaces the C-local partial type. No new storage/alias is added.

The original baseline residual was allocation-only in two blocks (five words).
A guarded do-loop with the count-base birth AFTER the entry guard removes
its high-half lifetime overlap with the initial bound: three words close.
The remaining two came from an isolated local address temp being assigned
v0. Reusing that address scratch for the existing per-slot active-word load
makes a multi-set global web: lreg records two SETs/two deaths, and greg
handles the recurrence. Both assignments are used; all target instructions
remain, without additions. The complete source is 46/46, BA698 86/86 exact.

Thus the 'sole local quantity must get v0' proof and alleged need for
-fno-gcse were conditional on the split source webs. A new reading of the
program's temporary reuse closes what no scheduling/priority spelling of
those fixed webs could. PRE remains enabled and may share the high half;
there is no need to resurrect the removed local HIGH pseudo. A no-GCSE
full-46 control of the earlier source matched 28/46; it was never promoted.

The old function override and its permission are REMOVED. No module flag,
policy exception, speculative boundary or forbidden construct replaces them.
The complete private query interface is now in-unit forward declarations,
not an unnecessary cross-module header; external linkage is unchanged.
Existing phase/draw public contracts remain, with zero failing published
interfaces. Full-TU controls and pass dumps are preserved under
build/tuFlagReview/ovl_21/scene_simulation; the grouping ledger records the
membership/left/right evidence and matching-sensitive constructions.

Fresh final acceptance after names, type views and private-interface cleanup:
**2511/2625 folded, 471 valid modules**, all eight integrity categories PASS,
including the complete linked EXE, all thirteen overlays/data, source policy,
convergence, input stability and exception non-growth. The campaign is 114
new folds above its receipt; ten overrides remain versus its original 24.
No module setting replaces either retired hard-case override. No commit made.

## Whole-seven progress-counter control

A fresh complete ovl_11:800F3BCC–800F3F7C merge is **6/7 exact at
baseline** (3D40 differs) and **6/7 exact with -fno-gcse** (3BCC differs).
The old regional compatibility checks therefore do not account for the whole
current TU source. All fourteen member/profile verdicts are determinate,
with successful compilations and no live source or setting change.
Reports, merged sources, production compiler/toolchain identity, objects and
instruction listings are under build/tuFlagReview/ovl_11/progress_counters.

Three complete-TU baseline alternatives change only the reset: indexed
arrays, reuse of its pointer scratch, and explicit loop backedges. All leave
six members exact and the reset nonexact; the backedge form has identical
.text to the unchanged merge (SHA-256 378f9a60312825a4a69df5bd1ec7ef5a8ea8cda0df8d1b40a50a659c3ec3c2cb).
No flagged module or fabricated cut was integrated. These outcomes are scoped
to those sources/context, not an impossibility claim; the next question is
which GCSE rewrite carries the symbol high across the first loop. The unrelated thirteen-entry ovl_30
debug-menu merge passes its checked fold at baseline without a flag change.

## Whole-fifteen hand-value dialog control

A fresh, unchanged merged ovl_11:80103770–801044E4 control supplies the
TU-wide matrix missing from the earlier regional review. Baseline is exact
for 13/15 (3B24/4148 differ); no-skip for 14/15 (4148 differs); no-follow
for 13/15 (3964/3B24 differ); both for 11/15
(3964/3B24/40A8/4148 differ). All compile without diagnostics, with no
undetermined relocated-byte verdicts. Source hashes, full compiler flags,
per-member verdicts and instruction listings are under
build/tuFlagReview/ovl_11/hand_value/merged-matrix.json and its four profile
directories. No uniform profile accounts for the complete current source.

A separate complete-TU baseline control moves only 4148's common +12
signed-column update to the text-selection join. It still leaves exactly
3B24/4148 nonexact (13/15); the candidate and report are retained as
join_column.c / join_column-matrix.json. This is a scoped source experiment,
not a proof of baseline unreachability. It isolates the column-join lever
from the older combined trial in the grouping ledger. No source or flag
change from these controls was promoted; no smaller flag-compatible dialog
module was created.

The independent accepted batch merges six already-matching modules into two:
42-entry mode-1 record/scripts/handoff and 22-entry ovl_25 scene state machine.
Both checked folds use baseline settings and preserve every member and linked
container. The latter's right edge is expressly the unresolved 95B0 mechanical
limit, not an original TU claim. No override or permission was added, broadened
or retired. Final tuProgress verification passes all eight integrity categories
at 2511/2625 folded and 467 valid modules, including every EXE/overlay byte and
data/table placement. The unchanged folded count reflects merges, not new
per-function integrations.

## Prior-source ovl_15 whole-unit flag trial

Freshly merged all thirteen then-current entries at 801335F8–80134770 using
foldTranslationUnit's source merger, without changing the live build. Under
one uniform setting, relocated-byte verdicts are: baseline **12/13 exact**
(only 335F8 differs); -fno-gcse **6/13 exact** (renderer fixed, seven contrary
members); -Os/classic GCSE **3/13 exact**. All compilations have zero
front-end diagnostics and zero undetermined verdicts. These are complete-TU
experiments, not the older split-source regional controls.

The seven current no-GCSE contradictions are 33808, 33B28, 33F4C, 34000,
34134, 342A0, 3468C. The first two lose symbol-high sharing across input
branches/calls; gcse_main includes cprop as well as PRE, so disabling it
removes more than the renderer's disputed hoist. Module-only compilation
reproduces the same differences, excluding this merge's order/visibility as
the cause for this source. The -Os experiment follows gcse.c's explicit
classic-GCSE/PRE branch; it does not provide an alternative exact profile.

One natural local split, s16 recordType = arg0 in the row renderer, makes
34134 exact under no-GCSE. That demonstrates why the seven are conditional
source witnesses, not proof of an immutable original flag boundary. Other
small pointer-cache, details-lifetime and initializer-backedge trials remain
nonexact; the explicit cursor/scroll pointer trial emits 203 words for 33808
rather than 200. No universal no-go claim follows. In particular, mixed
signed/unsigned halfword loads do not rule out a genuine aggregate view with
casts. No setting has been promoted and no source/flag/permission changed.

Reproducible evidence: build/tuFlagReview/ovl_15/save_slot_selection/
report.json, size-report.json and the complete variant sources/objects/diffs.
The existing grouping section records input/compiler hashes and each result.
The next question is a faithful whole-group representation or independently
supported membership—not simply moving the one-function override to a module
and disregarding the seven mismatches.

## 1. Findings established without any source rewrite

| Function | Current override | Fresh result | Recommendation |
|---|---|---|---|
| `func_80022794` | `-fno-rerun-cse-after-loop` | Baseline and configured: **116/116 exact** | Remove the flag and only its flag-allowlist permission. Its register pin and empty asm remain separate debt. |
| `ovl_11_func_800F1678` | `-fno-cse-skip-blocks` | Baseline and configured: **51/51 exact**, clean C | Remove the flag and its flag-allowlist permission. |
| `func_8002470C` | Both scheduling passes disabled | Baseline mismatches; **sched1 disabled alone: 25/25 exact**; sched2 disabled alone mismatches | Remove `-fno-schedule-insns2`; investigate the remaining sched1 exception. |
| `func_8002495C` | Both scheduling passes disabled | Baseline mismatches; **sched1 disabled alone: 25/25 exact**; sched2 disabled alone mismatches | Same. |
| `func_80019610` | None | **123/123 exact** at baseline | Remove the orphan flag-allowlist permission. |

`800F1678` is especially important: its evidence comment says the target
address rematerialization is unreachable from *any* baseline C shape. The
current clean C is a direct counterexample. The comment described an older
source family, not the complete source domain.

`80022794` is a different case. Its current pin/empty-asm hybrid matches at
baseline; this retires the flag requirement **for that hybrid**, not the
claim that a fully clean-C reconstruction remains unresolved.

For both counters, the comments' central sched2 claim is disproved as a
requirement of the current source. Turning sched2 back on does not destroy the
match. The outstanding baseline difference is the sentinel's position; the
current C deliberately reuses `tv` for both `-1` and the incremented byte.
That is an input to sched1's multi-SET decision, not evidence by itself that
the original compiler's scheduler was disabled.

## 2. Regional contradictions that the existing justifications miss

A changed neighbor is a **conditional contrary witness**: it refutes a
common flag setting for the current reconstructions, not every possible
reconstruction of those functions. It also does not prove two adjacent
functions shared a TU. Nevertheless, it is evidence that must be addressed,
not dismissed because today's repository compiles one function per file.

### A. Counter/sprite family: strong reason to reopen the remaining flag

**Subsequent TU reconstruction resolves both exceptions.** The complete
seventeen-function `src/modules/ui_sprites.c` matches production baseline,
including both scheduling passes, when each counter uses byte-global
increment, byte quotient, then subtracts quotient times its period. The
checked fold proved all members and linked EXE; the subsequent campaign gate
passed every container, interface, policy and exception-growth check at
2386/2625 folded functions. Both full overrides and their permissions are
retired. This supplies a clean-C counterexample to the earlier universal
claims, not evidence of original scheduling-off compilation. Fresh-sentinel
and two-statement local-accumulator failures did not cover this construction.
The grouping ledger's sprite-dispatch section records membership, boundaries,
public names/contracts and verification. The reviewed-tree results below
remain the historical controls for the superseded source.

The grouping ledger places the counters among related sprite wrappers and
mod-30 counter consumers (`notes/file-groupings.md:4801`).

- Both immediate neighbors of each flagged counter change under the
  currently configured two-flag setting.
- More importantly, even the **minimal remaining** `-fno-schedule-insns`
  changes `func_80024770`, `800247C0`, `80024810`, `80024860`, `800249C0`
  and the dispatcher `80024A4C`.
- The five mod-30 consumers go from exact 20-word functions to nonmatching
  20-word functions; the dispatcher also changes.

These are not arbitrary nearby code: the consumers share the counter globals,
magic-division idiom and dispatcher. The `8002495C` comment still says the
siblings are unmatched, but they are now matched and available as witnesses.
The two flagged functions being alike is positive family evidence, but does
not resolve this broader contradiction.

**Assessment:** the remaining sched1 flag is a matching mechanism, not yet a
convincing historical TU setting. Audit the sentinel/raw-byte variable reuse
and arithmetic representation before accepting two isolated scheduling-off TUs.

### B. `ovl_21`: the contrary witnesses were already documented

The shared `D_800C0448` record machinery is described at
`notes/file-groupings.md:9758`; its `800B990C` entry explicitly records both
contradictions. Fresh checks confirm them:

| Investigated flag | Related witness | Current → flag-applied emitted words | Result |
|---|---|---:|---|
| `800B98CC`: `-fno-schedule-insns` | Immediate successor `800B990C` | 69 → 68 | Mismatch |
| Same | Caller `800B8B3C` | 279 → 279 | Mismatch |
| `800BA698`: `-fno-gcse` | Caller `800B990C` | 69 → 71 | Mismatch |
| Same | Closely related scan `800B9844` | 34 → 33 | Mismatch |
| Same | Related slot stepper `800BA300` | 112 → 114 | Mismatch |

`800B9844` being unchanged under **no scheduling** does not make it neutral
under **no GCSE**. The existing justification for `800BA698` overlooks this
very useful scan-family witness.

`800BA698`'s own baseline emits the same 86 words in length, with five changed
words in the fresh comparison. Its comment promotes allocator/live-range
sensitivity to an impossibility claim about every source spelling. That does
not follow. Its raw byte-offset loops and separately maintained pointers are
specific reconstruction choices worth revisiting against the now-matched
record types and sibling scans.

**Assessment:** both are high-priority reconstruction audits; `800BA698` is
particularly weak as historical flag evidence. The store/return-copy mechanism
in `800B98CC` is well explained, but still conditional on the chosen C.

### C. `ovl_15_func_801335F8`: acknowledged source-shaped exception

Its current 132-word source needs `-fno-gcse`; baseline also emits 132 words
but is not exact. However:

- Immediate successor `80133808`: exact 200 words → nonmatching 198.
- Shared dual-base machinery member `801331C4`: exact 166 → nonmatching 154.
- Immediate predecessor `80133538` and previously flagged `8013345C` are
  invariant; invariance is not affirmative evidence for disabling GCSE.

The source explicitly carries a shifted coordinate accumulator
(`var_s1 += 0x180000`, then `>> 16`) and other machine-shaped loop state.
The comment itself admits the source is m2c-shaped. A nearby no-GCSE exception
was already removed after a typed, idiomatic rewrite.

**Assessment:** high-priority candidate for a source-induced exception.
User authorization makes it permitted, not historically established.

### D. The `80103B24` / `80104148` CSE settings cannot simply be combined

The shared-state group at `notes/file-groupings.md:1721` supplies a useful
contrast:

- `80103B24`'s no-skip setting remains compatible with eight tested other
  members, including immediate neighbors `80103964` and `80103C00`.
- Applying that setting to `80104148`, while retaining its no-follow flag,
  changes the exact 147-word function to 152 words.
- Applying `80104148`'s no-follow setting to `80103B24`, retaining no-skip,
  changes 55 words to 49 and loses the match.
- No-follow also changes `80103964`, from 112 to 116 words.

**Assessment:** `80103B24` has a relatively coherent *local* regional case;
`80104148` has a real, selective flag effect but needs either a supported TU
boundary or a different reconstruction. Neither can establish the flag policy
of the entire shared-state group.

### E. Other boundaries that need evidence

- **`ovl_11_func_800F00E4`, no GCSE:** immediate predecessor `800F00AC`
  changes (14 words in both builds); successor `800F021C` does not. The
  distant `800F3D40` analogue is mechanism precedent, not proof of a shared TU.
- **`ovl_11_func_800F0E00`, no strength reduction:** immediate predecessor
  `800F0C70` changes from 100 to 117 words. The private-state relatives
  `800F0F50`, `800F0FB0` and `800F19E0` remain exact, so a boundary before
  `800F0E00` is a plausible hypothesis, not an established fact.
- **`func_800183E0` / `80018B98`, no GCSE:** the local pair, the intervening
  clear helper and tested nearby text helpers are compatible. But extending
  this to the broader loader/text family fails: `8001A018`, `8001A8D0` and
  `8001A970` change. Keep any historical claim local.

### F. Four pointer-walk initializers: real recurrence, not a unique fingerprint

`ovl_11_func_80113FF0`, `8011AA64`, `8011AC30`, and `8011ADF4` all need the
**pair** `-fno-cse-follow-jumps -fno-cse-skip-blocks` for the current C.
Neither individual flag suffices. Three are consecutive and share the same
record/helper machinery. This is substantially better evidence than four
unrelated one-off matches.

But the stronger historical claim remains unproved:

- Related `80113CD0`, `80114194`, `8011AFB4` and `8011B210` already match at
  baseline and remain exact with the pair.
- `80113CD0` and `8011AFB4` use the same explicit `q/a/b/c` pointer-walk
  construction in baseline-matching C. Consequently, pointer walks themselves
  do **not** uniquely diagnose disabled CSE path exploration. The surrounding
  control structure matters.
- Related `8011B2B0` is a contrary witness: the pair changes two store
  address forms in its otherwise 257-word output. At `0x8011B51C`, the store
  changes from symbol-displacement addressing to `0(s6)`; at `0x8011B580`,
  from `4(s6)` to `0(s2)`.

The compiler source supports a source-sensitive interpretation: `cse_main`
uses both flags to select exploration paths and whether to reprocess a block
after a jump changes (`gcc/cse.c:8792–8836`); `cse_basic_block` also tests
the pair around loop processing (`:9057–9066`). This is not simply an
on/off switch for straight-line pointer arithmetic.

**Assessment:** preserve the four as a provisional family hypothesis, but
review them together with the baseline-matching initializers and the B2B0
contradiction before calling their switches recovered historical facts.

## 3. Complete disposition of the inventory

“Provisional” means the flag is effective on current C and the reviewed
mechanism is plausible; it does **not** mean original build settings proved.
The word counts below are emitted instruction counts, **not matching scores
or an optimization-distance metric**. Every configured exception freshly
matches. A baseline with the same count can still have different instructions.

| Function | Baseline → target words | Disposition and reason |
|---|---:|---|
| `func_80014494` | 47 → 48 | **Strong provisional.** Specific shared-high/reformed-low asymmetry; all ten distinct pad-group witnesses tested remain exact with no-skip. One of the better cases to retain. |
| `func_800183E0` | 517 → 491 | **Strong local provisional.** Redundant arithmetic/PRE effect, adjacent independent no-GCSE case; do not extend to the entire loader/text family. |
| `func_80018B98` | 293 → 294 | **Provisional.** Current no-GCSE effect confirmed and locally corroborated by 183E0. Old “all C shapes” proof and one-function-TU argument are too strong. |
| `func_80019610` | 123 → 123, exact | **Stale permission.** No override to justify. |
| `func_80022794` | 116 → 116, exact | **Retire flag.** Pin/asm review remains separate. |
| `func_8002470C` | 25 → 25, nonexact | **Trim then reopen.** Only sched1-off is needed; related counter consumers contradict a shared scheduling-off setting. |
| `func_8002495C` | 25 → 25, nonexact | **Trim then reopen.** Same result and regional problem. |
| `ovl_11_func_800E4BA4` | 37 → 35 | **Provisional, source-sensitive.** Detailed first/second loop-pass threshold explanation; six distinct local witnesses remain exact. Their neutrality does not prove the second pass was off. Revisit loop-body/hoist premises before accepting a singleton flag. |
| `ovl_11_func_800E7660` | 76 → 78 | **Provisional.** Reformed address after a guard; both immediate neighbors compatible. Discard the one-function-per-file historical argument. |
| `ovl_11_func_800EF8BC` | 84 → 88 | **Provisional.** Repeated address formation and compatible immediate neighbors; F1678 is now a flag-neutral analogue, not an independent no-skip witness. |
| `ovl_11_func_800F00E4` | 79 → 78 | **Reopen regional claim.** Clear GCSE effect, but immediate predecessor changes. |
| `ovl_11_func_800F0E00` | 81 → 84 | **Reopen/establish boundary.** Index-versus-walker effect confirmed; predecessor contradicts the flag, private-state successors do not. FE068 precedent is stale. |
| `ovl_11_func_800F1678` | 51 → 51, exact | **Retire flag.** Current clean C disproves the blanket baseline-impossibility assertion. |
| `ovl_11_func_800F3D40` | 17 → 18 | **Provisional.** Independent loop-base formation is a concrete mechanism; both immediate neighbors compatible. Six searched source forms do not prove universal impossibility. |
| `ovl_11_func_800F8224` | 53 → 54 | **Provisional.** Indexed scan versus reduced walker; current comment correctly limits its claim to measured source. Both immediate neighbors compatible, but original TU still unproved. |
| `ovl_11_func_80103B24` | 49 → 55 | **Strong local provisional.** Guarded stores and compatible tight subcluster; broader grouping conflicts with 80104148's settings. |
| `ovl_11_func_80104148` | 133 → 147 | **Reopen regional claim.** Selective no-follow effect is real; no-follow changes related 80103964 and conflicts with 80103B24. |
| `ovl_11_func_80104EC8` | 1793 → 1982 | **Strong provisional mechanism, weak TU provenance.** Extensive repeated guard/address effects, explicit rejection of disabling both CSE paths; two small immediate neighbors neutral. Still not proof of an original command line. |
| `ovl_11_func_80113FF0` | 88 → 101 | **Provisional family; review jointly.** Pair necessary for current C, but pointer-walk fingerprint is not unique to it. |
| `ovl_11_func_8011AA64` | 98 → 115 | **Provisional family; review jointly.** Consecutive shared-record siblings strengthen recurrence, not universal source exclusion. |
| `ovl_11_func_8011AC30` | 96 → 113 | **Provisional family; review jointly.** Both individual flag columns fail. |
| `ovl_11_func_8011ADF4` | 95 → 112 | **Provisional family; review jointly.** Baseline-matching AFB4 is neutral; B2B0 is contrary. |
| `ovl_15_func_801335F8` | 132 → 132, nonexact | **High-priority reconstruction review.** Machine-shaped coordinate loop; related GCSE-on witnesses. |
| `ovl_17_func_800B9158` | 24 → 24, nonexact | **Weak historical case.** Only one `addu` operand order changes. Two immediate neighbors compatible, but bounded spellings do not exclude a different address/web construction. |
| `ovl_21_func_800B98CC` | 18 → 16 | **High-priority reconstruction review.** Store/return coalescing effect; caller and immediate successor contradict sched1-off. |
| `ovl_21_func_800BA698` | 86 → 86, nonexact | **High-priority reconstruction review.** Allocation-sensitive exception with contrary matched scan/caller witnesses. |
| `ovl_25_func_800B95B0` | 105 → 106 | **Provisional.** Retained selector assignment/branch sense has a concrete CSE-following explanation; both immediate neighbors compatible. Natural control-flow alternatives remain a source-side question. |

## 4. Why the evidence trail overstates historical certainty

### Current repository TUs are not original TUs

Many comments conclude “no contrary regional witness” because each current
source file is its own TU. That only says the override is isolated **in our
build**. It provides no information about original compilation units.
Examples include `80018B98`, `8002470C`, `ovl_21:800BA698`,
`ovl_11:800F0E00`, `800E7660`, `800EF8BC`, and the initializer family.

### A successful flag column proves sufficiency for one source

A missing optimization can result from source types, aliasing, control-flow
structure, multiple assignments, declaration/ownership facts or compiler
heuristics. A bounded search exhausting 6, 21 or even 184,320 candidates only
exhausts its specified grammar and context. It does not prove “unreachable
from any C.” The surviving exact baseline `800F1678` is a concrete warning
against elevating that wording into a historical fact.

Likewise, a neighbor that matches under both settings is **neutral**, not an
independent affirmative witness for the unusual setting. A same-overlay flag
elsewhere is mechanism precedent, not a shared build command.

### Several cited precedents have already been withdrawn

- The configuration's introductory historical proof cites the former
  `-mno-split-addresses` exceptions on `80016C08`/`800165D8`. The same file
  explains their withdrawal: correct scalar declarations/TU ownership match
  at baseline. They cannot still serve as proof of original per-file flags.
- The `800F0E00` comment calls `800FE068` a current no-strength-reduce
  precedent, although the latter's flag was removed. Its existing audit
  explicitly rejects that grouping inference.
- Comments cite an `8011FF74` no-scheduling exception that is absent from
  the standing override inventory.
- The counter rationale says useful siblings are not matched yet; they are.
- `800F1678` is cited by other entries as affirmative no-skip evidence;
  the present source is flag-invariant.
- The file still contains an old “no per-file compiler flag overrides”
  statement and stale draft scores. These should be dated or replaced, not
  read as the current state.

The flag reference sheet repeats some of the withdrawn split-address
reasoning. Its general evidence gate is useful, but its examples should not
be treated as independent corroboration of the configuration it helped create.

### Historical possibility is not historical attribution

The vendored `CCPSX.TXT` documents `-O0`, `-O1`, `-O2` and other optimization
switches. Thus differentiated settings are technically and historically
plausible. That document does not identify this game's settings. The reviewed
notes identify the compiler/toolchain but do not supply the game's original
per-file build recipes or independently established boundaries for these
exceptions. An equivalent effective compiler state also need not imply the
exact command-line spelling used by this reconstruction.

## 5. Recommended next steps before closing the quality review

The investigations below will be carried out alongside translation-unit
consolidation using the agreed workflow in section 6, rather than treating
per-function flag explanations as settled history. The no-source-change
cleanup remains a separate work item.

1. **Apply the no-source-change cleanup separately.** Remove the two neutral
   overrides, trim sched2-off from both counters, and remove the three now-
   unnecessary flag permissions (including the pre-existing orphan).
   Re-run the individual byte oracles and `make -j8 check-all`; retain
   `80022794`'s other permissions until the separate asm review settles them.
2. **Reopen five high-priority functions:** the two counters,
   `ovl_21:800B98CC`, `ovl_21:800BA698`, and `ovl_15:801335F8`.
   Start from the named compiler decision and the now-matched family/data
   representations, not another unconstrained spelling search. Audit
   `ovl_17:800B9158` alongside this work as the small operand-order-only case.
3. **Test the remaining historical hypotheses by group.** Resolve the
   `80103B24`/`80104148` boundary question; review all four initializer
   exceptions with their baseline siblings; examine F00E4/F0E00 boundaries.
   Do not infer a new TU boundary solely to protect an exception.
4. **Rewrite evidence claims to their actual scope.** Record the current
   source/context hash, exact baseline/flag/subset verdicts, original-byte
   mechanism, tested neighbors and contradictions. Separate “necessary for
   this reconstruction” from “plausible original setting” and “documented
   original setting.” Remove universal source-impossibility language unless
   its premises and genuinely exhaustive domain are specified.
5. **Keep completion and provenance as separate claims.** The project can
   accurately report a byte-identical compiled reconstruction while retaining
   an explicit list of unresolved source/flag provenance. The present evidence
   does not justify saying all standing exceptions faithfully reproduce the
   original build. It also does not justify saying every remaining exception
   is a crutch.

## 6. Agreed plan: investigate flags while reconstructing translation units

**Status: planned, not implemented.** Treat the remaining overrides as open
TU-level hypotheses. Resolving them, or explicitly documenting why they remain
unresolved, is part of each affected TU consolidation's acceptance criteria.
This plan does not authorize speculative flag changes or weaken the existing
source-policy and verification gates.

### Historical interpretation

A current one-function override need not imply the original developers
customized that particular function. An entire original module might have used
a different setting while only one member exercises the affected optimization.
Alternatively, the switch may reproduce a different optimization profile,
compiler revision or source/TU context, or compensate for our reconstruction.

For example, `func_80014494` has a well-explained compiler mechanism and ten
compatible pad-group witnesses, but those unchanged witnesses are neutral:
they do not establish an original no-skip setting. Its appropriate status is
**mechanistically well-supported exception; historical origin unresolved**.
Applying the flag to every pad function would make the configuration look more
historical without supplying the missing evidence.

### Workflow for each proposed TU

1. **Establish membership independently of flags.** Use shared private data,
   internal calls, linkage evidence and address ordering. Consult and update
   the existing relevant section of `notes/file-groupings.md`. Do not create
   or move a boundary solely to protect an exception.
2. **Preserve the matching split-file build as a control.** Record sources,
   declaration context, toolchain identity and exact per-member results under
   baseline and relevant flag settings. Distinguish sensitive members from
   those that match under either setting.
3. **Stage the merged TU and test baseline first.** Reconcile declarations,
   prototypes and global ownership from evidence; these can themselves change
   code generation and explain an apparent flag requirement. Keep the live
   matching build intact while investigating a nonmatching merge.
4. **Measure a bounded TU-wide flag matrix.** Test the existing hypotheses
   and necessary subsets consistently across all members. Compare merged
   baseline with merged flagged builds, keeping source/context fixed, so a
   flag effect is not confused with a declaration or consolidation change.
   A proposed historical setting must account for the whole proposed TU.
5. **Classify the result and retain the evidence.** Record exact byte verdicts
   and any residual's owning compiler mechanism, not just a match percentage.
   Never promote a mismatching merge to preserve an experiment.

| Outcome | Action and interpretation |
|---|---|
| The override becomes unnecessary | Retire it and its unused flag permission after full verification. |
| One setting matches every member | Record a coherent TU-wide historical hypothesis, not proof of the original command line; retain the usual flag-evidence requirements. |
| Members require conflicting settings | Revisit reconstructed source/context and independently supported TU boundaries. Do not split the group merely to accommodate the flags. |
| Both settings match every member | Record that the bytes do not distinguish them; prefer baseline unless independent evidence supports the exception. |
| No tested setting matches the whole TU | Keep consolidation unresolved and the existing matching build intact; identify the next bounded source/context investigation. |

A conflict is evidence against the **combination of proposed grouping,
reconstructed source/context and compiler settings**. It is not automatic
proof that the grouping is wrong. Likewise, a successful TU-wide setting is
stronger consistency evidence than a one-function match, but not historical
attribution by itself.

### Initial pilots

1. **Pad/controller group containing `func_80014494`.** Start with the
   existing candidate group around `0x80013B04–0x80014554`: coherent grouping
   evidence and no measured regional contradiction to no-skip. Determine
   whether the requirement survives a faithful merged context.
2. **Counter/sprite group containing `func_8002470C` and `func_8002495C`.**
   Use the existing candidate group around `0x800245F4–0x80024AD4`. Related
   counter consumers currently contradict even the minimal sched1-off setting,
   making this a useful test of reconstruction versus TU-boundary assumptions.
   The sched2-off cleanup is already independently supported and need not wait
   for this investigation.

### Acceptance record for each affected consolidation

- Proposed membership and boundary evidence independent of flag outcomes.
- Reproducible split/merged baseline and relevant flag/subset measurements,
  with source/context/toolchain identities and neutral/contrary witnesses.
- Disposition of each exception: retired, provisional TU-wide hypothesis or
  explicitly unresolved, with the next bounded question where necessary.
- Updated grouping/evidence notes and any justified configuration/allowlist
  changes; no unsupported historical certainty or new source-policy workaround.
- Exact per-function byte checks, clean-source policy checks and
  `make -j8 check-all` passing for the integrated result, including placement
  and data/rodata effects of consolidation.

These pilots are investigations, not a commitment to particular original TU
boundaries or a promise that every remaining override can be removed.
