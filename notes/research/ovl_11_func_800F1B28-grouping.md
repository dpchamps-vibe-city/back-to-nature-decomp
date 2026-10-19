# ovl_11_func_800F1B28 grouping evidence

Verified-source/evidence identity supplied by the handoff:
`d0242dac87c556b5faa80ee36cfc494286acc6cc0200a02b0de8faf4c01bb866`.
The handoff reports byte-exact matching and the full finalize gate complete;
this is documentation only.

## Evidence

- `src/overlays/ovl_11/ovl_11_func_800F1B28.c` passes the unchanged mask
  argument to `ovl_11_func_800F1BD0`, `ovl_11_func_800F1C48`, then
  `ovl_11_func_800F1CC4`, returning zero if any rejects it. These are the
  three members of the existing mask-switch leaf group.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800F1B28/ovl_11_func_800F1B28.s`
  gives the caller a size of 0xA8: 0x800F1B28 + 0xA8 = 0x800F1BD0.
  Its calls at 0x800F1B34, 0x800F1B44 and 0x800F1B54 confirm that the
  leaf-call sequence agrees with link order.
- `configs/splat/ovl_11.yaml` places the caller and the three leaves at
  consecutive offsets 0x39D08, 0x39DB0, 0x39E28 and 0x39EA4. The new
  caller therefore extends the existing gapless leaf run at its head,
  with a direct call to every existing member rather than adjacency alone.

## Inference and limits

This supports adding 800F1B28 to the existing **low-confidence candidate
same-TU group**. Call/link adjacency is evidence of membership, not proof
of an original object boundary. No shared static, declaration-order effect
or register-variable quirk was established by this function.

The optional calls to `ovl_11_func_800E6EE0` for mask bits 0x20000 and
0x10000 do not establish same-TU membership with that distant helper or
merge its accessor family into this group. Nor does this finding extend
the group to adjacent 800F1AE0 or 800F1D54: no corresponding call/data tie
was established here.
