# Project-wide symbol canonicalization

Status: planned; implementation has not started. This document records the
execution plan, not a completed migration or authorization to commit.

## Goal

Carry the project's existing function-name discoveries through every caller,
including overlays. A function renamed during refinement should have one
canonical identity and interface throughout the reconstructed project, rather
than retaining address-based aliases in other containers.

Do this before translation-unit consolidation. The latter should organize
known functions, not rediscover that several apparent functions are the same
callee. This is a bounded static-analysis and rewrite pass, not a new naming
campaign, TU inference engine or persistent research queue.

Separate two changes:

1. **Name canonicalization:** change symbol spellings without changing C types,
   expressions, control flow, compiler flags or target addresses.
2. **Interface reconciliation:** independently audit and repair conflicting
   declarations, preserving target behavior and exact bytes.

A successful rename must not silently claim that its inherited prototype is
correct. Conversely, a prototype disagreement must not hide the fact that two
names refer to the same function.

## Observed motivating defect: Rand

Current evidence:

- `configs/symbols/exe.txt` identifies `Rand` at `0x80012A34`.
- `src/Rand.c` defines `u32 Rand(s32 arg0)`.
- Numerous overlay sources call `func_80012A34`, sometimes declaring an `s32`
  return and sometimes the definition's `u32` return.
- The overlay's generated undefined-function assignments resolve that anonymous
  name to `0x80012A34`, so those calls can match despite the naming discrepancy.
- The linked executable has both `Rand` and the repeated compiler marker
  `__gnu_compiled_c` at that address.
- `tools/build/exportEngineSymbols.ts` selects one name per address before
  rejecting names repeated at multiple addresses. Its current comparison can
  select `__gnu_compiled_c` instead of `Rand`, then discard the marker without
  restoring the real function. `Rand` is absent from the inspected
  `build/engine_syms.txt`.

This is a general export/identity problem, not a reason to special-case Rand.
The earlier overlay enablement plan already intended executable renames to
propagate into overlays; this work repairs and completes that contract.

## Scope and non-goals

### In scope

- Existing, independently identified **function** renames across the executable
  and every configured overlay, including applicable SDK function names.
- Direct calls, declarations, definitions where necessary, function-address
  expressions, callback initializers and supported symbol-bearing assembly.
- Source configuration, generated disassembly/linker inputs, function context,
  and diagnostic consumers that must agree on identity.
- A report of conflicting signatures, followed by a separately verified
  interface-reconciliation phase.
- A check preventing obsolete aliases from returning to active source or newly
  generated preparation output.

### Out of scope

- Inventing new semantic names for still-unidentified functions.
- Renaming data globals, struct members, locals or jump labels. Legitimate
  data views can share addresses without being interchangeable symbols.
- Merging TUs, changing linkage/visibility, changing ownership of globals,
  adding optimization flags, or changing function behavior to obtain a match.
- Rewriting historical research notes, old experiment artifacts or quoted
  assembly merely to remove an old spelling.
- Automatically committing, resetting unrelated work or modifying vendor code.

## Identity and evidence rules

### Container-qualified identity

Use `(defining container, exact function-entry address)` as the anchor, with
symbol kind and definition evidence attached. The referencing container is
separate: an overlay can import an executable function or a function in a
supported cross-slot overlay.

Do not use a global address-only map. Different overlay members reuse RAM
addresses. Do not infer a callee from the nearest preceding symbol, a shared
suffix, textual similarity or identical instruction bytes. Interior-entry
references and ambiguous overlay residency remain unresolved unless separately
established.

### Canonical name selection

Derive canonical identities from the existing curated symbol/configuration
facts and actual definitions, corroborated by linked symbols and original
call/relocation targets where applicable. Prefer an established project name
such as `Rand` over an anonymous `func_XXXXXXXX` only after identity agrees.

Filter compiler markers and section geometry before choosing public names.
If two meaningful names compete, surface the conflict instead of choosing one
alphabetically. Generated disassembly, linker assignments and
`include/functions.h` are downstream outputs, not independent witnesses that
can overrule their source inputs. The generated function header is especially
not evidence for a disputed signature.

