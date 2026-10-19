# ovl_11_func_8011DAD4 — capacity-query/insertion pair

The handoff reports byte-exact source and a passed full finalize gate for
`src/overlays/ovl_11/ovl_11_func_8011DAD4.c`, with source/evidence identity
`2d8eefc0bd6aed0dfea0653c7818facd02c52d5e8704733e5ff1d80b3bca4c7d`.
This note records grouping evidence only; no matching experiments were run.

## New grouping evidence

The verified insertion routine supports a low-confidence same-TU pairing with
its immediate predecessor, the capacity query `ovl_11_func_8011D9DC`, already
listed at the tail of the candidate by-value struct-slice run.

- **Call graph agrees with link order.** The original target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8011DAD4/ovl_11_func_8011DAD4.s`
  calls `8011D9DC(id, 1)` at `0x8011DB1C` and compares its result with the
  requested quantity. `configs/splat/ovl_11.yaml` places D9DC at file offset
  `0x65BBC`, DAD4 at `0x65CB4`, and DD48 at `0x65F28`.
  The original D9DC target is `0xF8` bytes, so
  `0x8011D9DC + 0xF8 = 0x8011DAD4`: no intervening code.
- **Paired query/mutation over the same records.** D9DC's original target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8011D9DC/ovl_11_func_8011D9DC.s`
  reads the signed halfword at `D_8006C838 + 0x51E6` and calls
  `func_8001F250(2, field + 1)` at `0x8011DA38`. DAD4 does the same at
  `0x8011DB48`. Both process the record at `D_8006C838 + 0x524C`, then
  `D_80071A90` records with a six-byte stride, treating +0 as the item ID
  and +4 as its quantity. D9DC sums available capacity using the 99-item
  cap; DAD4 fills those same stacks under that cap. This is a specific
  query/insertion pairing, not just a shared reference to the large resident
  state block.

## Limits

This supports the D9DC/DAD4 pair as a candidate, not a proven original source
boundary or a stronger claim for the entire by-value run. Shared resident
records and direct calls can cross translation units; no private static,
GP-relative ownership, declaration-order effect or shared register-variable
quirk was established here. D9DC remains an assembly-backed source stub.

DAD4 ends exactly at `0x8011DD48` (`0x8011DAD4 + 0x274`), but that adjacency
alone does not join the following range-map/store-view run to this pair.
Likewise, the distant record-writing/notification callees `800D7338` and
`800D6A94`, and other users of the resident item tables, are not added merely
because they share a call or data reference.
