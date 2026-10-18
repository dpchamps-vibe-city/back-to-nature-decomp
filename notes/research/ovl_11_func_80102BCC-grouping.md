# ovl_11_func_80102BCC: ID-map run backward extension

## Evidence identity

The handoff reports byte-exact output and a passed full finalize gate for
source/evidence identity
`241aabc970c18f7189edd54ceadc2f171a1190a9671914ea41d5adab211a926f`
(origin: agent). The gate was not rerun for this notes-only role. The supplied
changed files were reviewed without modification:

- `include/game_types.h`
- `include/globals_override.h`
- `include/overlays/ovl_11.h`
- `include/sdk_types.h`
- `src/overlays/ovl_11/ovl_11_func_80102BCC.c`

## New membership evidence

The existing `D_8012CF00` ID-map run at 0x80102E1C–0x801033D4 gains
candidate members `ovl_11_func_80102BCC` and `ovl_11_func_80102DC4`:

- **Shared rollback-state cluster:** the verified target saves all eight u16
  entries of `D_8012CF00` and the u16 `D_801273EE`, restores both before
  each candidate test, and restores both again before returning. Its first
  validator, `80102DC4`, consumes required bits from `D_801273EE`; its
  second, `80102E1C`, consumes IDs from `D_8012CF00`; its third,
  `80102F14`, reads both remaining-state globals to adjust the candidate's
  value. This connects the adjacent helpers through the exact state the
  target protects, rather than merely through common engine calls. The
  `_ids` and `_flags` declarations in `include/globals_override.h` are
  views of those same assembler symbols.
- **Call graph agrees with gapless link order:**
  `configs/splat/ovl_11.yaml` places `80102BCC`, `80102DC4`, `80102E1C`
  and `80102F14` at offsets 0x4ADAC, 0x4AFA4, 0x4AFFC and 0x4B0F4.
  The original extents are 0x1F8, 0x58 and 0xF8 for the first three, so
  each ends exactly at the next entry. The target calls those three
  successive helpers at 0x80102C94, 0x80102CA4 and 0x80102CB4,
  respectively, passing the same object to each. The latter two already
  belong to the documented ID-map run.
- **Shared candidate-object view:** the target walks `D_80050B5C` with a
  0x20-byte stride, reading the u16 ID at +4 and u8 value at +6.
  `80102DC4` reads the required flag mask at +8; `80102E1C` scans eight
  u16 IDs at +0xA; `80102F14` uses the adjustment-table pointer at +0x1C.
  `include/game_types.h` now records the target's fields in the existing
  `Ovl11IdAdjustmentObject` view. This shared layout corroborates the
  call/state cluster; the modern shared typedef alone is not evidence of
  an original shared header or TU.

Source evidence for the helpers:

- `src/overlays/ovl_11/ovl_11_func_80102DC4.c`
- `src/overlays/ovl_11/ovl_11_func_80102E1C.c`
- `src/overlays/ovl_11/ovl_11_func_80102F14.c`

Original instruction evidence:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_80102BCC/ovl_11_func_80102BCC.s`
  (ID snapshot at 0x80102C18–0x80102C2C; flag snapshot at 0x80102C44;
  per-candidate restores at 0x80102C6C–0x80102C90; final restores at
  0x80102CE8–0x80102D04).
- `build/ovl_11/functions/overlay/ovl_11_func_80102DC4.s`
  (required mask at 0x80102DCC; shared flag read/write at 0x80102DE0 and
  0x80102DFC).

## Limits

This is a medium-confidence extension conditional on the existing ID-map
run's same-TU hypothesis, not proof of private linkage or original TU
boundaries. The previously documented consumer-run caller `8010289C` is
not new evidence by itself. Neither that call nor the shared globals alone
justify merging this run with the earlier record-table consumer run or the
separate state-block handler/display cluster. The boundary at 0x801033D4
remains unchanged.
