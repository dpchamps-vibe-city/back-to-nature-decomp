# ovl_15_func_801320A8 — grouping evidence

Verified handoff identity:
`fb8595358c9b3fc674566c3a8932f2f6a932b363805bb7d84d989bc8b0d128c1`.
The handoff reports byte identity and a passed full finalize gate; this review
changes notes only and does not rerun matching.

## Additional support for existing dual-base slot membership

The ledger already records the two record bases and four calls to
`ovl_15_func_80133538`. The verified body adds a direct shared-table tie:

- `src/overlays/ovl_15/ovl_15_func_801320A8.c` counts the type halfwords in
  both five-entry rows of `Ovl15Slots`, the same table traversed by
  `src/overlays/ovl_15/ovl_15_func_801335F8.c`. The alias in
  `include/globals_override.h` identifies it as `D_80140F90`.
- Original assembly at
  `build/ovl_15/asm/nonmatchings/ovl_15_func_801320A8/ovl_15_func_801320A8.s`
  corroborates the table address at 0x80132100–0x80132104 and the
  eight-byte entry step at 0x801321B8. Thus the target shares the table
  itself, not merely the counter's record-base arguments.

Retain the existing low confidence for this function's same-TU membership;
this does not establish static linkage or an original TU boundary.

## Supported shared-prologue membership

Add the function to the existing medium-confidence
`D_80053350/func_8001AC10` shared-prologue cluster:

- The verified source uses the cluster's
  `func_8001AC10(field_D8 + 0x18, field_D8 + 0x14, text + D_80054BBC[0])`
  setup, with text base `D_800520A0`. The original assembly witnesses this
  at 0x801320D8–0x801320EC.
- It writes the shared state bytes `D_80137584` and `D_80137588`, as does
  existing member `src/overlays/ovl_15/ovl_15_func_80132638.c`, which also
  uses that draw setup and calls the slot-table walker `ovl_15_func_801335F8`.
- `configs/splat/ovl_15.yaml` places the target immediately before existing
  member `ovl_15_func_8013237C`: 0x42C0 + 0x2D4 = 0x4594. That successor's
  verified source uses the same draw setup and updates `D_80137584`, with
  0x15 as its stay-state argument; the target selects 0x15 for an empty
  transfer selection. This is corroborating state/link proximity, not a
  direct caller/callee relationship.

`D_800520A0` is not itself a shared-global tie: an overlay-source search
finds it only in the target. Candidate membership rests on the combined
shared draw idiom and state cluster, with link proximity as corroboration.
Keep the prologue cluster's confidence unchanged and do not merge it with
the dual-base slot group or infer exact TU boundaries from this evidence.
