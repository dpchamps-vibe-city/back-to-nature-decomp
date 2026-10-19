# ovl_11_func_800F787C: menu-driver candidate pair

## Verified premise

The supplied handoff reports byte-exact matching and a passed full finalize
gate for `ovl_11_func_800F787C`, source/evidence identity
`923fa06a33fbaa399654d01b989ccf6e1fb4c3a711cb18782c976a0bffa2f261`.
Its changed-file list names
`src/overlays/ovl_11/ovl_11_func_800F787C.c`. This notes-only review reads
that source and original disassembly; it does not rerun matching or the gate.

## New grouping evidence

The verified driver supports a low-confidence same-TU candidate pair with
its immediate successor `800F7AA0`, beyond its already recorded dependencies
on the numeric-row and slot-rendering helpers:

- `build/ovl_11/functions/ovl_11_func_800F787C.s` gives the driver size as
  0x224, with its last instruction at `0x800F7A9C`.
  `build/ovl_11/functions/ovl_11_func_800F7AA0.s` starts at `0x800F7AA0`:
  `0x800F787C + 0x224 = 0x800F7AA0`, a zero-gap link-order pair.
  The driver calls that successor at `0x800F79C0`, so call and link order
  agree rather than providing adjacency alone.
- Both use the particular menu-state cluster `D_80126E40`, `D_80126E44`,
  `D_80126E4A`, and `D_80126E4E`. The driver chooses record banks from the
  mode, reads cursor/page state for rendering and gates its default message
  on `D_80126E4E == 0xFF`. The successor reads those same cursor/page fields
  for record selection and writes `D_80126E4E = 0` when selection succeeds
  (`0x800F7B30`), before control returns to the driver.
- Their exit paths both store `0xFE` to `D_80126E40` and call
  `func_80022738` / `func_8001FABC(3)`. Their mode 0/2/1 branches also choose
  the same record-bank bases, `0x80070EC2` / `0x80070D42` / `0x80071042`:
  the driver renders those banks, while the successor passes them to
  `800F8224` for compaction. These accesses are visible in the original
  disassemblies, independently of the successor's nonmatching C draft.

## Limits

This is a candidate pair, not proof of an original translation-unit boundary.
The globals are absolute-addressed; no static linkage, data ownership,
declaration-order effect or register-variable quirk was established.
The successor's existing regional connection is documented in
[the record-dispatcher evidence](ovl_11_func_800F7D34-grouping.md), but this
entry does not assert one TU spanning every regional helper. In particular,
the driver's distant rendering calls do not merge this pair with the
`800F8B4C` / `800F8C1C` pair, and the preceding `800F77F0` is not added on
adjacency alone.
