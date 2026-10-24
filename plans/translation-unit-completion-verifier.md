# Translation-unit reconstruction completion verifier

## Status

Implemented 2026-10-24. This plan defines the structural completion heuristic
and integrity gate for repeatedly applying [`plans/tu-prompt.md`](tu-prompt.md)
until every eligible game function is owned by an explicit reconstructed source
module.

What landed, by phase:

- **0/1 — inventory and TU index.** `tools/lib/symbolIndex.ts`
  (`loadOriginalFunctionInventory`, from `functions.csv` + `type:func` symbols;
  `loadFunctionSpans` is now its compatibility view), `tools/lib/translationUnits.ts`
  and `tools/lib/translationUnitAudit.ts` (AST source view, in-process ELF object
  view, nested-function carriers and asm-label renames). Every current unit audits
  valid. The one identity change: the entry point is now named by its symbol
  `func_80011278`; its unit stem stays `__start`.
- **2 — scratch pilot.** splat 0.38.0 carries `modules/<name>` stems through the
  generated linker script; a redirected ovl_31 split with a two-function module
  linked byte-identically and both members matched through the oracle. The
  `modules/` convention was adopted; no `mod_` fallback was needed.
- **3 — build and generators.** One `ConfiguredCSources` listing for the
  executable and every overlay; `FlagKey` (path key for modules) and `StubStem`
  in the Makefile; `moduleCSegments` in `bootstrap.ts` / `bootstrapOverlay.ts`;
  module-safe `mergeFragments.ts` / `fixCrossFileRefs.ts`; TU-scoped
  `deriveRodataSplits.ts`; `tools/build/foldTranslationUnit.ts`.
- **4 — consumers.** progress, call graph, toolchain resolver, oracle, diffFunc,
  context export, TU-owned globals, finalization/integration/preparation guards,
  idiom/family/campaign readers and the Pi gates route through the TU index.
- **5 — policy.** `moduleConstructFindings` charges each construct to the
  enclosing member; module scope borrows nothing; a module flag override is never
  allowlisted.
- **6 — verifier.** `tools/diagnostics/tuProgress.ts` (0/1/2, JSON schema 1,
  `--verify`, `--baseline`, `--write-baseline`), `tools/diagnostics/tuExceptions.ts`,
  `make tu-progress|tu-complete|tu-verify`; `/auto_decompilation_loop fold`.
- **7 — pilot.** `src/overlays/ovl_31/modules/memcard_service.c` holds five of
  ovl_31's six functions; folded 0 → 5, remaining 2623 → 2618, no exception
  growth. The driver `ovl_31_func_800B7FCC` is blocked by the original `.rodata`
  interleaving its jump table with the helpers' string literals (see the ovl_31
  section of `notes/file-groupings.md`).

Known limits: the EXE side of a module is proven by compile + per-member oracle
(`tools/build/moduleFold.integration.test.ts`) and the shared splat path, not by a
scratch EXE link; `make tu-*` cannot carry the three-way exit status through make,
so an orchestrator calls the tool directly.

## Problem

The project is already complete according to its matching-decompilation metric:
`make complete` exits successfully and `make check-all` verifies the executable
and every configured overlay. Its source organization is not complete. Most C
translation units are still one address-derived source file per function, so
the current file layout says little about the original modules, their private
state or their interfaces.

An orchestrator should be able to dispatch `plans/tu-prompt.md` for one module
at a time and stop only when a deterministic heuristic says no function remains
to fold. The same gate must prove after every accepted fold that the project is
still complete in the existing sense: no function went back to a stub, no
exception was introduced, function ownership remains coherent, and every linked
container is byte-identical.

The difficulty is that current tooling often treats function identity and
translation-unit identity as the same thing:

- `src/<function>.c` is assumed to be the function's source;
- `<function>.c.o` is assumed to be its object;
- a splat `c` subsegment is treated as one function extent;
- progress and call-graph code inspect a same-named source file;
- rodata attribution is made per function object;
- source-policy exceptions are inferred from a source filename.

