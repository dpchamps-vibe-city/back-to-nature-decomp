# ovl_11_func_8011266C grouping evidence

Verified handoff identity:
`e6be15e490dde4688608f57645930fe8fe8e9055007cc94acb90ad9816b1dc25`.
The supplied static-origin handoff reports byte identity and a passed full
finalize gate; its changed-file list contains only
`src/overlays/ovl_11/ovl_11_func_8011266C.c`. This assessment changes notes
only and does not rerun matching or alter build inputs.

## New membership evidence

Add the previously unassigned interior member to the existing medium-confidence
`D_800719F8` flag-word status group, not to a new group:

- The verified source clears bit 0x4000 of `D_800719F8`, calls
  `ovl_11_func_800E6EE0(0x1E, 0, 2, 0)`, and re-sets that bit only when the
  result is zero. The immediate neighbours' sources,
  `src/overlays/ovl_11/ovl_11_func_801125DC.c` and
  `src/overlays/ovl_11/ovl_11_func_801126C8.c`, refresh masks 0x3E00 and
  0x38000 of the same word. This fills the intervening bit in their status
  aggregation rather than merely sharing a widely used global.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8011266C/ovl_11_func_8011266C.s`
  confirms the 0x5C-byte extent and absolute flag-word accesses around the
  helper call. `build/ovl_11/functions/ovl_11_func_801125DC.s` gives the
  preceding extent as 0x90 bytes: 0x801125DC + 0x90 = 0x8011266C;
  0x8011266C + 0x5C = 0x801126C8. The three entries are consecutive in
  `configs/splat/ovl_11.yaml` at file offsets 0x5A7BC, 0x5A84C and 0x5A8A8.
- `src/overlays/ovl_11/ovl_11_func_80111F10.c` and the extracted
  `build/ovl_11/functions/ovl_11_func_80111F10.s` show the common dispatcher
  calling 801125DC at 0x80111F40, this member at 0x80111F48, and 801126C8
  at 0x80111F60. Calls to 80112440 and 80112494 intervene before the last
  call; the three neighbours nevertheless retain their relative link order.
  Thus shared data, coordinated bit roles and call/link order corroborate
  candidate membership.

## Limits

This member uses direct global accesses and is non-leaf; it does not exhibit
its neighbours' far-anchor arithmetic. The source and assembly do not supply
a shared-static, GP-ownership or declaration-order witness. The common flag
word and dispatcher support the existing medium-confidence candidate group,
not a proven original TU boundary. Neither the distant 800E6EE0 callee nor
the dispatcher is newly assigned to this TU by this evidence.
