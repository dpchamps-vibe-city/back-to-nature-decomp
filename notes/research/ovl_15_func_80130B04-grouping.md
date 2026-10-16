# ovl_15_func_80130B04 — grouping evidence

Verified handoff identity:
`f46f081e435b12131d6ba2435f7548ae69889fe92790574f82153b561509ee9f`.
The handoff reports byte identity and a passed full finalize gate. This review
changes notes only and does not rerun matching.

## New membership in the existing shared-prologue cluster

The verified body supports adding this function to the medium-confidence
`D_80053350/func_8001AC10` cluster at 0x8012ED18–0x80132934:

- `src/overlays/ovl_15/ovl_15_func_80130B04.c` opens with the cluster's
  text-draw idiom: one `D_8005E3C0->field_D8` load, offsets +0x18/+0x14,
  and `D_80054BBC[0]` added to an updater address, here `D_80053596`.
  The original assembly corroborates this at 0x80130B08–0x80130B34 in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_80130B04/ovl_15_func_80130B04.s`.
- Beyond that idiom, the target reads `D_80137820[slot]`, the three-halfword
  slot-state array populated by the already-listed
  `src/overlays/ovl_15/ovl_15_func_801308B8.c`. It writes the selected slot to
  `D_8013758E`, which the listed
  `src/overlays/ovl_15/ovl_15_func_80130C2C.c` passes to
  `ovl_15_func_80137544`; all three handlers write `D_80137584`. The target
  also writes `D_8013758C` on cancellation. Its assembly shows the state
  accesses at 0x80130B64–0x80130BA8. These are shared-state ties, not merely
  calls to a common helper.
- Link placement agrees: `configs/splat/ovl_15.yaml` places the target at
  offset 0x2D1C between listed members `ovl_15_func_80130A94` (0x2CAC) and
  `ovl_15_func_80130BBC` (0x2DD4). The predecessor is 0x70 bytes and the
  target is 0xB8 bytes, giving gapless adjacency; both neighbours use the
  same draw idiom and menu-state byte.

The combined idiom, shared-state and link-order evidence supports candidate
same-TU membership, not exact original boundaries or static linkage. Retain
medium confidence. Neither the new updater extern nor the call to
`ovl_15_func_80136B18` justifies merging the separate renderer/formatter
cluster into this one.