Preserve genuine exported aliases where they have an independently supported
purpose. Do not confuse a valid public alias with an obsolete placeholder.

### Rewrite binding, not text

Use the existing pinned C parser and scope-aware binding checks. A matching
identifier spelling could be a local, struct member, macro or unrelated symbol.
Only rewrite occurrences proven to refer to the selected function identity.
Preprocessing must confirm that the rewritten source binds to the intended
callee under the production configuration.

Macro expansion, token pasting, assembler names and inline-assembly strings
require explicit handling; an AST identifier rewrite does not cover them.
Unsupported occurrences must be reported and left intact, never silently
omitted from the completion count.

## Implementation shape

Keep this small:

- Extend/reuse `tools/lib/symbolIndex.ts` and the existing container model for
  shared identity resolution. Do not build a second independent symbol system.
- Factor the engine export's filtering/selection into testable logic.
- Add one TypeScript diagnostic/rewrite CLI under `tools/diagnostics/`, with
  dry-run by default, a scoped write mode and a read-only check mode. A
  single-symbol scope supports the Rand pilot; the full census discovers all
  eligible existing renames automatically.
- Put generated reports and staged candidates under `build/`. Derive them
  afresh from current inputs; no manually maintained alias database or work
  queue is required. Any genuinely ambiguous canonical-name decision belongs
  in the established owning configuration with evidence.
- Use source/target/configuration hashes to reject stale applications and to
  identify what was measured. These are transaction safety, not a new research
  framework.

The report should list definition identity, canonical name, known aliases,
occurrence locations, evidence, prototype disagreements, proposed edits and
unresolved cases. Name and signature readiness are separate fields.

## Execution phases

### Phase 0 — establish the current baseline and census

1. Read `AGENTS.md`, `configs/project-profile.md`, the active Make dependencies,
   symbol exporters and overlay generation path before implementation.
2. Record the working-tree state and preserve unrelated edits. Establish a
   passing `make -j8 check-all` baseline; if it fails, diagnose that failure
   before attributing anything to this migration.
3. Enumerate configured compiled functions and their definitions, not every
   `.c` file left on disk. Separately classify inactive drafts and archives.
4. Build the read-only canonical/alias census over all configured containers.
   Include current C declarations, assembly/relocation references and relevant
   generated names. Record ambiguous, unsupported and conflicting cases.
5. Identify all prior semantic renames whose anonymous aliases remain active.
   Rand is the pilot, not the complete migration list.

Acceptance: a reproducible inventory with explicit coverage and no live edits.

### Phase 1 — repair export and canonical-name propagation

1. Fix the compiler-marker selection defect generically. Reject markers before
   per-address selection; ensure dropping a non-interface symbol cannot erase
   a real definition at the same address. Preserve valid data exports and the
   existing overlay-slot exclusions.
2. Keep linked-ELF authority and the explicitly reported pre-link fallback.
   Fallback or unavailable artifacts must not be silently presented as equally
   strong evidence for automatic rewriting.
3. Regenerate the engine export and confirm `Rand` is exported at `0x80012A34`.
   Check other known refined names rather than accepting a one-case fix.
4. Ensure overlay disassembly/splitting receives canonical engine names and
   proven cross-container names. Audit the current file-precedence behavior in
   `symbolIndex.ts`: a stale generated assembly label must not silently undo an
   independently established canonical function name.
5. Preserve the ability to resolve legacy aliases during migration, without
   making them preferred names in fresh output. Splat requires one name per
   address: do not solve compatibility by feeding it duplicate-address names.
   If a temporary linker compatibility layer is necessary, generate it from
   the verified map, separate it from canonical naming, and remove it once its
   active references are migrated.
6. Test the generation/build ordering. Regeneration must not strand still-live
   anonymous references halfway through the migration.

Acceptance: known names survive export and fresh overlay generation; existing
builds remain valid or the dependent migration is staged as one verified batch.

