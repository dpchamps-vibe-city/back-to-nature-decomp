# ovl_15_func_80131498 — grouping evidence

Verified handoff identity:
`8f91795c984eb79f2d9c8a379fc13e8468b0325edd404a4918e4cdb3c5dc1549`.
The handoff reports byte identity and a passed full finalize gate; this review
changes notes only and does not rerun matching.

## Additional support for existing membership

The `D_80053350/func_8001AC10` shared-prologue section already names this
function among the `D_80052FC4` users. That prologue is not new evidence.
The verified body adds a shared-state tie beyond the draw setup:

- `src/overlays/ovl_15/ovl_15_func_80131498.c` and the already-grouped
  `src/overlays/ovl_15/ovl_15_func_8012ED6C.c` implement the same two-card
  polling sequence: guard on `D_8013759A >= 2`, call
  `ovl_15_func_80135C20` for each card, normalize statuses 1 to 0 and 7 to 6,
  and update `D_80137594[0..1]` only when a cached status differs. Both then
  set `D_80137584` to 2 when both cached statuses are 4, otherwise 3.
  This is a behavioral correspondence, not a claim of identical machine code
  or a shared register-variable quirk.
- The already-grouped `ovl_15_func_8012EF84`, `ovl_15_func_801306D4` and
  `ovl_15_func_801316B0` sources in the same directory consume
  `D_80137594[D_8013758C]` and route through the same `D_80137584` state byte,
  as recorded in [the earlier cache-writer evidence](ovl_15_func_8012ED6C-grouping.md).
  The target is therefore another writer of the handlers' shared cached state,
  not merely another user of the text-draw idiom.

This supports membership in the existing shared-prologue/state cluster.
Shared objects and parallel behavior do not establish static linkage or exact
original TU boundaries; retain medium confidence. The helper call alone does
not merge this group with the separately listed MemCard SDK cluster.
