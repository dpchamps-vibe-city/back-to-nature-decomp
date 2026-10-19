# ovl_11_func_800E7A74 grouping evidence

Verified handoff identity:
`260881b5522a9f1a8b026f5e28e7eb302645d2d837dd1ecffa385f6bad324e5f`.
The supplied handoff reports byte identity and a passed full finalize gate;
its changed-file list is `configs/splat/ovl_11.yaml` and
`src/overlays/ovl_11/ovl_11_func_800E7A74.c`. This notes-only assessment
relies on that gate and does not rerun matching.

## Candidate membership

Extend the existing `D_80129560` accessor family with the target and record
a low-confidence local same-TU pair with `ovl_11_func_800E7B78`:

- `src/overlays/ovl_11/ovl_11_func_800E7A74.c` optionally resolves its ID
  from the signed low halfword of `D_80129560[arg0]`, then starts an
  operation through `ovl_11_func_80107C80` or tests the state returned by
  `ovl_11_func_8010734C`. `include/globals_override.h` records the shared
  overlay-private 0x50-byte s32 table and sibling accessors.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E7A74/ovl_11_func_800E7A74.s`
  confirms the absolute table base at 0x800E7A98/0x800E7A9C, a signed-halfword
  index scaled by four, and the `lh` at 0x800E7AA8. Its 0x104-byte extent
  ends exactly at 0x800E7B78.
- `src/overlays/ovl_11/ovl_11_func_800E7B78.c` forwards
  `(arg0, 4, 0, 0)` to the target, using the raw-ID state-test path and
  discarding the result. `build/ovl_11/functions/ovl_11_func_800E7B78.s`
  confirms the direct call at 0x800E7B90.
- `configs/splat/ovl_11.yaml` places target and wrapper consecutively at
  file offsets 0x2FC54 and 0x2FD58. Thus the call graph corroborates
  gapless link adjacency; the wrapper does not itself access the table.

## Limits

`D_80129560` is overlay-private, not proven TU-private. Shared table use
supports candidate accessor-family membership, while the direct wrapper
edge and adjacency support the local pair; neither establishes original TU
boundaries. No shared-static, GP-ownership, declaration-order or compiler-quirk
witness was found. The distant `80107C80`/`8010734C` callees are dependencies,
not grounds to merge their recorded groups with this pair. The jump-table
split in the handoff identifies this function's rodata, not a TU boundary.
