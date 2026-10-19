# ovl_11_func_800E6F34 grouping evidence

Verified source: `src/overlays/ovl_11/ovl_11_func_800E6F34.c`.
Supplied verified source/evidence identity:
`0f50ac5f1c0b5bb0eb7902c416eba852fb35af9e0cc9fbd5833dda3e71898cd4`.
The handoff reports byte identity and a passed full finalize gate, with only
that source in its changed-file list. This notes-only assessment does not
rerun matching.

## Candidate membership

Add the target to the existing `D_80129560` accessor family, with a
low-confidence local same-TU tie to `ovl_11_func_800E6EE0`. This extends
the candidate local pair recorded in
[the predecessor's evidence](ovl_11_func_800E6EE0-grouping.md) to
`800E6EAC` / `800E6EE0` / `800E6F34`; it does not establish a TU boundary.

## Evidence

- **Call graph and gapless link order agree.**
  `build/ovl_11/functions/ovl_11_func_800E6EE0.s` gives the predecessor's
  extent as 0x54 bytes: 0x800E6EE0 + 0x54 = 0x800E6F34.
  `configs/splat/ovl_11.yaml` places the two consecutively at file offsets
  0x2F0C0 and 0x2F114. The target's verified source calls that predecessor
  in both insertion paths; original instructions in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E6F34/ovl_11_func_800E6F34.s`
  confirm the calls at 0x800E6FAC and 0x800E6FEC. This supplies a new
  matched caller/callee edge at the existing local pair's upper end.
- **Shared slot-table access corroborates the local tie.** The target's
  original instructions at 0x800E6F60–0x800E6F70 form the absolute
  `D_80129560` base, scale a signed-halfword index by four, and read the
  slot's low unsigned halfword when `arg1 != 0`. The predecessor reads
  the same table through a signed-halfword index and four-byte stride,
  but uses `lh`, not `lhu`, under its own option-bit guard. The common
  table is recorded in `include/globals_override.h`; this is a shared
  global/accessor tie, not an identical signedness or compiler-quirk claim.

## Limits

The table is overlay-private, not proven TU-private. The new call/link
edge supports candidate local membership only; there is no shared-static,
GP-ownership, declaration-order or register-variable witness. The distant
`800CB540`, `800D6A94` and `800D7338` dependencies are not grounds to merge
their runs with this one. The shared `D_8006C838` item-grid view describes
storage, not original file ownership. No TU boundary or SDK idiom cluster
is inferred.