Every one of those assumptions becomes false when one C file defines two
original functions. A completion script added before that substrate is fixed
would report the first successful module as missing source or missing
functions.

## Goals

1. Separate original function identity from reconstructed TU identity.
2. Define a deterministic, non-manifest-based meaning of "folded into a
   module."
3. Report module-folding progress and a bounded list of remaining work.
4. Expose an exit status an orchestrator can use without interpreting prose.
5. Preserve the existing decompilation-complete and all-container byte-identity
   guarantees after every accepted module.
6. Detect duplicate ownership, missing symbols, stale per-function files,
   orphan source, wrong function order and source/config/object disagreement.
7. Prevent a module fold from adding asm, register pins, stubs or compiler-flag
   exceptions.
8. Keep `notes/file-groupings.md` as evidence for human/agent reconstruction,
   not as a manually maintained completion queue.

## Non-goals

- Proving that a reconstructed boundary is the historical original boundary.
  The binary does not contain enough information to prove that universally.
- Scoring prose quality, gameplay terminology or whether every rename is the
  best possible name.
- Treating adjacency by itself as same-TU evidence.
- Replacing `make complete`, the relocated function oracle, source policy or
  `make check-all`.
- Creating a committed module manifest, discovery queue or list that an agent
  can satisfy by editing the list instead of improving source organization.
- Inferring `static` linkage merely because two functions now share a C file.
- Automatically merging functions selected only by a heuristic.

## Honest scope of the heuristic

The terminal sentence should be read as:

> Every eligible game function that is currently implemented in C is owned by
> exactly one explicit reconstructed module TU, and the complete project still
> passes all matching and source-integrity gates.

Eligible functions include live and dead game functions that have compiled C,
including C functions with existing authorized source-policy exceptions. The
following are reported but excluded from the folding denominator:

- independently classified handwritten-assembly functions;
- SDK/library object members;
- unresolved dead-code `INCLUDE_ASM` stubs that the existing `make complete`
  definition deliberately excludes;
- containers whose extraction or load base remains unresolved.

Exclusions must be visible in text and JSON output. An empty or partly
configured project must never pass.

## Source convention: make module ownership visible

Use an explicit directory convention rather than guessing intent from a
filename:

```text
src/modules/<semantic-module>.c
src/overlays/<container>/modules/<semantic-module>.c
```

A module's splat `c` subsegment uses the corresponding relative stem, for
example `modules/cd_loader`. Its object is therefore
`build/src/modules/cd_loader.c.o` or the equivalent overlay path.

This convention is the source-tree record of progress. It avoids a second
tracking file and gives the verifier an unambiguous distinction between:

- an unfolded per-function TU at the container source root; and
- a deliberately reconstructed module under `modules/`.

Module stems must be semantic identifiers. Reject address-derived names such
as `func_80012345`, `ovl_11_func_800ABCDE`, `D_80012345`, or a cosmetic
`module_80012345`. Do not require every function symbol to be renamed; uncertain
function semantics may remain address-named inside a meaningfully bounded and
named module.

A one-function module is valid. Historical projects can contain genuine
singleton TUs, and requiring two functions would force an unsupported merge.
For a singleton, the semantic module path is the mechanical evidence that it
was reviewed and classified. Whether its name and documentation are useful is
a review/prompt question, not something the verifier can prove.

Before adopting the directory convention, run a small scratch pilot proving
that the configured splat version, generated linker scripts and every relevant
tool preserve a `c` segment whose stem contains `modules/`. If splat cannot
carry nested stems, use a reserved semantic stem prefix such as `mod_<name>` at
the container source root and keep the rest of this contract unchanged. Do not
silently fall back per file.

## Core model: original functions and reconstructed TUs are different tables

Add a shared module such as `tools/lib/translationUnits.ts` with two primary
identities:

```ts
interface OriginalFunction {
  container: string;
  vram: number;
  rom: number;
  size: number;
  name: string;
  classification: "game-c" | "pending-stub" | "handwritten-asm" | "library";
}

interface TranslationUnit {
  container: string;
  segmentName: string;
  sourcePath: string;
  objectPath: string;
  startVram: number;
  endVram: number;
  members: OriginalFunction[];
  foldedModule: boolean;
}
```

