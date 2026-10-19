# ovl_11_func_80118CE4 — reset/quantity-handler grouping

The handoff reports byte-exact source and a passed full finalize gate for
`src/overlays/ovl_11/ovl_11_func_80118CE4.c`, with source/evidence identity
`99627d3f403c92a7a3a2db280d238f271a519f3d28346f9e896d0a5368006b43`.
Its changed-file list is that source and `configs/splat/ovl_11.yaml`.
This is documentation only; no matching experiments or build-input edits were
performed.

## New grouping evidence

The verified handler adds a shared-data fingerprint to the previously listed
0x80118C28–0x80118CE4 candidate run:

- **Adjacent reset and consumer.** `configs/splat/ovl_11.yaml` places
  `ovl_11_func_80118CB0` at file offset `0x60E90` and this handler at
  `0x60EC4`, preserving the documented zero-gap `0x34`-byte predecessor.
  `src/overlays/ovl_11/ovl_11_func_80118CB0.c` resets `D_8012D524` and
  `D_8012D52C` to zero. The verified handler dispatches on `D_8012D524`
  and writes `D_8012D52C = 1` during quantity selection.
- **Shared quantity storage as well as control state.** The predecessor sets
  `D_8012D540 = 1`. `include/globals_override.h` identifies that halfword
  as `D_8012D538.unk8`, the quantity selected and consumed by this handler;
  the handler also reads `D_8012D538.unkA` for quantity-times-price checks
  and payment. Thus the neighbouring reset supplies both control state and
  quantity data used by the newly verified body.

## Limits

This strengthens the existing run as a low-confidence same-TU candidate;
shared overlay state and reset/consumer relationships can cross source-file
boundaries. It does not prove private-static ownership, original TU boundaries,
or membership of every intervening neighbour. No declaration-order or
register-variable evidence was established.

The handler calls the already documented `ovl_11_func_8011D084` /
`ovl_11_func_8011D0B4` state helpers, but those distant calls do not establish
that their candidate run belongs to this TU. The newly assigned jump-table
rodata in the handoff configuration is not itself original-TU evidence.
