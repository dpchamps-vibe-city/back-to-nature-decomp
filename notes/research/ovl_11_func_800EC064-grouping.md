# ovl_11_func_800EC064 grouping evidence

Verified source/evidence identity supplied by the handoff:
`75639e32c53607c366a5a0d77032048106673ea3cec62a9de0576b347911a786`.
The handoff reports byte identity and a passed full finalize gate. This
assessment changes notes only and does not rerun matching.

## Candidate membership

Add `ovl_11_func_800EC064` to the existing `D_80129560` accessor family.
The new reader and its shared-table adjacency support candidate same-TU
membership at low confidence, not a proven original translation unit.

- Verified source: `src/overlays/ovl_11/ovl_11_func_800EC064.c` optionally
  resolves the object index from the signed low halfword of
  `D_80129560[arg1]` when `arg3 != 0`. The source then selects an object
  pool and updates flag `0x400` or dispatches a stage sub-state change.
- Original assembly:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800EC064/ovl_11_func_800EC064.s`.
  Addresses 0x800EC074–0x800EC090 confirm the signed-halfword slot index,
  absolute table base, four-byte stride and `lh`. This is the same
  conditional low-halfword slot-resolution idiom used by recorded family
  member `800E6EE0`; see its verified source and
  [grouping evidence](ovl_11_func_800E6EE0-grouping.md).
- The original direct call at 0x800EC2B0 to `ovl_11_func_800E6EE0` supplies
  a member-to-member call edge corroborating that shared-table tie.
- `configs/splat/ovl_11.yaml` places the target at offset 0x34244 and family
  member `800EC304` at 0x344E4. The target's original 0x2A0-byte extent
  ends exactly at `800EC304`, whose verified source also consumes
  `D_80129560`. This adds a shared-table predecessor to the already recorded
  `800EC304` / `800EC354` / `800EC490` link-order run. There is no direct
  call between the target and `800EC304`.

## Limits

`D_80129560` is overlay-private, not proven TU-private; the distant
`800E6EE0` call does not establish a common TU by itself. No original TU
boundary, shared static, declaration-order effect, register-variable quirk
or SDK cluster is established. The EXE-owned object pools and the call to
`8010941C` do not justify merging their separate recorded groups into this
accessor family. The newly assigned function-local jump-table rodata slice
in the splat configuration is not an original TU boundary witness.
