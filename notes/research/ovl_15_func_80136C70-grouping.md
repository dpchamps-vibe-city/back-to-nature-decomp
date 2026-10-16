# ovl_15_func_80136C70 — grouping evidence

Verified handoff identity:
`20b7a2f90f5f4c30b85efd654ab821644b479d6d06f926ef0261bc1dfcf212d6`.
The handoff reports byte identity and a passed full finalize gate. This review
changes notes only; matching is not rerun.

## Extend the existing renderer/formatter pair backwards

The ledger already records the gapless `80136C70` → `8013703C` pair and its
two call sites. The immediate predecessor supplies another corroborated
call/link edge:

- `build/ovl_15/asm/nonmatchings/ovl_15_func_80136B18/ovl_15_func_80136B18.s`
  calls `ovl_15_func_80136C70` at 0x80136B4C in a three-iteration slot loop,
  forwarding its mode argument. It then handles the save-slot cursor through
  `D_80137587` and the input flags.
- `configs/splat/ovl_15.yaml` places these functions at offsets 0x8D30,
  0x8E88 and 0x9254. The original sizes give gapless boundaries:
  0x80136B18 + 0x158 = 0x80136C70 and
  0x80136C70 + 0x3CC = 0x8013703C.
- The verified source `src/overlays/ovl_15/ovl_15_func_80136C70.c` renders
  one save-slot summary, including its status, date and two names. It calls
  the next function twice for the date's numeric fields. The exe-side
  `func_8001A970`/`func_8001ABF0` calls are in
  `src/overlays/ovl_15/ovl_15_func_8013703C.c`, not directly in the renderer.

This supports admitting `ovl_15_func_80136B18` to the existing candidate run.
Keep medium confidence: direct calls and link adjacency do not prove an
original TU boundary or static linkage.

## Shared scratch-buffer tie, without merging groups

The verified renderer fills 0x28 bytes at `D_80140EE0` with 0xFF and builds
halfword date text there. Its original assembly at
`build/ovl_15/asm/nonmatchings/ovl_15_func_80136C70/ovl_15_func_80136C70.s`
corroborates the buffer address and memset at 0x80136EE8–0x80136EFC.

`build/ovl_15/asm/nonmatchings/ovl_15_func_80133808/ovl_15_func_80133808.s`
uses the same address for halfword name text at 0x80133AAC–0x80133AE8 and
passes it to `func_80017B3C` at 0x80133AF0. An overlay-assembly reference
search finds these two functions as the buffer's only named code users.
This is new shared-global evidence linking the renderer to the existing
`D_8013759E/A0/A2/A4` state run's head, beyond their common text APIs.
The buffer's original linkage is unknown; retain this as a cross-cluster
same-TU hypothesis, not a reason to merge the runs or include intervening
functions.
