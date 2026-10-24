# Project-wide symbol canonicalization

Status: planned; not implemented. No automatic commits.

## Purpose

Carry existing function renames through the executable and overlays, reconcile
known interface discrepancies, and prevent those inconsistencies from returning.
Use the existing tools and a small static-analysis/rewrite script if needed—not
a new symbol-management framework.

The repository's source and configuration remain the record of discovery.
There is no separate alias database or research queue to maintain.

## Starting case: Rand

`configs/symbols/exe.txt` identifies `Rand` at `0x80012A34`, and `src/Rand.c`
defines `u32 Rand(s32 arg0)`. Overlay callers still use `func_80012A34`, with
inconsistent signed/unsigned return declarations.

The inspected engine exporter has a concrete bug: it can choose the repeated
compiler marker `__gnu_compiled_c` instead of `Rand` at their shared address,
then discard the marker without restoring Rand. Fix this generically, not with
a Rand-specific exception.

## Execution checklist

1. **Establish a matching baseline.** Preserve unrelated working-tree changes
   and run `make -j8 check-all`.
2. **Fix the exporter.** In `tools/build/exportEngineSymbols.ts`, exclude
   compiler markers before choosing public names. Preserve valid data exports
   and overlay exclusions. Regenerate and confirm Rand and other established
   names reach the overlay symbol inputs.
3. **Find obsolete aliases.** Reuse `tools/lib/symbolIndex.ts`, container
   metadata and actual definitions to find anonymous names for already-named
   functions throughout configured active sources. Identity is the defining
   container plus exact entry address—not a global address-only lookup.
4. **Migrate Rand first, then the remaining established renames.** Use the C
   parser to update bound declarations, calls and function-pointer references,
   not global text replacement. Handle assembly and macro references explicitly;
   report unsupported or ambiguous cases and leave them untouched. Preserve
   historical notes and archived drafts.
5. **Keep this pass name-only.** Do not change types, casts, control flow,
   linkage, global ownership or compiler flags. List prototype disagreements
   separately. If a rename exposes a declaration collision, resolve it as a
   separately reviewed interface change rather than silently forcing the rename.
6. **Regenerate and verify.** Change owning source/configuration inputs, never
   generated headers or linker files by hand. Check affected functions against
   the original bytes, then run `make -j8 check-all`. Correct additional tooling
   consumers only where the migration exposes a concrete naming failure.
7. **Reconcile known interface conflicts separately.** Audit against matched
   definitions, SDK declarations and original ABI/caller evidence. For Rand,
   use the corroborated `u32 Rand(s32)` contract, retaining signed locals or
   casts only where caller evidence requires them. Publish validated interfaces
   in appropriate hand-maintained headers; do not use generated
   `include/functions.h` as independent signature evidence or a blanket compiler
   header. Verify every affected caller again.

Do not invent new semantic names or rename data globals in this pass. Genuine
public aliases and different overlay functions at the same RAM address must not
be collapsed. Conflicting meaningful names need a decision, not alphabetical
selection. A repeated pass should make no further changes in a completed scope.

## Lightweight symbol and interface guards

Reuse the existing parser, callee audit, compiler and verification tools. Add
only the missing focused checks. They should validate the current project,
independently of which editing task produced it.

| Guard | Required behavior |
|---|---|
| **Canonical names** | Detect obsolete aliases with established replacements in active source and fresh generated output. Exclude historical prose, genuine public aliases and still-unnamed functions. |
| **Correct identity** | Keep imports bound to their defining containers. Reject ambiguous automatic rewrites and confirm that calls and callbacks retain their original targets. |
| **Consistent interfaces** | Detect implicit declarations and known type/arity disagreements between definitions and callers, including callback uses. Report unknown contracts separately; never invent a signature to silence the check. |
| **Stable propagation** | Regeneration must retain established names rather than reintroducing anonymous aliases. Report stale symbol exports and naming failures in affected preparation/diagnostic consumers. |
| **Verified output** | Check affected functions against original bytes and run `make -j8 check-all`, including linked data and callback/jump tables. Header changes must rebuild dependent callers. |

Name checks and interface checks report separately: a verified name-only change
can still have an explicitly outstanding prototype conflict. A completed scope
must pass its checks; unresolved cases must remain visible rather than being
silently treated as clean.

Keep linkage, global definitions, ownership and compiler flags unchanged during
name cleanup. Shared types and global declarations retain their established
homes under `configs/project-profile.md`. On a failed check, fix the concrete
issue or leave that change unapplied, without overwriting unrelated work.

## Focused regression coverage

Keep tests proportional to the work:

- Exporter: Rand survives a same-address compiler marker; unrelated public/data
  exports remain valid.
- Identity: two overlays sharing an address are not treated as one function;
  a known executable import resolves correctly.
- Rewriting: calls and callbacks migrate, while shadowed locals and unrelated
  text do not; a second run is unchanged.
- Interface checks: a conflicting prototype and an implicit declaration are
  reported; a corroborated declaration passes. Name-only rewriting does not
  silently change either contract.
- Integration: the Rand pilot and another established rename retain exact
  bytes, regeneration retains canonical names, and `make -j8 check-all` passes.

Run these focused checks through an appropriate existing validation path so
later edits or regeneration cannot silently undo the cleanup. No task-specific
workflow is required.

## Completion

Deliver the exporter fix, verified propagation of existing resolvable function
renames, the reusable focused guards, and a short list of remaining ambiguous
references or signature conflicts. Name cleanup and interface cleanup are
separate results; neither should conceal incomplete work in the other.

The result is a coherent, verified set of function names and interfaces, with
small checks that preserve that coherence. Source organization and future
refactoring workflows are outside this plan.
