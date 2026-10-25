# Reconstruct source modules

Reconstruct as much of the original source organization as the evidence
supports. Each unit is a whole translation unit, consolidated from the
functions that compiled together, with meaningful names, a clear interface and
useful documentation, and an unchanged binary. Produce clean, understandable C,
not merely a proposed grouping or fewer source files. One run is not one
module: finish a unit, verify it, and go on to the next.

Read `AGENTS.md` and `configs/project-profile.md`. Examine the current source
organization and the relevant grouping notes. Preserve unrelated working-tree
changes.

## Discover the units

Identify functions that compiled together using shared private state, internal
helpers, call relationships, data/table usage and original link order.

Search `notes/file-groupings.md` for candidate function names and addresses and
read only the relevant sections; do not load the entire ledger. Its groupings
are hypotheses to test against the evidence, not authority.

Every member and every boundary needs evidence. Adjacency, shared compiler
flags or a widely used global are not evidence on their own. The strongest
evidence this toolchain leaves is small-data addressing. The assembler reaches
a small global through `$gp` only from a translation unit that declares it, and
absolutely from any other.

- Two functions that reach the same small global, one through `$gp` and one
  absolutely, are in different units. That is a proven boundary.
- Neighbors that share small globals through `$gp`, which outside functions
  reach absolutely, share a unit.
- `tools/build/deriveTuOwnedGlobals.ts` reads the `$gp` side from the original
  bytes.

Static helpers, private state, tables used only inside the group, and the
order of pooled `.rodata` (string literals and jump tables in member order)
corroborate.

Take the whole unit the evidence supports, never a slice of it. A group that
the evidence treats as one unit belongs in one module:

- Extend an existing module to cover the group.
- Merge existing modules when the evidence puts them in the same unit; a
  module that holds only part of a supported group is unfinished.
- Give a boundary inside such a group evidence of its own. Where only a
  mechanical limit stops the fold, name that limit in the note as the reason
  for the boundary. A per-function flag override that cannot yet be reconciled
  is such a limit.
- Do not treat a dead function's undecompiled `INCLUDE_ASM` stub between
  members as a boundary: the module carries it.

Leave a neighbor outside only when the evidence places it in another unit or
says nothing either way.

## Make the change

Organize the functions into an appropriately named source file, preserving
original function order where relevant. `tools/build/foldTranslationUnit.ts`
folds whole units: list every function the module should hold, including the
members of existing modules it absorbs. Produce or update a public header only
where there is an externally used interface; it need not expose every function
in the module, and a module may publish through more than one header when each
serves its own callers.

Reconcile declarations and includes using established canonical identities and
corroborated interfaces. Follow repository rules for types, globals, ownership
and source policy. Do not infer private linkage merely because functions now
share a file.

Make the necessary build/configuration changes through the supported generation
path. Address concrete tooling assumptions exposed by the change without
turning the task into an unrelated infrastructure project. Never hand-edit
generated outputs or add matching workarounds to force a grouping to succeed.

## Name and document what you discover

- Rename functions, parameters, locals, types, fields and globals when their
  purpose is supported by the implementation and its callers.
- Prefer names describing demonstrated behavior over speculative gameplay
  meaning. Leave uncertain concepts neutrally named rather than manufacturing
  confidence.
- Propagate public renames through all callers, headers, overlays and relevant
  configuration. Preserve the connection to original container/address
  identities; shared RAM addresses do not make different overlay functions the
  same symbol. Use available canonicalization tools and safeguards rather than
  assuming planned machinery is already implemented.
- Document the module's responsibility and public interface: inputs, outputs,
  units, state changes and important constraints.
- Explain non-obvious behavior and genuinely necessary matching-sensitive
  constructions. Do not narrate obvious statements or turn source comments into
  experiment logs.
- Improve readability within each module where byte identity permits. Keep
  naming, documentation and readability changes evidence-backed and
  verifiable. Do not silently change behavior or disputed types as part of a
  rename.

Naming, documentation and readability improvements are part of this task when
they arise from understanding the modules you reconstruct. Avoid unrelated
cleanup.

## Investigate compiler settings

Test the merged source under baseline settings first. Where existing overrides
are involved, investigate them across the proposed TU using the relevant
findings and workflow in `plans/flag-review-report.md`.

Do not invent a TU boundary to protect an override. A conflict may indicate a
source/context problem, an incorrect grouping, or an unresolved compiler-setting
hypothesis. An unchanged neighbor is compatible evidence, not affirmative proof
of an original flag. Retire unnecessary exceptions only after verification.

## Verify and finish

Preserve the existing matching build while experimenting. Accept the
reconstruction only when:

- Canonical symbol identities and affected interfaces are checked; no obsolete
  aliases, implicit declarations or known declaration conflicts are silently
  introduced by the change.
- New scope/name collisions and changes to declaration visibility, linkage or
  global ownership have been addressed explicitly.
- Affected functions remain byte-identical, including callers affected by
  shared-header changes.
- The complete linked build passes the project's full verification gate across
  the executable and overlays, including associated data, callback pointers,
  jump tables and placement.
- Source-policy and modification-scope checks pass.

Update the relevant grouping note with the evidence for each module's
membership and for each of its boundaries, and what remains uncertain. State
what each boundary rests on; a disclaimer is not a substitute for evidence.
The evolving source organization is the record of progress; do not create a
separate discovery queue or tracking framework.

Fold, name, document and verify as many units as you can finish in this run.
Verify in batches, not per unit: the fold helper already proves each fold
where it lands (it re-splits, builds and byte-checks the container, and rolls
back on failure), so fold, name and document several units, reconcile their
interfaces in one pass, and run the full verification once for the batch. Fix
what it names before starting the next batch. Summarize what changed, why each module's functions belong together,
the naming/interface discoveries, flag findings and verification results. If
blocked, leave the live build intact and explain the concrete obstacle and next
useful experiment. Do not commit.

Success means **coherent modules with meaningful names, clear interfaces,
useful documentation and an unchanged binary**.
