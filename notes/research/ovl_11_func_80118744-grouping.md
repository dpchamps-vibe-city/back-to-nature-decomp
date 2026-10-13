# ovl_11_func_80118744: candidate tail of the D_8012D7A8 neighbourhood

## Scope and provenance

Documentation-only review of the verified source at
`src/overlays/ovl_11/ovl_11_func_80118744.c`. Supplied finalize evidence identity:
`886f491c7b44d0afb7955cca8a5114d2786d2c1f8a1298a4de692ad75918444d`.
No source, header or configuration changes, matching experiments or new gate
claims are part of this review.

## New grouping evidence

The existing ledger section for the `D_8012D7A8` accessor run already extends
through `ovl_11_func_80117F70`, on shared-record and link-order evidence.
The following tail is a supported **low-confidence candidate extension**, not
an established original translation-unit boundary.

- `configs/splat/ovl_11.yaml` and the original assembly sizes give a gapless
  sequence: `80117F70` (0x180) → `801180F0` (0x60C) → `801186FC` (0x48)
  → `80118744` (0x284), ending at `801189C8`.
- The two call edges agree with this local ordering: `80117F70` calls
  `801186FC` at 0x80117F88, and `801180F0` calls `80118744` at 0x80118394.
  The caller and verified handler are **not immediate neighbours**: the
  intervening function is the preceding initializer's five-id scan helper.
- `src/overlays/ovl_11/ovl_11_func_80117F70.c` initializes
  `D_8012D7CC`, `D_8012D7D0`, `D_8012D7D4` and the descriptor at
  `D_8012D7D8`, as well as `D_8012D548`, `D_8012D5A8` and `D_8012D538`.
  The original `801180F0` assembly consumes that state/descriptor cluster
  and selects the same two records for the descriptor's record pointer.
- At 0x80118350–0x80118398, `801180F0` passes `&D_8012D524`, base 0,
  flag value 5, the five-word descriptor at `D_8012D7D8` by value, and
  `&D_8012D538` to the verified handler. The verified source consumes this
  record/descriptor, writes `D_8012D52C`, and advances state through
  `ovl_11_func_8011D098`/`ovl_11_func_8011D0B4`. This establishes concrete
  initializer → controller → handler record flow within the gapless tail,
  beyond a coincidental shared busy flag.

Original assembly evidence (each path under
`build/ovl_11/asm/nonmatchings/`):

- `ovl_11_func_80117F70/ovl_11_func_80117F70.s`
- `ovl_11_func_801180F0/ovl_11_func_801180F0.s`
- `ovl_11_func_80118744/ovl_11_func_80118744.s`

The call edges and sizes are also present in `build/callGraph.json`; the scan
helper's verified source is
`src/overlays/ovl_11/ovl_11_func_801186FC.c`.

## Limits

No new tail member directly accesses `D_8012D7A8`; its link to that table run
is conditional on the ledger's existing `80117F70` extension. Absolute data
references do not establish private/static ownership. `D_8012D52C` is already
recorded as container-wide, so its use alone cannot identify a TU. Similarity
to the distant `8011BAFC` handler and the verified per-function jump-table
split likewise do not justify extending membership to that handler or to
`801189C8`. Only the corroborated local tail is added to the ledger.
