# ovl_11_func_80114194 — candidate grouping evidence

Supplied verified source/evidence identity:
`a48aa9ec2e38e018bebb12a75513947d8e1c2a453c52c36b41429db3ac5259c0`
(origin: agent; byte-exact and full finalize gate reported passed).

## New evidence

The verified `src/overlays/ovl_11/ovl_11_func_80114194.c` initialises both
`D_8012D548` and `D_8012D5A8` through `ovl_11_func_8011D400`, after the
`ovl_11_func_80111F10` prologue. It fills their parallel pointer/halfword/byte
fields, gates two pointers on `D_800719F8`, then clears `D_8012D52C` and
returns 1. This adds a shared-record/helper fingerprint to the previously
recorded busy-flag write and dispatcher-table membership.

The original assembly in
`build/ovl_11/asm/nonmatchings/ovl_11_func_80113FF0/ovl_11_func_80113FF0.s`
and
`build/ovl_11/asm/nonmatchings/ovl_11_func_80114398/ovl_11_func_80114398.s`
shows the same prologue/helper pair, `D_8012D548` initialisation,
`D_80054BBC[1]`-based pointer formation and busy-flag reset on either side of
the target and its preceding reset stub. These are original-assembly facts;
the disabled, nonmatching 80113FF0 C draft is not used as proof.

`configs/splat/ovl_11.yaml` records the same link order. Original assembly
extents establish a zero-gap sequence:
`80113FF0 (0x194) → 80114184 (0x10) → 80114194 (0x204) → 80114398`.
The target extent is witnessed by
`build/ovl_11/asm/nonmatchings/ovl_11_func_80114194/ovl_11_func_80114194.s`;
the stub body is in `src/overlays/ovl_11/ovl_11_func_80114184.c`.

## Limits

Shared record/helper use plus local link adjacency supports candidate
membership in the ledger's existing D_8012D52C family, especially its earlier
neighbourhood. The busy flag alone is container-wide, and the common helpers
are callees, not demonstrated members of this original TU. No direct call
between these neighbours, private static ownership or declaration-order
constraint is established. A single original TU spanning this neighbourhood
and the later 8011A9CC–8011B6C0 run remains unproven; confidence is not raised.
