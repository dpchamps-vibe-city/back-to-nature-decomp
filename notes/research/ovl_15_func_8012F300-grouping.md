# ovl_15_func_8012F300 — grouping evidence

Verified handoff identity:
`b944ca65517f2566aa872da2d217f03cc23c7e34e0d703c2e0bd92385fd799f2`.
The handoff reports byte identity and a passed full finalize gate. This review
changes notes only and does not rerun matching.

## New membership in the existing shared-prologue cluster

The verified body supports adding this function to the medium-confidence
`D_80053350/func_8001AC10` cluster at 0x8012ED18–0x80132934:

- `src/overlays/ovl_15/ovl_15_func_8012F300.c` opens with the cluster's
  text-draw idiom: one `D_8005E3C0->field_D8` load, offsets +0x18/+0x14,
  and `D_80054BBC[0]` added to an updater address, here `D_80053566`.
  Original assembly corroborates this at 0x8012F304–0x8012F330 in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012F300/ovl_15_func_8012F300.s`.
- The shared-state tie is more specific than a common draw helper:
  `src/overlays/ovl_15/ovl_15_func_8012F158.c`, already named in the
  cluster's evidence, populates the three halfword statuses in `D_80137820`.
  The target reads the selected status and stores the slot in `D_8013758E`,
  consumed by the listed `src/overlays/ovl_15/ovl_15_func_8012F8DC.c`.
  It also writes `D_80137584`, `D_80137588` and `D_8013758C`.
  The listed selector handlers `src/overlays/ovl_15/ovl_15_func_80130B04.c`
  and `src/overlays/ovl_15/ovl_15_func_80131AA4.c` share the status array,
  selected-slot store and cancellation-state pair. The target assembly
  shows its state accesses at 0x8012F35C–0x8012F3E8.
- Link placement corroborates these ties: `configs/splat/ovl_15.yaml`
  places `ovl_15_func_8012F158` at offset 0x1370, the target at 0x1518,
  and listed member `ovl_15_func_8012F3FC` at 0x1614. Their original
  assembly sizes are respectively 0x1A8 and 0xFC for the first two,
  establishing gapless adjacency to handlers with the same draw idiom
  and menu-state byte.

The combined shared-state, idiom and link-order evidence supports candidate
same-TU membership, not static linkage or exact original boundaries. Retain
medium confidence. Neither the new updater extern nor the call to
`ovl_15_func_80136B18(1)` establishes ownership of those symbols or justifies
merging the separate renderer/formatter cluster.