### Original-function inventory

Derive original functions independently of C subsegments:

1. Read container-qualified `type:func` symbol identities and addresses.
2. Use the original disassembler/function-boundary artifacts to establish
   extents.
3. Keep container in every key; overlay RAM addresses are not globally unique.
4. Reject conflicting names, starts, extents or uncovered original text as
   undetermined rather than selecting one reading.

Do not continue using `loadFunctionSpans()` as "the list of `c` subsegments."
Split the API into names that state what they return, for example:

- `loadOriginalFunctionSpans(container)`;
- `loadTranslationUnits(container)`;
- `translationUnitForFunction(container, vram)`.

Compatibility wrappers may remain temporarily, but new code must not call a
subsegment a function.

### TU inventory

Derive reconstructed TUs from configured `c` subsegments. The TU begins at its
subsegment and ends at the next configured subsegment boundary. Its members are
the original functions whose starts fall in that interval.

Validate the same relation through two independent views:

- **source view:** the pinned C AST finds the expected top-level and nested
  function definitions in the source file;
- **object view:** the built object symbol table identifies the emitted `.text`
  symbols and their offsets.

A valid TU has one container, one configured source, one object, at least one
eligible member, no duplicate expected definition and no unexplained function
symbol. Compiler-generated local labels are not game-function members.

### Membership invariants

For each module:

- members are in ascending original address order in source/object emission;
- every member is wholly inside the TU's configured text interval;
- no member belongs to a second TU;
- no intervening non-member game function, assembly segment, library object or
  loaded data range is swallowed silently;
- the source/object emits every declared member exactly once;
- no legacy per-function source remains active for a folded member;
- no module source exists without a configured TU;
- module and members use one container's baseline flags;
- all pre-existing per-file flag hypotheses are reconciled before merging.

A flag conflict is not permission to split a likely TU or add another override.
It is an explicit blocker requiring the investigation in
`plans/flag-review-report.md`.

## Build-system support required before measuring progress

### Enumerate configured sources, not filesystem guesses

The executable currently uses a top-level wildcard, while overlays parse their
configured `c` entries. Replace both with one shared configured-source listing
that supports nested module stems and returns paths in link order.

This prevents:

- a nested module source being omitted;
- an obsolete per-function source being linked in addition to its module;
- an unrelated review copy under `src/` becoming a build input;
- executable and overlay rules disagreeing about what a TU is.

The listing must be deterministic and covered by tests for nested paths,
overlays and missing sources.

### Preserve modules through generation

`split-all`, overlay bootstrapping and config repair must not expand a module
back into one C segment per function. The source tree and AST-defined membership
are the record from which a generated config preserves/reconstructs a folded
module.

Add a bounded explicit helper if needed, for example:

```text
npx tsx tools/build/foldTranslationUnit.ts \
  --container exe \
  --module cd_loader \
  --members FuncA,FuncB,FuncC \
  --write
```

The helper is not a grouping oracle. It performs only checked mechanical work:

- members must be container-qualified, contiguous and already implemented;
- source order must agree with original address order;
- no non-member segment may be crossed;
- effective flags must be compatible;
- a dry run prints the exact config/source consequences;
- `--write` updates through the supported config path;
- it creates no queue or persistent module claim beyond source/config state.

### Object and oracle lookup

A function oracle request still names a function, but compilation and object
lookup go through its TU:

```text
function identity -> TU source -> TU object -> function symbol within object
```

`functionOracle.locateFunctionSymbol()` already supports an object whose file
stem differs from its function symbol. Preserve and test that behavior with two
functions in one object. Compute the `.text` section base from the module start
and symbol offset, not by assuming the function starts at object offset zero.

### Rodata and other owned sections

A module object has one pooled `.rodata`/`.rdata` section. Refactor
`deriveRodataSplits.ts` and `loadFunctionDataSubsegments()` so section ownership
is TU-scoped:

