# ovl_11_func_800E3200: keyframe-state handler membership

## Verified premise

The supplied agent handoff records byte identity and the full finalize gate
passing for evidence identity
`6c9df7ad42f9173afe9c3aae30faa9f8e07cbf89aa668772b6d3603f77f6dc34`.
The source at `src/overlays/ovl_11/ovl_11_func_800E3200.c` has SHA-256
`c65b92cbba54b657b91e9e9e024fb2d9940d1a87ce4df0d0ae7f13713d55377d`.
This documentation does not rerun matching or change build inputs.

## New membership evidence

The ledger previously mentioned the target only as a non-member predecessor
of `ovl_11_func_800E32E4`. The verified handler now supports membership in the
keyframe-state family and a candidate same-TU pair with that neighbour
(medium confidence for original TU membership):

- **Code order agrees with table order.** `configs/splat/ovl_11.yaml` places
  the target at offset `0x2B3E0`; its `0xE4` bytes end at `800E32E4`, offset
  `0x2B4C4`. In `build/ovl_11/asm/data/69960.data.s`, its three-word table
  `D_801240BC` (`0x001E0004, 0x00000000, 0x0000FFFF`) ends exactly at the
  neighbour's `D_801240C8`.
- **Shared dispatch cluster.** `build/ovl_11/asm/data/1740.rodata.s` puts the
  target in `D_800B970C[4]` and the neighbour in `[14]`, alongside the already
  grouped `800E2F58`, `800E3020` and `800E3464` handlers.
- **Shared handler/callee pattern.** The verified source and
  `src/overlays/ovl_11/ovl_11_func_800E32E4.c` use the same state at +0x28,
  timer at +0x2A, flag word at +0x34, and
  `800D04D4(obj, table, &state, -1, &timer)` call. The extracted target
  assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E3200/ovl_11_func_800E3200.s`
  independently shows those offsets and the family state-switch shape.

## Role and limits

The target steps `D_801240BC`; while it is active, it preserves +0x2A across
`800D12B8(obj, 0x12, 0x31)`. Its terminal arm sets +0x2C to 300 and clears
flags 0x800 and 0x2000.

Shared helpers and object layout alone establish a family, not a source file.
The matching code/table adjacency and named dispatch cluster support the local
same-TU hypothesis, conditional on those orders retaining original layout.
No static ownership, declaration-order effect or register-variable quirk is
established. This does not place the helpers or every distant family member
in the same original TU.
