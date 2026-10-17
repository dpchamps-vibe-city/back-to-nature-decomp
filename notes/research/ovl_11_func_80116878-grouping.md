# ovl_11_func_80116878: selected-record threshold run

## Verified premise

The supplied handoff reports byte-exact matching and a passed full finalize
gate for `ovl_11_func_80116878`, with source/evidence identity
`14e1be206892e5e20f82ccbb43e85624fcace1d76bf49e82de224733b26ad0d5`.
Its changed files are `include/globals_override.h`,
`include/overlays/ovl_11.h`, and
`src/overlays/ovl_11/ovl_11_func_80116878.c`. This documentation reads the
verified source and original disassembly; no solver or build was run.

## New membership evidence

Candidate same-TU run, confidence medium:
`801165E0` → `80116878` → `801168C0`.

- **Gapless original link order:** the original disassemblies give sizes
  `0x298`, `0x48`, and `0xC0`, respectively:
  `0x801165E0 + 0x298 == 0x80116878`,
  `0x80116878 + 0x48 == 0x801168C0`, and
  `0x801168C0 + 0xC0 == 0x80116980`.
  `configs/splat/ovl_11.yaml` agrees, with split offsets
  `0x5E7C0` → `0x5EA58` → `0x5EAA0` → `0x5EB60`.
- **Call graph agrees with link order:** `801165E0` calls `80116878` at
  `0x80116708` and `801168C0` at `0x80116798`. Both calls receive
  `&D_8012D548`. The predicate result selects the caller's next message
  and state transition; the following helper handles a later state.
- **Shared selected-record operation:** both helpers load the record's
  signed selector at `+0x5C`, fetch the selected unsigned halfword at
  `+0x28`, and call `ovl_11_func_800D740C`. Both sign-extend the low
  halfword of that result. The verified C expresses the predicate as
  `D_80071A60 >= threshold`.
- **Shared global through different address spellings:** `80116878`
  directly loads `D_80071A60`. At `0x801168FC`–`0x80116908`, `801168C0`
  loads, subtracts the same selected-entry threshold from, and stores
  the word at `D_8006C838 + 0x5228`.
  `0x8006C838 + 0x5228 == 0x80071A60`, so this is the same word,
  corroborating a predicate/update pairing rather than adjacency alone.

Primary disassembly paths:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_801165E0/ovl_11_func_801165E0.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80116878/ovl_11_func_80116878.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_801168C0/ovl_11_func_801168C0.s`

## Limits

This is candidate membership, not a recovered original TU boundary. The
shared word is main-RAM state, not a proven private static or GP-owned
symbol. `src/overlays/ovl_11/ovl_11_func_8011CC08.c` also reads
`D_80071A60`; that remote renderer is not added on shared state alone.
Likewise, the remote lookup helper `800D740C` and other `StructD548`
consumers are dependencies, not established same-TU members.

The contiguous `80116980` text/sprite table-builder run remains separate:
its boundary adjacency alone does not justify merging it with this run.
No declaration-order or register-variable quirk was established here.
