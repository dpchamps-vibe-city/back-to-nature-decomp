# ovl_11_func_80107044: translation-unit grouping evidence

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `7d8ac17a8b4afc4c6da50dc80976d67acbbe8500345f1d29403089f71042c4bd`.
Its changed-file list is `include/game_types.h`, `include/sdk_types.h` and
`src/overlays/ovl_11/ovl_11_func_80107044.c`. This review does not rerun matching
or change build inputs.

## New evidence

- **Immediate caller plus shared table.** Original assembly for
  `ovl_11_func_8010726C` loads `D_8012CF48` at 0x80107270–0x80107278,
  walks ten entries at stride 0x18, and passes each entry directly to
  `ovl_11_func_80107044` at 0x801072E8 and 0x8010731C for states 2 and 4.
  Thus the verified function's argument is an entry of the same table already
  tying together `80106DE8`, `80106E38` and `80106F20`, not merely a similar
  anonymous structure.
- **Complementary state operations.** The verified `Ovl11RotationState`
  layout has state/direction at +0/+2, a timer at +4, an increment at +8,
  three rotation halfwords at +0xC/+0xE/+0x10 and a source pointer at +0x14.
  These agree with the reset leaf `80106DC0` and table-entry setup in
  `80106E38`. `80106F20` starts state 2 or extends the +4 timer; `80107044`
  updates states 2/4 and enters state 3 with that timer; `8010726C` counts
  down state 3 before entering state 4. This corroborates the shared-table
  call edge with complementary producer/consumer roles.
- **Gapless link order.** The existing run ends at 0x80107044. Original
  assembly sizes give `80107044 + 0x228 = 8010726C`,
  `8010726C + 0xE0 = 8010734C`, and
  `8010734C + 0x44 = 80107390`. `configs/splat/ovl_11.yaml` lists those
  successors consecutively at offsets 0x4F224, 0x4F44C and 0x4F52C.
- **Adjacent shared-table accessor.** `8010734C` walks the same ten-entry,
  0x18-stride table, compares the linked object's s16 ID at +0xA and returns
  the entry's state at +0. Its table use was already mentioned in the ledger;
  the newly established updater/driver pair fills the intervening contiguous
  run and supports including the accessor in the candidate group.

Evidence paths:

- `src/overlays/ovl_11/ovl_11_func_80107044.c`
- `include/game_types.h` (`Ovl11RotationState` and its component types)
- `src/overlays/ovl_11/ovl_11_func_80106DC0.c`
- `src/overlays/ovl_11/ovl_11_func_80106E38.c`
- `src/overlays/ovl_11/ovl_11_func_80106F20.c`
- `src/overlays/ovl_11/ovl_11_func_8010734C.c`
- `build/ovl_11/functions/ovl_11_func_80107044.s`
- `build/ovl_11/functions/ovl_11_func_8010726C.s`
- `build/ovl_11/functions/ovl_11_func_8010734C.s`
- `configs/splat/ovl_11.yaml`

## Limits

Assuming shared table ownership and corroborated call/link adjacency reflect
original source organization, this extends the existing medium-confidence
candidate TU through `8010734C` (exclusive code end 0x80107390). It does not
prove either original TU boundary, a private static, GP-relative ownership,
a declaration-order effect or a file-scope register-variable quirk. The shared
engine base `D_8007AFF0`, rotation setter `800DCC1C` and sound helper alone do
not justify merging distant functions into this group. Neighbouring source is
used as grouping evidence, not as a new claim that its matching gate was run
in this documentation turn.
