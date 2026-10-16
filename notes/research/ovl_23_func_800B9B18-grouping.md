# ovl_23_func_800B9B18: movement client of the vector-helper run

Documentation-only handoff: the supplied target is byte-exact and passed the
full finalize gate. Supplied source/evidence identity:
`f386f3dc8e22e14dc9e98becdb68b910022705926ffc4865de0282126ab3c94e`
(origin: agent). Supplied changed files: `configs/splat/ovl_23.yaml`,
`include/globals_override.h`, `include/overlays/ovl_23.h`, and
`src/overlays/ovl_23/ovl_23_func_800B9B18.c`. This pass changes only notes;
it does not rerun matching or alter the verified inputs.

## New membership evidence

- **Shared clearance call and concrete threshold:** the target calls
  `ovl_23_func_800B9E04` with its argument record, the s16 direction at +6,
  and the s16 at `D_800BF87C+0x39E`. Its immediate link successor
  `ovl_23_func_800B9CCC` calls that same helper with the same record's
  selected direction and a distance derived from that exact halfword
  (`field * 3 / 2`, narrowed to s16). Both treat return value 1 as clearance.
  This identifies a specific shared field/callee protocol, not just two
  users of the overlay-wide aggregate or a recovered type.
- **Matching eight-direction geometry:** the target updates positions at
  argument +0x14/+0x18, using `speed * 1000 / 1414` on diagonals. The
  clearance helper projects the same +0x18 position using that same ratio
  and direction encoding: 0 adds y, 4 subtracts y, 1/7 add diagonal y,
  and 3/5 subtract diagonal y. The movement client and the existing helper
  therefore share a concrete coordinate convention and arithmetic idiom.
- **Gapless link order corroborates the call/data evidence:** the original
  target is 0x1B4 bytes at 0x800B9B18, ending exactly at 0x800B9CCC.
  Consecutive splat text offsets are 0x1CF8, 0x1EAC and 0x1FE4 for
  `800B9B18`, `800B9CCC` and `800B9E04`. This extends the existing
  helper/client run backward, rather than creating a separate group from
  adjacency alone.

Original-assembly anchors: `800B9B18` loads the aggregate's +0x39E at
0x800B9BB4 and calls `800B9E04` at 0x800B9BC8; its direction cases occupy
0x800B9C10–0x800B9C58 and its final instruction is at 0x800B9CC8.
`800B9CCC` loads the same halfword at 0x800B9D7C and calls the shared
clearance helper at 0x800B9D98.

## Supporting connection and limits

The target also shares `(D_800BF87C.unk3B2 << 12) / 10` and the x limit
`(D_800BF87C.unk39C + 0x37) << 12` with the existing movement-clamp member
`ovl_23_func_800B989C`, operating on the same argument offsets +0x14/+0x18.
This supports a connection to that family, but this update does not merge
its other members or all consumers of `D_800BF87C` into the helper run.

Add `800B9B18` to the existing vector-helper/client candidate group,
retaining medium confidence. The shared threshold/callee protocol,
direction arithmetic and contiguous placement support suspected same-TU
membership; they do not establish static linkage or exact TU boundaries.
The new bounds view/alias and jump-table split are reconstruction devices,
not evidence of original C declarations or source-file boundaries. No
compiler-flag, declaration-order or register-allocation premise is asserted.

## Evidence paths

- `src/overlays/ovl_23/ovl_23_func_800B9B18.c`
- `src/overlays/ovl_23/ovl_23_func_800B9CCC.c`
- `src/overlays/ovl_23/ovl_23_func_800B9E04.c`
- `src/overlays/ovl_23/ovl_23_func_800B989C.c`
- `include/globals_override.h` (`Ovl23MovementBounds` and its
  `Ovl23_D_800BF87C_bounds` alias of `D_800BF87C`).
- `configs/splat/ovl_23.yaml` (consecutive text entries).
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B9B18/ovl_23_func_800B9B18.s`
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B9CCC/ovl_23_func_800B9CCC.s`
- [Existing helper/client evidence](ovl_23_func_800B9E04-grouping.md) and
  [previous run extension](ovl_23_func_800B9CCC-grouping.md).
