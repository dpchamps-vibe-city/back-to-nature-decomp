# ovl_15_func_801308B8 — grouping evidence

Verified handoff identity:
`9af4d2b4d109f7dbc9e53f6f438cd95bf38e893f762138fc6b458fa83776d909`.
The handoff reports byte identity and a passed full finalize gate; this review
changes notes only and does not rerun matching.

## Additional support for existing membership

The `D_80053350/func_8001AC10` shared-prologue section already names this
function among the `D_80052FC4` users. Its verified body adds an independent
shared-state tie to the already-named `ovl_15_func_8012F158`:

- `src/overlays/ovl_15/ovl_15_func_801308B8.c` and
  `src/overlays/ovl_15/ovl_15_func_8012F158.c` both select the card with
  `D_8013758C`, read `D_80137594[card]`, populate three halfword slot states
  in `D_80137820`, and pass the corresponding 0x3C-byte `D_801376E0` record
  to `ovl_15_func_80136288`. Both conditionally call `ovl_15_func_80135FF0`
  and update the same menu-state bytes `D_80137584`/`D_80137587`.
- The original assembly corroborates the shared objects and traversal:
  `build/ovl_15/asm/nonmatchings/ovl_15_func_801308B8/ovl_15_func_801308B8.s`
  at 0x80130930–0x80130A00, and
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012F158/ovl_15_func_8012F158.s`
  at 0x8012F1C8–0x8012F2BC. Both advance header records by 0x3C and slot
  states by 2 for three iterations.

The target additionally tests all three slot states before choosing the next
menu state, so it is a related scan handler, not a byte-identical duplicate.
The shared state and parallel helper sequence strengthen the existing
candidate same-TU grouping beyond its recorded draw prologue. They do not
establish static linkage or exact TU boundaries; retain medium confidence
and do not merge other clusters on this evidence alone.
