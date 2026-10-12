# ovl_21_func_800B8228 — shared-state initializer and link adjacency

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with source/evidence identity
`e4a6c91df3fc9b76eef8487cf7f2c6572034cc6e1f1396dc51e996675e3b1d18`.
The supplied changed files are `include/overlays/ovl_21.h` and
`src/overlays/ovl_21/ovl_21_func_800B8228.c`. This note does not rerun matching
or change build inputs.

## New grouping evidence

- The verified `src/overlays/ovl_21/ovl_21_func_800B8228.c` clears the leading
  state pair and the word at `D_800C0448 + 0xC`, calls
  `ovl_21_func_800B8320`, then initializes halfwords at +0x646 through +0x680.
  `src/overlays/ovl_21/ovl_21_func_800B8320.c`, already a ledger member,
  resets other fields of the same state block, reads its +0x64C halfword,
  and calls the next existing member, `ovl_21_func_800B836C`. This connects
  the newly documented initializer to the existing shared-state reset chain.
- The extracted target assembly at
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B8228/ovl_21_func_800B8228.s`
  records a 0xF8-byte body and the direct call to `ovl_21_func_800B8320`.
  Its end is exactly `0x800B8228 + 0xF8 = 0x800B8320`.
  `configs/splat/ovl_21.yaml` corroborates the adjacent offsets 0x408 and
  0x500. The call graph and link order therefore agree, in addition to the
  shared storage.

## Membership conclusion and limits

Add `ovl_21_func_800B8228` to the existing `ovl_21` D_800C0448 state-pair
writer cluster in `notes/file-groupings.md`; its membership was not previously
recorded. Under that cluster's existing same-TU hypothesis, the shared-state
initialization/reset chain and gapless call/link adjacency support extending
membership to this initializer. Confidence remains medium: shared globals and
direct calls can cross translation units, and adjacency alone does not establish
an original boundary. No new TU boundary, global ownership, or compiler quirk
is established.