- one `.rodata, modules/<name>` attribution per TU object;
- any member's relocation resolves through that TU section base;
- the original module's complete rodata extent must agree with the object;
- ambiguous table ownership is undetermined, not assigned to the first member.

Tentative data/sdata definitions likewise become TU facts. Folding must dedupe
repeated tentative definitions and retain every independently established
owning-global definition required for GP-relative addressing.

## Consumers that must become TU-aware

At minimum, migrate these before accepting the first production module:

- `tools/diagnostics/progress.ts` and `make complete`;
- `tools/agent/callGraph.ts` and the shared call-graph source path;
- `tools/agent/decompToolchain.ts` source/object resolution;
- `tools/agent/diffFunc.ts` and `tools/lib/functionOracle.ts`;
- `tools/agent/contextExport.ts` and source-fact caching;
- `tools/build/deriveRodataSplits.ts`;
- build input/dependency receipts and finalization;
- idiom/callee/triage tools that still construct `<function>.c` directly;
- source policy and modification-scope attribution.

Search for direct constructions of `${functionName}.c`, `${funcName}.c`, and
`${fn}.c.o`; each is a migration candidate. Do not add a second ad hoc lookup to
each caller. Route them through the shared TU index.

## Source policy inside a multi-function file

A module can contain an allowlisted function beside ordinary functions. The
current filename-based scope is therefore insufficient.

Use the pinned C AST to attribute each construct to its enclosing function:

- register bindings and inline asm inherit that function's identity;
- nested functions retain their own identity when the target symbol table
  establishes one;
- module-scope constructs are audited separately and cannot borrow an arbitrary
  member's allowlist entry;
- SDK macro definitions retain their existing independently recognized policy;
- an `INCLUDE_ASM` mixed into an otherwise compiled module remains a partial
  fold and fails policy.

A module fold may retain an existing function-level exception. It may not add a
new exception, broaden an exception from one function to a module, or hide asm
inside a module-local macro.

## The progress/verifier CLI

Add `tools/diagnostics/tuProgress.ts`:

```text
npx tsx tools/diagnostics/tuProgress.ts
npx tsx tools/diagnostics/tuProgress.ts --remaining
npx tsx tools/diagnostics/tuProgress.ts --container ovl_11
npx tsx tools/diagnostics/tuProgress.ts --json
npx tsx tools/diagnostics/tuProgress.ts --check-complete
npx tsx tools/diagnostics/tuProgress.ts --verify
```

### Human report

Print folding and integrity as separate dimensions:

```text
Translation-unit reconstruction
  Eligible C functions: 2565
  Folded into modules:   412
  Remaining:             2153
  Valid module TUs:      63
  Excluded:              1 handwritten asm, 51 pending dead stubs

Project integrity
  Original-function inventory     PASS
  Decompilation completeness      PASS
  Source policy                   PASS
  Linked container bytes          PASS
```

Numbers above are illustrative; always derive current values.

`--remaining` lists bounded, deterministic records with container, address,
function name, current source/TU and immediate configured neighbors.
It is not a proposed group and must say so. The agent obtains membership evidence
from source, callers, globals, link order and the relevant section of
`notes/file-groupings.md`.

### JSON report

Use a versioned schema and preserve distinct states:

```ts
interface TuProgressReport {
  schemaVersion: 1;
  complete: boolean;
  inventory: {
    eligible: number;
    folded: number;
    remaining: number;
    excluded: Record<string, number>;
  };
  modules: Array<{
    container: string;
    source: string;
    object: string;
    members: Array<{ name: string; vram: string }>;
    status: "valid" | "invalid" | "undetermined";
    findings: string[];
  }>;
  remaining: Array<{
    container: string;
    name: string;
    vram: string;
    source: string;
    neighbors: string[];
  }>;
  integrity: {
    inventory: "pass" | "fail" | "undetermined";
    decompilation: "pass" | "fail" | "not-run";
    policy: "pass" | "fail" | "not-run";
    linkedBytes: "pass" | "fail" | "not-run";
  };
}
```

Never collapse `undetermined` into either folded or remaining. A malformed or
incomplete inventory is a broken gate, not ordinary module backlog.

