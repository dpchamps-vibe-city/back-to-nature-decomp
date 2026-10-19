# ovl_11_func_8011282C grouping evidence

Verified handoff identity:
`6161c589a324c606b2e4cfa51a24d3e52c9daf724a6657559b95e8e302a8aeac`.
The supplied static-origin handoff reports byte identity and a passed full
finalize gate; its changed-file list contains only
`src/overlays/ovl_11/ovl_11_func_8011282C.c`. This assessment changes notes
only; it does not rerun matching or alter build inputs.

## New membership evidence

Extend the existing medium-confidence `D_800719F8` flag-word status group
through 8011282C, including its intervening assembly-only neighbour 8011277C:

- The verified source clears mask 0x600000 of `D_800719F8`, then refreshes
  bits 0x200000 and 0x400000 from zero results of
  `ovl_11_func_800E6EE0(0x1F, 0, 2, 0)` and `(0x20, 0, 2, 0)`.
  `src/overlays/ovl_11/ovl_11_func_8011266C.c`, already assigned to the
  group, uses the same clear/query/set pattern for bit 0x4000 and ID 0x1E.
- `build/ovl_11/functions/ovl_11_func_8011277C.s` shows the intervening
  member clearing mask 0x1C0000 of the same word and refreshing bits
  0x40000, 0x80000 and 0x100000 using the same helper and arguments
  `(id, 0, 2, 0)` for IDs 0x1C, 0x1A and 0x1B. Its mask follows the
  preceding 801126C8 member's 0x38000 mask and immediately precedes
  8011282C's 0x600000 mask. These are coordinated status-bit assignments,
  not merely unrelated references to a widely shared global.
- That assembly gives 8011277C an extent of 0xB0 bytes, ending exactly at
  0x8011282C. The target assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8011282C/ovl_11_func_8011282C.s`
  gives an extent of 0x88 bytes and confirms the flag/helper accesses.
  `configs/splat/ovl_11.yaml` lists 801126C8, 8011277C and 8011282C
  consecutively at offsets 0x5A8A8, 0x5A95C and 0x5AA0C, corroborating
  their gapless link order.
- `src/overlays/ovl_11/ovl_11_func_80111F10.c` and the extracted
  `build/ovl_11/functions/ovl_11_func_80111F10.s` show the common dispatcher
  ending with calls to 801126C8, 8011277C and 8011282C, at instruction
  addresses 0x80111F60, 0x80111F68 and 0x80111F70. Their call order agrees
  exactly with their link order and complementary flag-word roles.

## Limits

This supports an extension of the existing candidate TU group, not a proven
original source-file boundary. The target uses direct absolute global accesses;
it does not establish the far-anchor idiom recorded for other group members,
a shared static, GP ownership or a declaration-order constraint. The 8011277C
role is inferred from extracted assembly, not a newly verified C function.
Neither the distant 800E6EE0 helper nor the shared dispatcher is newly assigned
by this evidence. Although 8011282C ends at 801128B4, adjacency alone does not
merge the following work-area reset group into this status group.
