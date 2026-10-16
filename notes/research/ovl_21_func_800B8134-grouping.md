# ovl_21_func_800B8134 — shared-handle exit callback

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with source/evidence identity
`01eddb7a25755e58a214da770f956e7c45f00f0c4fe7d58ead5834324e77cc69`.
Its origin is agent; the supplied changed files are
`include/globals_override.h`, `include/overlays/ovl_21.h`, and
`src/overlays/ovl_21/ovl_21_func_800B8134.c`. This note does not rerun matching
or change build inputs.

## New grouping evidence

- The verified source reads the signed handles at `D_800C0448 +0x984/+0x986`
  and passes each non-`-1` value to `func_8001FAE8`. Existing cluster member
  `src/overlays/ovl_21/ovl_21_func_800B8320.c` resets exactly these two fields
  to `-1`; `src/overlays/ovl_21/ovl_21_func_800B8A80.c` writes the first from
  `func_8001FABC`, and `src/overlays/ovl_21/ovl_21_func_800B871C.c` writes
  the second from that same helper. This adds a cleanup consumer to the
  recorded members' shared-handle lifecycle, not merely another reference
  to the broad state base. The target assembly at
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B8134/ovl_21_func_800B8134.s`
  confirms both `lh` accesses and guarded calls.
- The already-documented `D_800BCB58` table in
  `build/ovl_21/asm/data/3C64.data.s` places this function last, after
  800B7E98/800B7FC8/800B806C/800B80D4, matching their consecutive link order
  in `configs/splat/ovl_21.yaml`. The dispatcher assembly at
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B7E3C/ovl_21_func_800B7E3C.s`
  indexes it with `D_80070CC4` (`D_8006C838 +0x448C`). The verified target
  repeats the update chain in `src/overlays/ovl_21/ovl_21_func_800B80D4.c`
  and writes that same outer state to `0xFF` on completion. The table alone
  is not new evidence; the verified shared-state exit role now connects this
  previously unrecorded callback to the family.
- Its 0xF4-byte target body ends exactly at existing member 800B8228:
  `0x800B8134 + 0xF4 = 0x800B8228`. The configuration corroborates adjacent
  offsets 0x314/0x408. That neighbour's verified source calls 800B8320,
  which resets the very handles consumed here. This adjacency supports the
  shared-state connection; it is not a direct call from 800B8134 to 800B8228.

## Membership conclusion and limits

Add only `ovl_21_func_800B8134` to the existing `ovl_21` D_800C0448 cluster
as its completion/exit callback. Under the existing same-TU hypothesis, the
shared handle lifecycle, ordered callback family and initializer adjacency
support membership. Confidence remains medium: globals, callbacks and calls
can cross translation units. No original TU boundary, data ownership,
compiler quirk, additional member, or merger with the far-buffer cluster is
established.