### Exit contract

- `0`: every eligible function is folded and every requested integrity gate
  passes;
- `1`: the inventory is valid and ordinary module work remains;
- `2`: configuration, membership, policy, build or another required invariant
  is failed/undetermined.

This distinction lets an orchestrator continue on exit 1 and stop immediately
on exit 2.

`--check-complete` is read-only and structural. `--verify` runs the terminal
integrity gate as well. Default and `--json` may report integrity from fresh
artifacts but must label anything not executed as `not-run`.

## Structural completion heuristic

An eligible function is folded exactly when all of these hold:

1. The original-function inventory contains a unique container/address identity.
2. Exactly one configured C TU owns that identity.
3. The TU source is under the module path.
4. The module stem is semantic rather than address-derived.
5. The source AST defines the expected function identity exactly once.
6. The built object defines the corresponding text symbol exactly once.
7. Source emission order and object symbol offsets agree with original address
   order.
8. No active legacy per-function source/config entry remains.
9. The TU has no membership, flag, rodata, ownership, policy or interface
   finding.

The project is structurally complete only when:

- the eligible denominator is nonempty;
- every eligible function is folded;
- every module is valid;
- there are no orphan module sources or duplicate/uncovered functions;
- all exclusions are explicitly classified.

This heuristic deliberately does not infer semantic coherence from count alone.
A giant `game.c` could satisfy a naive path/count test. The prompt forbids that
behavior, and the report should emit review warnings for suspicious modules,
including very large member counts, no call/global relationship among members,
or a generic module name. Keep those warnings visible, but do not invent a
universal size threshold that would reject a genuine large original TU.

## Integrity gate

`tuProgress.ts --verify` (or a Make target wrapping it) runs, in order:

1. Fresh TU/original-function inventory consistency.
2. Updated `make complete` using TU-aware source status.
3. Repository-wide source policy with per-function AST attribution.
4. `make check-all` across the executable and every overlay.
5. Config-generation convergence against a before/after content snapshot.
6. `git diff --check`.
7. A final inventory read proving inputs did not change during verification.

The all-container build is mandatory. An executable rename changes the engine
symbol export and can relink every overlay; checking only the edited module's
container is not sufficient.

### Preserve the meaning of "complete"

Refactor `progress.ts`, but do not weaken it. After each fold:

- every function previously counted as decompiled remains counted;
- no live eligible function is a stub;
- no function disappears because its old same-named file disappeared;
- original function and byte denominators stay constant unless independent
  classification evidence intentionally changes them;
- linked bytes remain exact.

Add a regression fixture where two formerly complete functions move into one
module. `make complete` must remain true before and after, while a removed
function definition or a module containing an `INCLUDE_ASM` placeholder must
make it false.

### No exception growth

A module campaign needs a baseline receipt because source policy permits
already authorized exceptions. Compare each accepted iteration with the
campaign's starting snapshot and reject:

- newly added source-policy allowlist keys or kinds;
- new compiler-flag overrides;
- new register pins or embedded asm;
- new `INCLUDE_ASM` stubs;
- broadening a function exception to module scope.

Removing exceptions is allowed. The receipt belongs under ignored `build/` and
contains hashes and normalized exception sets, not a work queue or module list.
A standalone verifier may accept `--baseline <tree-ish-or-receipt>`; the
orchestrator always supplies the exact snapshot it started from.

## Orchestrator contract

The orchestrator repeatedly dispatches the existing prompt, one coherent module
per agent run:

```text
capture campaign baseline

loop:
    before = tuProgress --json

    if before has invalid/undetermined inventory:
        stop and report the concrete infrastructure/configuration failure

    if before.remaining == 0:
        run tuProgress --verify
        finish only on exit 0

    dispatch plans/tu-prompt.md with:
        - the current baseline identity
        - a bounded remaining-function sample
        - before.folded count
        - no authority to commit

    run per-iteration integrity gate
    after = tuProgress --json

    accept only if:
        - integrity passes
        - after.folded > before.folded
        - no previously folded function regresses
        - no exception set expands
        - the agent touched one coherent module scope
```

