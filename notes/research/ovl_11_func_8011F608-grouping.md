# ovl_11_func_8011F608 — selection-driver grouping evidence

## Provenance

Notes-only review of the supplied byte-exact/full-finalize handoff. Verified
source/evidence identity:
`7b80846edd342b59e6da9a1fec28fa330fc052b64e0c51665030959a61f7ff4e`.
The supplied changed-file list is `configs/splat/ovl_11.yaml`,
`include/globals_override.h`, `notes/file-groupings.md` and
`src/overlays/ovl_11/ovl_11_func_8011F608.c`. Verification is supplied, not
rerun here; no build inputs were edited for this documentation.

## Membership evidence

The handoff already added `8011F608` to the D_801285xx table-selection ledger
entry. Its source supports that membership and a candidate membership for its
immediate successor, the reset leaf `8011F958`:

- **Shared state connects the driver to the existing initializer.**
  `src/overlays/ovl_11/ovl_11_func_8011F52C.c` clears `D_80128540`, the three
  words at `D_80128544`, and `D_80128556`, and initializes the selector
  triplet at `D_80128550`. The verified driver advances `D_80128540`, passes
  the same three-word buffer to `8011F9EC`, and stores `8011FB80`'s result
  in `D_80128556`. The array alias in `include/globals_override.h` identifies
  the buffer with the initializer's existing struct view; it is not a new
  object. `8011F9EC` writes the selector triplet that `8011FB80` consumes
  (see their respective sources under `src/overlays/ovl_11/`). Thus the
  driver connects the shared state to the existing table-selection helpers,
  rather than merely using nearby data.
- **The reset call closes the local chain.** In case 9 the verified driver
  calls `8011F958`. Its source,
  `src/overlays/ovl_11/ovl_11_func_8011F958.c`, saves `D_8012855A` and calls
  the same initializer `8011F52C`, which clears that halfword along with the
  driver's state. This ties the reset leaf to both the driver and the
  existing initializer; it is not included just because it is adjacent.
- **Configured link order corroborates the calls.**
  `configs/splat/ovl_11.yaml` lists consecutive starts at file offsets
  `0x677E8` (`8011F608`), `0x67B38` (`8011F958`), `0x67BCC` (`8011F9EC`)
  and `0x67D60` (`8011FB80`). The driver directly calls each of the three
  successors. The initializer is earlier at `0x6770C`, with `8011F574` and
  `8011F5BC` intervening; it is not the driver's immediate predecessor.

## Conclusion and limits

Retain the driver in the existing medium-confidence candidate group and add
`8011F958` as its candidate reset leaf. The ledger's lower address becomes
`8011F52C`, reflecting the initializer already listed there. This is not a
newly recovered TU boundary, and the wider attachment through `80120414`
retains the premises in
[the existing extension note](ovl_11_func_80120414-grouping.md).

The conditional premise is that this local state/initializer/helper chain,
corroborated by link order, reflects one original module. Shared absolute
addresses do not prove ownership or static linkage, and calls can cross TUs.
No declaration-order effect or register-variable quirk was established.
The neighbouring timer `D_80128558` is not independent shared-state evidence.
Broad engine/render/music calls and access to `D_8007AFF0` do not justify
merging other groups. No additional membership is inferred for the intervening
functions or for the separate preceding `8011F1D0` run; its
[existing boundary caveat](ovl_11_func_8011F1D0-grouping.md) still applies.
