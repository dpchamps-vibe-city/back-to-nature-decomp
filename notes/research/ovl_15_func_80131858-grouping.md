# ovl_15_func_80131858 — grouping evidence

Verified handoff identity:
`5ae2b213dcf91c61ac75df0f82249474f2d018b58c8509b9bf3784cf0774f09c`.
The handoff reports byte identity and a passed full finalize gate; this review
changes notes only and does not rerun matching.

## Additional support for existing membership

The `D_80053350/func_8001AC10` shared-prologue section already names this
function among the `D_80052FC4` users. Its verified body adds a shared-state
tie to the existing members `ovl_15_func_801308B8` and
`ovl_15_func_8012F158`, beyond that recorded prologue:

- `src/overlays/ovl_15/ovl_15_func_80131858.c` and
  `src/overlays/ovl_15/ovl_15_func_801308B8.c` select the same card through
  `D_8013758C`, read `D_80137594[card]`, populate the three halfword slot
  states in `D_80137820`, and pass the corresponding 0x3C-byte
  `D_801376E0` record to `ovl_15_func_80136288`. Both conditionally call
  `ovl_15_func_80135FF0` and update `D_80137584`/`D_80137587` after testing
  all three slot states. Their body logic differs only in the menu state
  selected for status 4/5: 0x1E here versus 0x10 in 801308B8.
- Original assembly corroborates the shared objects and traversal:
  `build/ovl_15/asm/nonmatchings/ovl_15_func_80131858/ovl_15_func_80131858.s`
  at 0x801318D0–0x80131A00, and
  `build/ovl_15/asm/nonmatchings/ovl_15_func_801308B8/ovl_15_func_801308B8.s`
  at 0x80130930–0x80130A60. Both advance header records by 0x3C and slot
  states by 2 for three iterations.
- `src/overlays/ovl_15/ovl_15_func_8012F158.c` uses the same state cluster
  and helper sequence, as recorded in
  [the 801308B8 grouping note](ovl_15_func_801308B8-grouping.md).

The shared-state tie strengthens the existing candidate same-TU membership;
body similarity alone is not the premise. These global references do not
establish static linkage or exact TU boundaries. Retain medium confidence
and do not merge other clusters on this evidence alone.
