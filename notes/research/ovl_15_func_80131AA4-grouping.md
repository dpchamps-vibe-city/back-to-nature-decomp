# ovl_15_func_80131AA4 — grouping evidence

Verified handoff identity:
`41d965e3b9d9ef91aba4bc41f0d48547978426288e4c494cfbf84871922d26ff`.
The handoff reports byte identity and a passed full finalize gate. This review
changes notes only and does not rerun matching.

## Support for membership in the existing shared-prologue cluster

The verified body supports the recorded membership in the medium-confidence
`D_80053350/func_8001AC10` cluster at 0x8012ED18–0x80132934:

- `src/overlays/ovl_15/ovl_15_func_80131AA4.c` shares the text-draw idiom
  and selection logic of the existing member
  `src/overlays/ovl_15/ovl_15_func_80130B04.c`: a single
  `D_8005E3C0->field_D8` load, offsets +0x18/+0x14, updater base
  `D_80053596` plus `D_80054BBC[0]`, then `ovl_15_func_80136B18(0)`.
  This connects another member to that updater, rather than introducing a
  new updater symbol. See [the 80130B04 note](ovl_15_func_80130B04-grouping.md).
- The shared-state tie is stronger than body similarity alone: the target
  reads `D_80137820[slot]`, populated by the existing member
  `src/overlays/ovl_15/ovl_15_func_80131858.c`, and writes the selected slot
  to `D_8013758E`, consumed by the existing member
  `src/overlays/ovl_15/ovl_15_func_80131BCC.c`. All three write
  `D_80137584`; the target also writes `D_8013758C` on cancellation.
  Original assembly corroborates the draw setup at 0x80131AA8–0x80131AD4
  and state accesses at 0x80131B00–0x80131B48 in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_80131AA4/ovl_15_func_80131AA4.s`.
- `configs/splat/ovl_15.yaml` places the target at offset 0x3CBC between
  listed members `ovl_15_func_80131A34` (0x3C4C) and
  `ovl_15_func_80131B5C` (0x3D74). The predecessor occupies 0x70 bytes
  and the target 0xB8 bytes, giving gapless link adjacency. Their sources
  in `src/overlays/ovl_15/` share the draw idiom and `D_80137584` writes.
  Placement corroborates the shared-state evidence; adjacency alone is
  not the membership premise.

These ties support candidate same-TU membership, not static linkage or exact
original boundaries. Retain medium confidence. The common selector call does
not justify merging the separate renderer/formatter cluster.
