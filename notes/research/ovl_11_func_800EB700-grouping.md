# ovl_11_func_800EB700 grouping evidence

The verified function adds a candidate member to the existing `D_80129560`
accessor family and a local same-TU tie to `ovl_11_func_800EB760`. It does
not establish an original translation-unit boundary.

Supplied full-finalize-gate-passed source/evidence identity:
`e2e24aabdd4d6746573aaea53b518013ec60f1a418fdd0e3a4d0fb5c04d3596a`.
The supplied changed files are `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800EB700.c`; neither is modified by this
documentation.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800EB700.c` passes
  `D_80129560[(s16)arg1]` to `ovl_11_func_800EFC68`, then passes the result
  to `ovl_11_func_800EFDA0` with `arg3` as the comparison value and `arg2`
  as the mode. Family member `ovl_11_func_800E67D4.c` in the same directory
  uses the same two-helper chain, with the table supplying the comparison
  value instead.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800EB700/ovl_11_func_800EB700.s`
  confirms the absolute table base, signed-halfword-scaled word load and
  calls at 0x800EB734/0x800EB744. Its 0x60-byte extent ends at 0x800EB760.
- `src/overlays/ovl_11/ovl_11_func_800EB760.c` and its corresponding assembly
  under `build/ovl_11/asm/nonmatchings/ovl_11_func_800EB760/` show the same
  table slot forwarded as the second argument to `ovl_11_func_800EFABC`,
  another helper in the recorded range-entry accessor band.
  `configs/splat/ovl_11.yaml` places EB700/EB760 consecutively at offsets
  0x338E0/0x33940. Shared table use and slot-forwarding shape corroborate
  that gapless adjacency.

Limits: `D_80129560` is overlay-private, not proven TU-private. The neighbours
use different helpers and do not call each other; no register-variable or
declaration-order quirk is inferred. These observations support candidate
membership, not merging the whole accessor family or its distant callees
into one proven original TU.
