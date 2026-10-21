# ovl_21_func_800B96B0 — result-selection handler

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with source/evidence identity
`9f7b3eec263dbfb7fac064956280a9a6300ae8ac5dd99c41b9991dd15b8467a9`.
Its changed files are `include/globals_override.h`, `include/overlays/ovl_21.h`,
and `src/overlays/ovl_21/ovl_21_func_800B96B0.c`. This note does not rerun
matching or change build inputs.

## New grouping evidence

- The verified source calls `ovl_21_func_800BA4C0` and reads the shared
  `D_800C0448` state at +2/+0x640/+0x642. The latter selector fields are
  also consumed by established members
  `src/overlays/ovl_21/ovl_21_func_800B871C.c` and
  `src/overlays/ovl_21/ovl_21_func_800B88A0.c`, respectively.
- Its immediate predecessor,
  `src/overlays/ovl_21/ovl_21_func_800B9644.c`, shares more than that state
  and update call: both write the halfword at `D_8006C838 +0xE62A`, write
  the word at +0xE62C, and increment the word at +0x448C. The predecessor
  copies the result from +0xE628 and sets the flag to 1; this handler maps
  a random or saved selector through `D_800BCC14` and clears the flag.
  The extracted target at
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B96B0/ovl_21_func_800B96B0.s`
  confirms these stores at 0x800B9768/0x800B9784/0x800B978C.
- `build/ovl_21/asm/data/3C64.data.s` places 800B9644 and 800B96B0 in the
  final two slots (17/18, zero-based) of `D_800BCB6C`, which
  `src/overlays/ovl_21/ovl_21_func_800B806C.c` indexes with the leading
  state halfword. `configs/splat/ovl_21.yaml` places them consecutively at
  offsets 0x1824/0x1890; this handler's 0xE8-byte body ends at 800B9798
  (offset 0x1978). Thus the dispatcher relationship and link order agree,
  corroborating the newly observed shared result-field protocol.

## Membership conclusion and limits

Add only `ovl_21_func_800B96B0` to the existing `ovl_21` D_800C0448
state-pair cluster as its result-selection handler. The shared state and
result fields, common update call, and ordered dispatcher/link adjacency
support membership under the existing same-TU hypothesis. Confidence remains
medium: globals and calls can cross translation units. No TU boundary,
data ownership, or additional neighbour membership is established.
