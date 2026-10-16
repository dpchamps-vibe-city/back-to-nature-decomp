# ovl_21_func_800B7FC8 — reset/update callback and dispatcher adjacency

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with source/evidence identity
`6d82d0c4373e865e6d4697376b4d618054948bbe41fe50d911009d771d5f94df`.
Its origin is agent; the supplied changed files are `include/overlays/ovl_21.h`
and `src/overlays/ovl_21/ovl_21_func_800B7FC8.c`. This note does not rerun
matching or change build inputs.

## New grouping evidence

- The extracted target assembly at
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B7FC8/ovl_21_func_800B7FC8.s`
  records a 0xA4-byte body, ending exactly at the existing cluster member
  `ovl_21_func_800B806C`: `0x800B7FC8 + 0xA4 = 0x800B806C`.
  `configs/splat/ovl_21.yaml` corroborates adjacent offsets 0x1A8 and 0x24C.
- `build/ovl_21/asm/data/3C64.data.s` supplies the five-entry callback table
  `D_800BCB58`: 800B7E98, 800B7FC8, 800B806C, 800B80D4, 800B8134, in the
  same consecutive function order recorded by the splat configuration.
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B7E3C/ovl_21_func_800B7E3C.s`
  confirms dispatch through that table using `D_80070CC4`, the word at
  `D_8006C838 + 0x448C`. The target increments that same word when
  `func_80013394() == 1` and `func_8001FE6C() == 0`. Thus the callback graph,
  shared outer state and link order agree.
- The verified target repeats the four-call update sequence
  `800BB138` → `800BB250` → `func_8001B5DC` → `800BB2B4` found in its gapless
  successor, `src/overlays/ovl_21/ovl_21_func_800B806C.c`, and in
  `src/overlays/ovl_21/ovl_21_func_800B80D4.c`. It additionally calls the
  already-recorded `D_800C0448` reset helpers `800B8320` and `800B9798` (the
  latter with `(0, 0)` and `(1, 0)`). Their verified sources at
  `src/overlays/ovl_21/ovl_21_func_800B8320.c` and
  `src/overlays/ovl_21/ovl_21_func_800B9798.c` reset the shared state block
  and clear the six records' offset-0x18 halfwords respectively.

## Membership conclusion and limits

Add only `ovl_21_func_800B7FC8` to the existing `ovl_21` D_800C0448 cluster
as a reset/update callback that advances the outer state after both completion
probes pass. Under the existing same-TU hypothesis, the ordered callback table,
gapless adjacency to its shared-update-chain sibling and calls into the
cluster's reset helpers support this previously unrecorded membership.

Confidence remains medium: this function does not directly access
`D_800C0448`, and tables, globals and calls can cross translation units.
No original TU boundary, global ownership, compiler quirk, merger with the
far-buffer cluster, or additional ledger member is established.
