# ovl_11_func_800E7088 grouping evidence

Verified source: `src/overlays/ovl_11/ovl_11_func_800E7088.c`.
Supplied verified source/evidence identity:
`2c72f771f3068ea612d65c7e668521cdffa024bfab959d15698694b7156c502c`.
The handoff reports byte identity and a passed full finalize gate, with
`include/globals_override.h` and the target source as its changed files.
This notes-only assessment does not rerun matching.

## Candidate membership

Extend the existing `D_80129560` accessor family and its candidate local
same-TU run `800E6EAC` / `800E6EE0` / `800E6F34` through `800E7088`.
The local same-TU tie remains low-confidence; no TU boundary is established.
See [the predecessor's evidence](ovl_11_func_800E6F34-grouping.md).

## Evidence

- **Gapless link order and a shared nearby callee agree.**
  `build/ovl_11/functions/ovl_11_func_800E6F34.s` gives the predecessor's
  extent as 0x154 bytes: 0x800E6F34 + 0x154 = 0x800E7088.
  `configs/splat/ovl_11.yaml` places them consecutively at offsets
  0x2F114 and 0x2F268, immediately after `800E6EE0` at 0x2F0C0.
  Both insertion functions call that nearby predicate: the predecessor
  at 0x800E6FAC/0x800E6FEC, and the target at 0x800E70D4 in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E7088/ovl_11_func_800E7088.s`.
  This extends the recorded local call/link cluster, not a direct
  caller/callee pair between `800E6F34` and `800E7088`.
- **Shared data and helper cluster corroborate adjacency.** The target's
  original instructions at 0x800E70E8–0x800E70F8 optionally read the low
  signed halfword of `D_80129560[arg0]` using a signed-halfword index and
  four-byte stride. The predecessor uses the same table/index pattern,
  but reads an unsigned halfword. Both read `D_8006C838+0x51E6`, call
  `func_8001F250(2, exponent + 1)`, scan six-byte item records, and use
  `ovl_11_func_800D6A94` and `ovl_11_func_800D7338` to insert an item.
  Their record arrays differ: the predecessor starts at +0x5288; the
  target checks +0x524C, scans +0x5258, then falls back to `D_80070EC2`.
  The verified sources and `include/globals_override.h` preserve these
  distinct storage views.

## Limits

`D_80129560` is overlay-private, not proven TU-private; `D_8006C838` is
shared engine storage. Neither the new reconstructed struct nor the
shared distant insertion helpers proves original file ownership. The
combined local adjacency, nearby predicate and data/helper cluster support
candidate membership only. No shared-static, GP-ownership, declaration-order,
register-variable or SDK-idiom witness was found, and no distant helper's
run is merged with this one.
