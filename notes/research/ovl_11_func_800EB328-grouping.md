# ovl_11_func_800EB328 grouping evidence

## Candidate membership

Add `ovl_11_func_800EB328` to the existing `D_80129560` accessor family,
retaining its confidence and conditional same-TU premise. The new tie is
shared table/helper use corroborating adjacency to member `800EB010`, not
proof of an original translation-unit boundary.

Verified source/evidence identity supplied by the handoff:
`5c8f76ec06e0e588eca79e6484bb45d6a5abe3328e501595d70370d81df8e002`.

## Evidence

- `src/overlays/ovl_11/ovl_11_func_800EB328.c` reads `D_80129560[arg3]`
  and `[arg3 + 1]` as position inputs and `[arg1]`/`[arg2]` as angular
  state. It resolves the 0x22 position record through `800EFF04`, updates
  its three s32 coordinates, and passes a selected direction to family
  member `800E5B84`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800EB328/ovl_11_func_800EB328.s`
  confirms the resolver call at 0x800EB378, absolute `D_80129560` base
  and adjacent-slot loads at 0x800EB38C–0x800EB3C8, angular-slot loads at
  0x800EB3F4/0x800EB44C, and `800E5B84` call at 0x800EB6C4. Only arg1's
  table index is signed-halfword-normalized; arg2 and arg3 are s32 indices.
- The immediate predecessor, `src/overlays/ovl_11/ovl_11_func_800EB010.c`,
  also uses `D_80129560` and calls `800E5B84`. Its original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800EB010/ovl_11_func_800EB010.s`
  places that call at 0x800EB260 and gives its extent as 0x318 bytes:
  0x800EB010 + 0x318 = 0x800EB328. `configs/splat/ovl_11.yaml` likewise
  places the two functions consecutively at offsets 0x331F0 and 0x33508.
  Shared table and helper use therefore corroborate the zero-gap adjacency.

## Limits

An overlay-private table is not necessarily TU-private, and the adjacent
functions do not call each other. This supports membership in the existing
candidate family, not a newly proven same-file pair or a merge with the
resolver's own group. No shared register-variable quirk, declaration-order
effect, or SDK idiom cluster has been established by this function.
