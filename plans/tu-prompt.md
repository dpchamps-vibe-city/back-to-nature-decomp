# Reconstruct the next source module

Improve the project through **one evidence-backed module reconstruction**:
consolidate a plausible translation unit, give its functions and data meaningful
names where justified, clarify its interface, and document its behavior.
Produce clean, understandable C while preserving exact binary identity—not
merely a proposed grouping or fewer source files.

Read `AGENTS.md` and `configs/project-profile.md`. Examine the current source
organization and relevant existing grouping notes, then choose a small,
well-supported candidate. You may create a module or extend an existing one.
Preserve unrelated working-tree changes.

## Discover the group

Identify functions that plausibly compiled together using shared private state,
internal helpers, call relationships, data/table usage and original link order.

Search `notes/file-groupings.md` for candidate function names and addresses and
read only the relevant sections; do not load the entire ledger. Treat its
existing groupings as hypotheses, not authority.

Explain the proposed membership and boundaries. Do not infer membership solely
from adjacency, shared compiler flags or a widely used global. Include
confidently supported members; leave uncertain neighbors outside rather than
forcing a complete subsystem into one file.

## Make the change

Organize the functions into an appropriately named source file, preserving
original function order where relevant. Produce or update a public header only
where there is an externally used interface; it need not expose every function
in the module.

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
- Improve readability within the selected module where byte identity permits.
  Keep naming, documentation and local readability changes scoped,
  evidence-backed and individually verifiable. Do not silently change behavior
  or disputed types as part of a rename.

Naming, documentation and readability improvements are part of this task when
they arise from understanding the selected module. Avoid unrelated cleanup.

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

Update the relevant existing grouping note with a concise explanation of the
discovery and remaining uncertainty. Describe module names and boundaries as
reconstructed hypotheses unless stronger evidence exists. The evolving source
organization is the record of progress; do not create a separate discovery
queue or tracking framework.

Stop after **one coherent, verified reconstruction**. Summarize what changed,
why the functions belong together, the naming/interface discoveries, flag
findings and verification results. If blocked, leave the live build intact and
explain the concrete obstacle and next useful experiment. Do not commit.

Success means **a coherent module with meaningful names, a clear interface,
useful documentation and an unchanged binary**.