A run that improves naming/docs inside an already folded module but folds no new
function is useful maintenance, not progress for this campaign. The controller
should not loop on it as if the terminal heuristic moved.

The orchestrator never trusts the agent's summary or an edited note. It derives
progress again from source, config, objects and original identities.

### Autoloop extension mode: `fold`

Add a new mode to the existing `/auto_decompilation_loop` extension,
structurally mirrored on `repair`. The mode has no flags or arguments:

```text
/auto_decompilation_loop fold
```

It keeps running until every eligible function has been folded into
semantic modules and the terminal integrity gate passes. The operator
stops it the same way as any other loop mode — by typing any message.

#### Command registration

Add a `fold` case to `parseArgs()` in `commands.ts`, parallel to `repair`.
The mode word `fold` is not a function name, the same way `repair` is not one.

```ts
type ParsedArgs = { action: "run" | "repair" | "fold" | "status" | "stop" | "usage"; ... };
```

No arguments are parsed after `fold`. Unknown tokens produce a usage
message.

#### Loop body

The fold loop uses the same tier escalation, turn structure and checkpoint
machinery as `runRepairLoop`. Its iteration logic is:

```text
capture campaign baseline receipt (under build/)

state = readState(config)

while not flag.aborted:
    before = run tuProgress --json

    if before.inventory is invalid/undetermined:
        stop and report the infrastructure failure (exit 2)

    if before.remaining == 0:
        run tuProgress --verify
        finish only on exit 0

    dispatch plans/tu-prompt.md with:
        - the campaign baseline identity
        - a bounded sample of before.remaining
        - before.inventory.folded count
        - no authority to commit

    run per-iteration integrity gate (same contract as the abstract loop)
    after = run tuProgress --json

    accept only if:
        - integrity passes
        - after.folded > before.folded
        - no previously folded function regresses
        - no exception set expands
        - the agent touched one coherent module scope

    if integrity fails:
        stop — a failing gate means the infrastructure hypothesis is
        wrong, not that the next module deserves a try

    commit the fold if LoopConfig.commitOnMatch is set
    deps.baseline.add(changedFiles)

finish only on tuProgress --verify exit 0
```

#### State record

Add to `types.ts`:

```ts
interface FoldRecord {
  container: string;
  moduleName: string;
  sourcePath: string;
  members: string[];
  status: "accepted" | "failed" | "interrupted";
  foldedAt: string;
  tier: string;
  beforeFolded: number;
  afterFolded: number;
}

// Extend LoopState:
folds?: FoldRecord[];

// Extend FunctionOutcome:
| { kind: "folded"; functionName: string; members: string[]; moduleName: string; tier: string; changedFiles: string[]; commit?: string }
| { kind: "fold-failed"; functionName: string; members: string[]; record: FoldRecord; detail: string; commit?: string }
```

#### Integrity, exceptions and scope

Everything from the abstract Orchestrator contract applies:

#### Mid-run interruption

The operator stops the loop the same way as any other loop mode — typing a
message sets the `aborted` flag, which the loop body checks between
iterations (at the top of the `while`). If the agent is mid-turn when the
flag fires, the turn finishes and control returns to the loop body, which
then exits cleanly rather than starting the next module.

Interruption leaves the tree exactly as the last agent turn left it — no
automatic rollback. The integrity gate is not re-run; the operator chose
to stop, not to accept or reject the in-progress module. The loop writes a
`FoldRecord` with `status: "interrupted"` to durable state so the next
`fold` invocation can detect the partial state and decide whether to
restore or continue.

On restart, the loop reads the last `FoldRecord`:

- If its `status` is `"interrupted"`, the loop first checks whether the
  module's source still exists and whether its members are still in the
  `remaining` list of `tuProgress --json`. If the module is not reflected
  in the TU index at all, the loop restores the per-function sources from
  `HEAD` before proceeding (the same restore contract `repair` uses — the
  tree started from a committed, matching state).
