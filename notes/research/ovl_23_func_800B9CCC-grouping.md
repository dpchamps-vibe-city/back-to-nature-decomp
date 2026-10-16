# ovl_23_func_800B9CCC: vector-helper run extension

Documentation-only handoff: the supplied target is byte-exact and passed the
full finalize gate. Supplied source/evidence identity:
`a0a824be5848f8a851f1f98fe1a0b8485122b6d39efdcd5168d7e113ac5b625a`
(origin: agent). Supplied changed files: `include/overlays/ovl_23.h` and
`src/overlays/ovl_23/ovl_23_func_800B9CCC.c`. This pass changes only notes.

## New membership evidence

- **Same concrete coordinate/table pair:** the target selects
  `s32@D_800BF87C+0x210+i*0x44` and `s16@D_800BBA40+i*2`, with `i` from
  the argument's s16 selector at +0. It subtracts these from the argument's
  +0x14/+0x18 positions after signed division of fixed-point positions by
  4096. These are exactly the accesses in existing member `800BA1E0` and
  the coordinate cluster scanned by `800B9E04`, not merely a shared
  recovered type or an unrelated use of the large aggregate.
- **Direct client of both adjacent members:** `800B9CCC` calls `800BA1E0`
  to classify direction, then passes the same argument and that direction
  to `800B9E04` for clearance checking. These original calls are at
  0x800B9D6C and 0x800B9D98.
- **Corroborating gapless link order:** the target is 0x138 bytes at
  0x800B9CCC and ends exactly at 0x800B9E04, extending the existing
  helper/client run backward. Splat offsets 0x1EAC and 0x1FE4 agree with
  the original assembly and current link map.

Original-assembly anchors: 0x800B9CF8–0x800B9D10 selects the +0x210 field
with stride 0x44; 0x800B9D3C–0x800B9D4C selects the table halfword. The last
instruction is at 0x800B9E00.

## Evidence paths and limits

- `src/overlays/ovl_23/ovl_23_func_800B9CCC.c`
- `src/overlays/ovl_23/ovl_23_func_800B9E04.c`
- `src/overlays/ovl_23/ovl_23_func_800BA1E0.c`
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B9CCC/ovl_23_func_800B9CCC.s`
- `configs/splat/ovl_23.yaml` (offsets 0x1EAC, 0x1FE4, 0x23C0)
- `build/ovl_23/ovl_23.map` (consecutive text ranges)
- [Existing helper/client evidence](ovl_23_func_800B9E04-grouping.md)

The shared concrete storage and direct calls, corroborated by link order,
support adding the target to the existing candidate group at medium
confidence. They do not prove static linkage or exact original TU boundaries.
The recovered views and declarations are not evidence of original C types;
no compiler-flag or register-allocation premise is asserted. This does not
merge other users of `D_800BF87C` into the group.
