# ovl_11_func_8011EA0C: translation-unit grouping evidence

The supplied handoff records byte-exact verification and a passed full finalize
gate under source/evidence identity
`f0185eb04a40754243c31bbde52e3f0aa952a1148b0bcf5405a8edfd19256b01`.
The reviewed source is `src/overlays/ovl_11/ovl_11_func_8011EA0C.c`
(SHA-256 `a9d84e727a61fb1c9d6aed72a7355114829aa41fa9ebe2018cb39f763b1bea90`).
`build/experimentLedger/ovl_11_func_8011EA0C.jsonl` records that source
hash with `exact: true` and residual key `[0,0,0,0]`. Verification was
supplied, not rerun for this notes-only role.

## New evidence

- **Shared state/table controller and renderer:** the verified handler reads
  and writes both `D_80128420` and `D_80128422`. Directional input selects
  a byte at `D_801284AC + D_80128422 * 8 + 4 + direction`, storing it back
  to `D_80128422`, before calling `ovl_11_func_8011ECC4`. That existing
  cluster member uses the same selection and 8-byte-stride table to draw
  the cursor from the halfwords at record offsets +0/+2. Evidence:
  `src/overlays/ovl_11/ovl_11_func_8011EA0C.c` and
  `src/overlays/ovl_11/ovl_11_func_8011ECC4.c`; the original handler's
  table lookup/store is at 0x8011EAB8–0x8011EAD8 in
  `build/ovl_11/functions/ovl_11_func_8011EA0C.s`.
- **Call graph agrees with gapless link order:** the handler's 0x248 bytes
  end exactly at 0x8011EC54, its state-8 helper and an existing member.
  Its other local callee, 8011ECC4, follows the existing 8011EC54 →
  8011EC84 → 8011ECC4 run. `configs/splat/ovl_11.yaml` gives the
  consecutive offsets 0x66BEC, 0x66E34, 0x66E64 and 0x66EA4;
  `build/ovl_11/functions.csv` corroborates sizes and both call edges.

## Membership conclusion and limits

Add `ovl_11_func_8011EA0C` to the existing
`D_80128420/D_80128422` init/set cluster at **low confidence** as its
input/state handler. The existing ledger already mentioned its call to
8011EC54 and the ignored return value; that edge alone is not new evidence.
The additional tie is the verified controller/renderer use of the shared
state and table, corroborated by the uninterrupted local call/link run.

The premise is that this local state/selection ownership and call/link
alignment reflect one original module. Shared globals and calls can cross
files, and adjacency does not establish a translation-unit boundary. No
shared static, GP-relative ownership or declaration-order/register-variable
quirk was established. Do not promote the confidence or add other neighbours
on adjacency alone.

The verified source also corrects the older ledger's claim that the handler
calls 8011EC54 only after storing state 6 itself: the call is gated by state
not being 7, readiness checks and input mask 0x120, not a required state-6
store. This correction is not additional grouping evidence.