### Phase 2 — stage the rename-only migration, beginning with Rand

1. Locate every supported active reference to the pilot identity.
2. Rewrite only names and name-dependent metadata. Keep existing declaration
   types unchanged in this phase, even when they are flagged for later repair.
   If canonical naming creates a declaration collision, park that occurrence
   for an explicit joint name/signature change rather than forcing it through.
3. Update the owning hand-maintained inputs and regenerate derived outputs.
   Never hand-edit generated headers, linker scripts or undefined-symbol lists.
   Preserve established function/rodata attribution when regenerating configs.
4. Include callbacks and address-taken uses, not just `jal` call sites. Treat
   supported inline/top-level assembly references as symbol-only edits, with
   existing policy exceptions unchanged. Do not add assembly or register pins.
5. Verify affected functions with the relocated-byte oracle and check linked
   data/relocations with `make -j8 check-all`.
6. Apply only the verified edit set, checking input hashes first. On failure,
   discard the staged candidate or reverse only edits owned by this operation;
   never use a broad Git reset or overwrite intervening user work.

Acceptance: migrated callers use `Rand`, still reach the same original entry,
retain their pre-migration types and reproduce exact bytes. Signature debt is
reported, not concealed by the successful name change.

### Phase 3 — extend to all existing function renames

1. Run the same rewrite over the complete eligible census in bounded batches,
   ordered by dependency and change scope rather than match percentages.
2. Use one identity resolver for export, original-call naming, preparation,
   callee lookup, context export and relevant diagnostic/call-graph consumers.
   Audit these consumers rather than assuming an exporter fix reaches them:
   `requireFunctionLocation` currently locates an exact name, and historical
   preparation has failed with “No container defines func_80012A34”.
3. Keep ambiguity explicit. Report proposed versus applied versus blocked
   identities and occurrences. No item disappears from the denominator because
   a parser or resolver cannot handle it.
4. Re-run the census after each integrated batch. A second write pass over an
   unchanged migrated scope must produce no edits.

Acceptance: every resolvable obsolete alias in active scope has been migrated;
remaining aliases have explicit reasons. Newly generated output uses canonical
names and does not recreate the cleanup workload.

### Phase 4 — reconcile interfaces separately

1. Audit each canonical function's declarations against its matched definition,
   the relevant SDK declaration where applicable, and original ABI/caller
   evidence. Reuse the existing callee-truth machinery; do not treat generated
   declarations as independent confirmation.
2. Resolve return types, parameter widths/signedness, arity and pointer types
   explicitly. Do not cast blindly to preserve the effect of a wrong prototype.
3. For Rand, establish the shared `u32 Rand(s32)` contract. Where a target caller
   performs signed arithmetic/comparison, recover the corresponding signed
   local or explicit cast from evidence and verify that caller independently.
4. Publish validated interfaces through an appropriate hand-maintained public
   header under `include/`, replacing duplicate caller-local declarations.
   Do not include the generated m2c function-context header wholesale as a
   shortcut. Leave global declarations and shared types in their established
   homes per `configs/project-profile.md`.
5. Regenerate function context through its generator and re-check affected
   callers and the complete build. An unresolved type change stays a named
   follow-up; it does not invalidate a separately verified identity repair.

Acceptance: completed interfaces have one corroborated contract; incomplete
ones are explicitly counted, with no false “all prototypes fixed” claim.

### Phase 5 — prevent recurrence and support future refinement

1. Add a read-only active-source/generated-output check for obsolete aliases
   whose canonical identity is unambiguous. Historical notes, archived runs,
   legitimate aliases and still-unnamed functions are not violations.
2. Ensure preparation and context generation request canonical names and
   resolve legacy diagnostic input to the correct container-qualified identity.
3. Support a future refinement rename as one coordinated operation: update the
   canonical definition/configuration, propagate references, regenerate derived
   inputs and verify bytes. Declaration changes remain a separate decision.
4. Wire the check into an appropriate existing validation path, with a focused
   command for iteration. Document failure messages and exceptions clearly;
   uncertain resolution is not a license for an automatic rename.