- If the module is already reflected in the TU index, the loop treats it
  as accepted and moves on. This handles the case where the agent finished
  editing but the loop was stopped before committing.
- If the interrupted fold's source file exists and `tuProgress` reports the
  members as folded, the fold is recorded as accepted and the loop
  continues.
- In any other case, the per-function sources are restored from `HEAD`
  and the attempt is recorded as failed rather than blocking the queue.

- **Baseline receipt** — captured at campaign start; compared after each fold
  to reject new allowlist entries, flag overrides, embedded asm, or exception
  broadening.
- **Exception growth is rejected at the integrity gate** — you never reach the
  point of reverting a fold because the gate runs before accepting.
- **Fold queue** — derived from `tuProgress --json` each iteration, never from
  a manifest file. There are no options to filter or limit it.
- **Scope guard** — the same `scope-guard.ts` integration roots apply.
  A fold that touches files outside those roots is flagged but does not
  automatically fail; it may be establishing a new module path that needs
  registration.

## Implementation phases

### Phase 0 — freeze behavior and fixtures

- Record the current successful `make complete` and `make check-all` behavior.
- Freeze container-qualified original-function counts and representative EXE /
  overlay identities in tests, not as a mutable production manifest.
- Add synthetic fixtures for one-function C segments, two-function module
  segments, semantic singletons, overlays sharing a VRAM, handwritten asm and
  pending dead stubs.

**Acceptance:** tests reproduce current one-file-per-function behavior without
changing source or generated configs.

### Phase 1 — translation-unit index

- Add the original-function and TU APIs.
- Separate function spans from subsegments.
- Add AST/object membership validation.
- Test duplicate, missing, crossing, nested-function and overlay cases.

**Acceptance:** the current tree maps every configured function to its existing
per-function TU, and no consumer behavior changes yet.

### Phase 2 — one scratch multi-function module

- Prove nested semantic module stems through cpp, cc1, maspsx, GNU as, splat and
  the linker.
- Compile two synthetic or scratch functions in one C object.
- Prove the relocated oracle can compare each symbol independently.
- Prove module-scoped rodata relocations resolve correctly.

**Acceptance:** both functions match independently and a linked fixture matches,
without production source promotion.

### Phase 3 — build and generator support

- Replace source wildcard/awk divergence with shared configured-source listing.
- Preserve module segments through split/bootstrap/config repair.
- Add the checked fold helper if necessary.
- Make flag selection and dependency files path-safe for nested module stems.
- Refactor TU-owned rodata/data derivation.

**Acceptance:** a production-shaped fixture survives `split-all`, rebuild and a
second split without expanding back to per-function segments.

### Phase 4 — migrate function-oriented consumers

- Route source/object lookup through the TU index.
- Update progress, call graph, finalization, context export, diagnostics and
  receipts.
- Remove direct function-filename assumptions from active paths.
- Keep explicit-source experiment overrides working.

**Acceptance:** registration/unit tests pass; existing single-function workflows
remain unchanged; a function inside a module works with diff, triage, callee
truth, context export and finalization.

### Phase 5 — policy inside modules

- Attribute source-policy constructs to enclosing function identities.
- Keep module-scope constructs separate from member-level exceptions.
- Reject mixed stub/compiled-C modules and exception leakage between members.

**Acceptance:** an allowlisted construct in one member does not authorize a
neighbor, and module-scope forcing cannot borrow a member's exception.

### Phase 6 — progress and terminal verification

- Implement `tuProgress.ts`, JSON schema and exit contract.
- Add `make tu-progress` and `make tu-complete` or equivalently named targets.
- Compose the full integrity gate and input-stability recheck.
- Document commands in README and `notes/tools-directory-structure.md` only
  after implementation exists.

**Acceptance:** the current tree reports ordinary incomplete module work (exit
1), not infrastructure failure; synthetic all-folded fixtures exit 0; broken
fixtures exit 2.

### Phase 7 — orchestrated pilot

- Capture an ignored baseline receipt.
- Run `plans/tu-prompt.md` against one small, strongly evidenced group.
- Require strict folded-count increase and all integrity gates.
- Verify a second agent run sees the new source organization rather than a
  persistent queue entry.

