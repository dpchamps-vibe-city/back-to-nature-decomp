# ovl_11_func_80102368: record-table consumer membership

## Verified handoff

The handoff reports byte-exact output and a passed full finalize gate for
source/evidence identity
`182bc9c7005860e55e0d588e2d6b64d0766a3f7726e77a1935ea6ff926a80f11`
(origin: agent). The supplied changed files were reviewed without modification:

- `include/globals_override.h`
- `src/overlays/ovl_11/ovl_11_func_80102368.c`

The gate was not rerun for this notes-only role.

## New membership evidence

The verified source supports adding `80102368` to the existing
D_80071AC0/D_80070D42 record-table consumer hypothesis, extending its listed
range backward from 0x801027D4 to 0x80102368:

- **Shared ID-table and record-table data flow:** the target reads all eight
  u16 entries of `D_8012CF00_ids` (assembler symbol `D_8012CF00`) and looks
  each up among six-byte records at `D_8006C838 + 0x5288`, which is
  `D_80071AC0`. Its bound uses the signed halfword at +0x51E6 through
  `func_8001F250(2, exponent + 1)`. Existing member `80102A64` scans that
  exact table with that exact bound; `80102A10` supplies its IDs from the
  same eight-entry table. `80102844` accesses the same six-byte records,
  and `801027D4` tests an accessed ID against `D_8012CF00`. The new tie is
  this specific combination of globals and complementary roles, not just
  the engine-helper call already mentioned in the ledger.
- **Call graph corroborates the local link run:** `80101B9C` calls the
  target and `801024D8` consecutively at 0x80101CE4 and 0x80101CEC. That
  caller also reads/writes `D_8012CF00` and calls `801027D4` and `80102844`
  while managing the selected IDs. The target's original extent is 0x170,
  ending exactly at `801024D8`; that function's 0x2FC extent ends exactly
  at `801027D4`, whose 0x70 extent ends at existing member `80102844`.
  `configs/splat/ovl_11.yaml` records the corresponding offsets 0x4A548,
  0x4A6B8, 0x4A9B4 and 0x4AA24.
- **Adjacent display companion:** `801024D8` renders the corresponding
  record grid and marks IDs found in `D_8012CF00`. Both display functions
  use the context at `D_8005E3C0->field_D8 + 0x68`, start x at 0x84,
  advance/wrap with the same 0x22/0xEB grid geometry, and choose between
  `800D6F78` and `800F6D14` for record/ID drawing. This corroborates the
  adjacent pair's shared caller and data, rather than relying on adjacency
  alone.

## Evidence paths

Verified target source and its original instruction listing:

- `src/overlays/ovl_11/ovl_11_func_80102368.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80102368/ovl_11_func_80102368.s`
  (ID load at 0x801023B0–0x801023C0; bounded lookup at
  0x801023E0–0x8010242C; render choice at 0x80102430–0x8010247C).

Corroborating sources/listings and link order:

- `src/overlays/ovl_11/ovl_11_func_801027D4.c`
- `src/overlays/ovl_11/ovl_11_func_80102844.c`
- `src/overlays/ovl_11/ovl_11_func_80102A10.c`
- `src/overlays/ovl_11/ovl_11_func_80102A64.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80101B9C/ovl_11_func_80101B9C.s`
  (display calls at 0x80101CE4/0x80101CEC; accessor and membership calls
  include 0x80102104/0x80102124).
- `build/ovl_11/asm/nonmatchings/ovl_11_func_801024D8/ovl_11_func_801024D8.s`
  (table/bound at 0x8010253C–0x80102550; six-byte stride at 0x801026A4;
  selected-ID scan at 0x8010272C–0x80102774).
- `configs/splat/ovl_11.yaml`
- `include/globals_override.h` (`D_8012CF00_ids` alias and the target's
  `Ovl11ItemGridState` view).

## Limits

Membership remains conditional on the existing medium-confidence same-TU
hypothesis. No private linkage, declaration-order effect or register-variable
quirk is established here; the modern shared type is not original-TU evidence.
The caller and intervening display/helper functions corroborate the target's
membership, but this note does not assign every function in the wider link
interval to one TU. It does not merge the separate ID-map, handler/state-block
or distant item-table clusters, and leaves the consumer run's upper boundary
unchanged.
