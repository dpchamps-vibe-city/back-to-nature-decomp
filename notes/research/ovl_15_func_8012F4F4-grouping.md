# ovl_15_func_8012F4F4 — grouping evidence

Verified handoff identity:
`88c32e2271f3a9ae7182a673ac5b9ae88fe329e0c5f46ad8eac1d82d41f4265a`.
The handoff reports byte identity and a passed full finalize gate. This review
changes notes only and does not rerun matching.

## New shared-state tie within the existing cluster

The medium-confidence `D_80053350/func_8001AC10` cluster at
0x8012ED18–0x80132934 already names this function as a `D_80052FC4`
shared-prologue member. That draw idiom is not new grouping evidence. The
verified body supplies a more specific tie to the listed adjacent handler:

- `src/overlays/ovl_15/ovl_15_func_8012F4F4.c` clears the halfword
  `D_8013759C` while setting `D_80137588 = 1` and `D_80137584 = 0xB`.
  Original assembly corroborates the stores at 0x8012F69C, 0x8012F6A8 and
  0x8012F6AC in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012F4F4/ovl_15_func_8012F4F4.s`.
- `src/overlays/ovl_15/ovl_15_func_8012F6C0.c` reads that same halfword
  to select its two substates, then advances it from zero to one after an
  input event. The original `lh` at 0x8012F6D0 and `sh` at 0x8012F750 are in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012F6C0/ovl_15_func_8012F6C0.s`.
  Both handlers also write `D_80137584` and use the cluster's draw idiom.
- Link placement corroborates the shared-state relationship:
  `configs/splat/ovl_15.yaml` places the target at offset 0x170C and
  `ovl_15_func_8012F6C0` at 0x18D8. The target's original 0x1CC-byte body
  ends exactly at the latter's 0x8012F6C0 entry. The 0x114-byte size belongs
  to `ovl_15_func_8012F6C0`, not to the target.

This initialization/consumption pair strengthens the existing same-TU
hypothesis beyond the previously recorded shared prologue and adjacency.
The inference assumes that the shared handler state and idiom reflect original
source organization; neither absolute global accesses nor adjacency prove
static linkage or exact TU boundaries. Retain medium confidence and the
existing group extent.
