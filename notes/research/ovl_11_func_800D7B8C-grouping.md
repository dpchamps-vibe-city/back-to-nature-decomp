# ovl_11_func_800D7B8C: two-grid growth-update grouping

## Verified anchor

The documentation handoff reports byte identity and the full finalize gate
passed for source/evidence identity
`19942e3066ec176ca1d5910ad5b31acbd4404ec06cfa1957bae70679c7f0547f`.
Its supplied changed-file list is `include/game_types.h`,
`include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800D7B8C.c`. This review changes notes only;
it relies on that verification rather than rerunning matching.

## New membership evidence

- **Same two grids, not merely the broad work-area global.** The verified
  source walks layer 0 with 25 rows and 45 columns, then layer 1 with 7 rows
  and 7 columns, resolving each entry through `ovl_11_func_800DAF60`.
  Original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D7B8C/ovl_11_func_800D7B8C.s`
  corroborates the bounds at 800D7BBC–800D7BE4 and lookup at 800D7C48.
  `src/overlays/ovl_11/ovl_11_func_800DAF60.c` selects the existing cluster's
  `D_80071DFC[25][45]` and `D_80074124[7][7]` eight-byte records.
- **Shared record-state and helper pipeline.** The target calls
  `800DA3AC` on layer-0 entries (800D7C94), tests the entry tag through
  `800D5868` (800D7CA0), and normalizes its tags through `800DAF24`
  (800D7D9C/800D7DD4). Its writes to the two tag halfwords, bytes +4/+5
  and halfword +6, including consuming bit 0x8000 at 800D7DF0–800D7E08,
  tie it to the existing entry-state operations in
  `src/overlays/ovl_11/ovl_11_func_800D85F8.c` and the record-tag update
  run. The existing grid walker
  `src/overlays/ovl_11/ovl_11_func_800D9064.c` applies that run's
  `800DA49C`/`800DA518`/`800DA454` helpers to the same main grid.
- **Nearby initializer is supporting context only.**
  `configs/splat/ovl_11.yaml` places the existing cluster member
  `800D7B24` at offset 0x1FD04, the return-zero `800D7B84` at 0x1FD64,
  and this function at 0x1FD6C. The initializer's source clears the same
  fields in every 7×7 entry. This is nearby related work, not a direct
  caller/callee adjacency: the intervening function remains, and no call
  between the initializer and this updater is established.

## Inference and limits

Add 800D7B8C to the existing Ovl11D124Entry table cluster, retaining its
**low-confidence candidate-TU** status. The premise is that operations over
these particular two grids, sharing the tag/state representation and helper
pipeline, may belong to one original source unit. The tables are not proven
TU-private, and the helper calls could cross a source boundary. Neither the
new reconstructed growth-state type nor a generic reference to `D_8006C838`
is independent evidence of common origin. No exact TU boundary,
declaration-order effect, register-variable quirk or SDK cluster is established;
this does not merge or upgrade the broader clusters.