Acceptance: an intentionally reintroduced obsolete active alias is detected,
while regeneration and a subsequent clean run produce no drift.

## Required tests

### Identity and exporter tests

- Rand and `__gnu_compiled_c` share an entry: Rand remains exported after all
  repeated markers are excluded. Also cover section markers and alias order.
- Two meaningful names at one entry require explicit canonical evidence;
  input order and lexical ordering cannot silently decide historical identity.
- Identical RAM addresses in two overlays remain different identities.
- An overlay's executable import resolves to the executable definition.
- A proven cross-slot import resolves correctly; unresolved residency stays
  ambiguous. Interior labels and `function + offset` references are not
  rewritten as entry aliases without proof.
- Data exports, slot exclusion, missing-ELF fallback and stale-export checks
  retain their existing behavior.

### Rewrite tests

- Calls, prototypes, definitions, function pointers and callback tables migrate.
- Shadowed locals, member names, strings, unrelated identifiers and historical
  comments do not get rewritten by a textual substitution.
- Macro-generated references, conditional branches and assembly references are
  either handled with verified binding or explicitly rejected/reported.
- Name-only rewriting preserves types, casts, linkage, flags and global
  definitions. Conflicting signed/unsigned prototypes are reported separately.
- Re-running is idempotent; source drift invalidates a prepared edit set;
  unrelated dirty files are preserved; failures leave no partial live rewrite.

### End-to-end tests

- Rand pilot: canonical export, canonical overlay call, same original call
  target and exact compiled/linked bytes.
- Another previously refined executable function: no Rand-specific logic.
- Overlay identity collision negative control and an available verified
  cross-container reference.
- Preparation/callee lookup no longer fails on a proven legacy alias, and new
  output uses the canonical spelling.
- Signature repair with a target-required signed comparison is tested
  separately from the rename-only result.
- Warm regeneration plus a fresh generated-artifact build in an isolated
  workspace agree. Do not clean or wipe the user's working tree for this test.

## Verification and completion

While implementing, run the focused TypeScript tests and per-function byte
oracles for each batch. Before reporting completion:

1. Run the applicable project test suite and source-policy/scope checks.
2. Regenerate affected artifacts through the owning tools, then verify that a
   repeat generation is stable. Check cold-generation behavior in isolation.
3. Run `make -j8 check-all`: the executable and all configured overlays must
   match, including jump tables, callback pointers and linked placement.
4. Run the obsolete-alias check and publish the final coverage report: renamed
   identities/references, remaining legitimate aliases, unresolved identities,
   unsupported occurrences and separately outstanding signature repairs.
5. Confirm no unrequested flag, assembly, ownership or TU changes, and no
   generated/extracted binaries staged for commit. Do not commit unless asked.

Name migration and interface reconciliation have separate completion states.
Do not report complete project-wide reconciliation while either scope still
contains unclassified omissions. The minimal first milestone is the general
exporter fix plus a fully verified rename-only Rand pilot; the final goal is
propagation of **all existing resolvable function renames**, not just that pilot.

## Relevant existing infrastructure

- `tools/build/exportEngineSymbols.ts`: engine exports and name selection.
- `tools/build/bootstrapOverlay.ts` and `Makefile`: cross-container naming,
  split/link dependencies and generation order.
- `tools/lib/symbolIndex.ts`: container-scoped symbol/definition lookup.
- `tools/agent/cSourceGuard.ts` and the existing C parser: safe source handling.
- `tools/agent/calleeTruth.ts`, `tools/agent/contextExport.ts` and
  `tools/agent/type-propagation/`: interface evidence and preparation consumers.
- `tools/lib/functionOracle.ts`: relocated-byte verification.
- `plans/overlay-decompilation-enablement.md`: existing propagation contract and
  warm/cold generation lessons.
- `plans/flag-review-report.md`, section 6: subsequent TU consolidation work;
  this plan is its naming/interface prerequisite, not an extension of the flag
  experiment scope.
