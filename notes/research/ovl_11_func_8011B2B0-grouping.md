# ovl_11_func_8011B2B0 — candidate grouping evidence

Supplied verified source/evidence identity:
`794aeaf636c840b40a60f851d8b5714886061a7e7e0b07131893554969b271f1`
(origin: agent; byte-exact and full finalize gate reported passed).

## New evidence

The verified [source](../../src/overlays/ovl_11/ovl_11_func_8011B2B0.c)
initialises `D_8012D548` and `D_8012D5A8` through
`ovl_11_func_8011D400`, populates their parallel pointer/halfword/byte fields
with flag-gated entries, then clears `D_8012D52C` and returns 1.

This adds a shared-record/helper fingerprint beyond the previously recorded
reset ending and link position: the immediately preceding
[8011B210](../../src/overlays/ovl_11/ovl_11_func_8011B210.c) initialises the
same `D_8012D548` record through the same helper. Nearby
[8011AFB4](../../src/overlays/ovl_11/ovl_11_func_8011AFB4.c) additionally shares
`D_800719F8`-gated entries and `D_80054BBC[1]`-based text-pointer formation.
The [overlay split configuration](../../configs/splat/ovl_11.yaml) places
B210, B2B0 and B6B4 at offsets 0x633F0, 0x63490 and 0x63894, corroborating
the ledger's existing zero-gap sequence
`8011B210 (0xA0) → 8011B2B0 (0x404) → 8011B6B4`.

The two-record/helper combination also occurs in
[80114194](../../src/overlays/ovl_11/ovl_11_func_80114194.c), an earlier
candidate in the same ledger family; see its
[grouping note](ovl_11_func_80114194-grouping.md).

## Limits

Shared record/helper use plus local link adjacency supports candidate
membership in the existing reset-stub family, not proven original-TU
ownership. The busy flag is container-wide; shared callees are not thereby
members of this TU. Neither private-static ownership nor a declaration-order
constraint is established. A single TU joining the earlier 80114194
neighbourhood to this later run remains unproven; confidence is unchanged.
