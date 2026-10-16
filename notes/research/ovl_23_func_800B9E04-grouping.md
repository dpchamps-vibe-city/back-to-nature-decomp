# ovl_23_func_800B9E04: vector-helper client grouping evidence

Documentation-only handoff: the target is byte-exact and passed the full
finalize gate. Supplied source/evidence identity:
`f70e102b809c29ce6347ae139b7088b7e4c653d996d99073abb943e64d5ad230`
(origin: agent). The supplied changed-file list is
`configs/splat/ovl_23.yaml`, `include/globals_override.h`,
`include/overlays/ovl_23.h`, and
`src/overlays/ovl_23/ovl_23_func_800B9E04.c`. This pass edits only notes.

## New membership evidence

- **Shared concrete record/table accesses with the next member:** the first
  six-entry scan reads `s32@D_800BF87C+0x210+i*0x44` and
  `s16@D_800BBA40+i*2`. The adjacent `ovl_23_func_800BA1E0` reads exactly
  that pair, indexed by the argument record's s16 selector at +0. Both use
  the argument's s32 positions at +0x14/+0x18, divide positions by 4096
  with signed truncation, and subtract the selected coordinates. This is
  the same concrete coordinate cluster, not just a shared recovered type.
- **Shared helper composition:** both send those differences through
  `ovl_23_func_800BA368` and pass its result directly to
  `ovl_23_func_800BA278`. The target also calls the existing group's distance
  helper `ovl_23_func_800BA30C` before each direction comparison. It therefore
  connects to all three math leaves already listed in the group.
- **Link order corroborates the call/data evidence:** the target occupies
  0x3DC bytes at 0x800B9E04 and ends exactly at 0x800BA1E0. The consecutive
  splat offsets are 0x1FE4 and 0x23C0. This extends the existing gapless run
  through 800BA278, 800BA30C and 800BA368; it does not infer membership from
  adjacency alone.

Original-assembly anchors:

- Target: 0x800B9FB0–0x800B9FC0 establishes the table and aggregate +0x210
  bases; 0x800BA070/0x800BA078 calls the angle/selector pair;
  0x800BA088–0x800BA09C advances by 2/0x44 for six entries. The distance
  calls are at 0x800BA018 and 0x800BA124. The last instruction is
  0x800BA1DC.
- 800BA1E0: 0x800BA200–0x800BA218 selects the same 0x44-stride field;
  0x800BA240–0x800BA250 selects the corresponding table halfword;
  0x800BA258/0x800BA260 calls the same angle/selector pair.

## Per-function detail retained outside the ledger

The target first rejects a projected y coordinate outside the selector's
`D_800BBA4C` bounds. Its two six-entry scans exclude the argument's own
index and reject a matching direction when distance is within the supplied
limit. The second scan reads `D_800BF87C+0x38/+0x3C+i*0x50`, corresponding
to positions +0x14/+0x18 of the records beginning at `D_800BF8A0`.

The previous helper-run ledger detail is retained here: 800BA1E0 selects
one +0x210/0x44 coordinate and table y coordinate and returns the classified
direction. 800BA278 classifies angles with 0x2D-wide intervals into indices
0–7 (fallback 4). 800BA30C takes `SquareRoot0` of the squared s16 coordinate
deltas and returns the s16-truncated result. 800BA368 takes `ratan2`, wraps
negative results by +0x1000 and converts to degrees with `*0x168/0x1000`.
The existing helper sizes are 0x98, 0x94, 0x5C and 0x4C respectively, forming
a gapless run ending at 0x800BA3B4. The legacy ledger's description of both
math helpers as taking s16 deltas should not imply an s16 signature for
800BA368: its current source parameters are s32.

## Evidence paths

- `src/overlays/ovl_23/ovl_23_func_800B9E04.c`
- `src/overlays/ovl_23/ovl_23_func_800BA1E0.c`
- `src/overlays/ovl_23/ovl_23_func_800BA278.c`
- `src/overlays/ovl_23/ovl_23_func_800BA30C.c`
- `src/overlays/ovl_23/ovl_23_func_800BA368.c`
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B9E04/ovl_23_func_800B9E04.s`
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800BA1E0/ovl_23_func_800BA1E0.s`
- `include/globals_override.h` (`Ovl23DecisionView` and the
  `Ovl23_D_800BF87C_movement` alias) and `configs/splat/ovl_23.yaml`.

## Inference and limits

Add 800B9E04 to the existing vector-math candidate group, retaining medium
confidence. Shared coordinate storage, the identical helper composition and
gapless placement support suspected same-TU membership. They do not prove
static linkage or exact original TU boundaries. The recovered aggregate
view/alias and the target's new jump-table split are not evidence of original
C declarations. No compiler-flag or register-allocation premise is asserted,
and this pass does not merge other users of the large D_800BF87C aggregate.