**Acceptance:** one real module lands byte-identically, `make complete` remains
true, no exception grows, the remaining count decreases exactly by its member
count, and the next report is deterministic.

## Required tests

### Unit tests

- parse container-qualified original functions independently from C segments;
- map several functions into one TU interval;
- overlay functions at the same VRAM remain distinct;
- source AST and object symbols agree/disagree as expected;
- nested functions are attributed without stealing a sibling's identity;
- semantic singleton module passes;
- address-derived module path fails;
- duplicate ownership fails;
- missing member, extra symbol and wrong source order fail;
- intervening asm/data/object segment fails;
- orphan and stale per-function sources fail;
- pending dead stub and handwritten asm are visible exclusions;
- module-local asm is charged to the correct function;
- an exception cannot leak to another member;
- no newly added allowlist kind or flag override passes the campaign receipt;
- JSON ordering and exit codes are deterministic.

### Integration tests

- two C functions in one EXE module object, independently oracle-matched;
- two functions in an overlay module with `-G0`;
- module with pooled rodata/jump-table relocation;
- module with TU-owned common/small-data definitions;
- `make complete` before and after a fold;
- removed module member makes completion fail;
- full linked fixture remains byte-identical;
- call graph gives every member the same TU source but retains individual
  function identities;
- context export finds definitions inside modules;
- source policy scans an entire module once and reports function-scoped findings;
- split/config generation reaches a fixed point.

### Production acceptance

After tooling changes, run:

```text
npm test
npx tsx tools/diagnostics/tuProgress.ts --json
make complete
npx tsx tools/agent/sourcePolicy.ts
make check-all
```

Run the config-convergence check using a before/after snapshot suitable for a
dirty module-refactoring workspace. Do not rely on `git diff --exit-code` against
HEAD when intended source/config changes are present.

## Failure modes and safeguards

| Failure | Safeguard |
| --- | --- |
| A source move makes functions disappear from progress | Original inventory independent of C subsegments; member/object checks |
| A giant arbitrarily named file games the count | Prompt/review requirement plus conspicuous cohesion/size warnings; no claim of historical proof |
| Same VRAM merges functions from different overlays | Container-qualified identity in every API and report |
| Old and new sources both link | Configured-source enumeration and duplicate-symbol/orphan checks |
| Module's second function cannot be diffed | Function-to-TU object lookup and symbol-offset-aware oracle tests |
| One member's allowlist authorizes the whole file | AST enclosing-function attribution |
| Per-function rodata logic assigns pooled constants incorrectly | TU-scoped section attribution and undetermined on ambiguity |
| Split regenerates per-function segments | Module-aware generator fixed-point test |
| Agent adds a workaround while preserving bytes | Campaign baseline exception-set comparison plus source policy |
| `make check-all` passes after a function silently becomes a stub | TU-aware `make complete` remains a separate mandatory gate |
| Notes are edited to fake completion | Notes never enter the completion denominator |
| Orchestrator repeats no-progress work | Strict folded-count increase after each one-module dispatch |
| Concurrent source changes invalidate verification | Initial/final input hashes and baseline receipt; abort on drift |

## Completion criteria

This plan is complete when all of the following are true:

1. Function identity and TU identity are separate throughout active tooling.
2. Multi-function module sources build in the executable and overlays.
3. Every diagnostic/finalizer needed by `tu-prompt.md` resolves module members
   correctly.
4. `tuProgress.ts` provides deterministic reports and 0/1/2 exit semantics.
5. `make complete` remains true across module folds without same-named files.
6. Full source policy is module-aware.
7. `make check-all` remains the all-container byte authority.
8. The campaign gate rejects exception growth and regressions.
9. The orchestrator can dispatch one module repeatedly and stop only on a fresh
   terminal verification.
10. No committed queue or editable completion manifest exists; source/config
    organization is the progress record.

At that point the heuristic can honestly say "all eligible functions have been
folded into modules" while separately and simultaneously proving that the
matching decompilation remains complete.
