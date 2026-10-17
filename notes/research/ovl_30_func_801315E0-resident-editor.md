# ovl_30_func_801315E0: resident editor and resolved policy false positive

## Verified result

Restored the exact C preserved by the 2026-10-17 autoloop park, without changing
its body. Source: `src/overlays/ovl_30/ovl_30_func_801315E0.c`, SHA-256
`09a6406aef8dc292e16fb380c993643bbfd546055c681992b4bb27e90028b9e0`.
Verified build-input identity:
`3fc653dc0f50ea6e27097fa0a858912d789b362ca72db865a3d258f7827e58a2`.

The relocated-byte oracle reports MATCH, 406/406 words (0x658 bytes); the
staged control-flow/population/schedule/allocation residual is 0/0/0/0.
The authoritative `finalizeWorkspace` route passed exact diff, `make check-all`,
function-change scope, clean-source policy and generated signature publication.
The PS-X EXE and all 13 configured overlays match. The fresh-process report is
`build/unpark/ovl_30_func_801315E0/finalization.json`; the separate, explicitly
requested policy-tool edits were not charged to the function's scope gate.

## Function and declarations

The function builds 32 automatic `Ovl30ValueMenuEntry` records, selecting fields
from the 0x1D4-byte `D_80076220` record family. Its signed-halfword resident
selector is at `D_8006C838 + 0x4458`. When `func_80013394()` permits the menu,
it prints the selected resident, transfers the caller's cursor through
`D_80134014` to `ovl_30_func_80133454(entries, 32)`, and returns the updated
cursor. Input bits 4/8 call `ovl_30_func_80133974` with selector width 2, bounds
0..0x24 and delta -1/+1. The decrement path returns before testing increment.
All four callee declarations are independently corroborated by `calleeTruth`.

The field bases, resident-selector view and label declarations are in
`include/globals_override.h`. Four adjacent-halfword views use declaration
asm labels, for example:

```c
extern u16 D_80076226[] __asm__("D_80076224+2");
```

This attaches an assembler name/address expression to a C object declaration;
it is not an asm statement and inserts no instructions. The four labels refer
to the halfwords two bytes after D_80076224, D_80076228, D_80076234 and
D_8007623C. Distinct C declarations retain the measured compiler behavior while
the labels express the relocation bases. Byte identity validates this
representation, not a claim that the original source used these declarations.

## Why it was parked

The autoloop reached an exact candidate but its patch/header policy fallback
recognized only bare identifiers inside declaration asm labels. It rejected
`+2` and mislabeled the four declarations as embedded assembly, causing the
`asm-needs-human-approval` park. The complete-source AST audit already
recognized declaration labels separately from instruction asm.

The fallback now checks declaration placement through the pinned C AST and
accepts a symbol optionally followed by one decimal/hex integer byte offset.
Asm statements, register bindings, operand lists, escaped instruction text and
nonconstant expressions remain outside that exemption. Regression tests in
`.pi/extensions/shared/source-policy.test.ts` reproduce all four declarations
and exercise those negative controls. No function-specific allowlist entry or
compiler-flag override was added. The obsolete approval request and runtime
park entry are removed after verification.

## Grouping evidence and limits

This extends the existing adjacent animal-editor candidate group, rather than
establishing a new source-file boundary:

- Original extents give `80131100 + 0x4E0 = 801315E0`; this function in turn
  ends at `801315E0 + 0x658 = 80131C38`. Original instructions are in
  `build/ovl_30/asm/nonmatchings/ovl_30_func_801315E0/ovl_30_func_801315E0.s`.
- Like the three preceding animal editors, this function uses an automatic
  20-byte-entry menu array, the same gate, cursor, display helper and field
  writer, and the same selector-input tail. Its record stride and selector
  width differ: 0x1D4/2 instead of 0xB4/4.
- Its resident labels begin at D_8012EA00, immediately after the preceding
  animal headings, and continue through the heading D_8012EC6C in
  `build/ovl_30/asm/data/0.rodata.s`.

Together, adjacency, the mirrored menu protocol and rodata order support a
medium-confidence wider editor-family/TU candidate. Shared helpers alone would
not: they are used elsewhere in the overlay. No private-static ownership or
TU-specific compiler fingerprint proves a file boundary. In particular,
80131C38's different editor protocol is not evidence to extend the group
further, and this does not merge it with the distant shared-helper cluster.
