# ovl_11_func_800F44B4: complementary spawn-record reader

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`ee446ec1f49a95c9fb2e14b9a59bd67f3763d3d2061f131fc0f684244c71f999`.
Its changed-file list is `include/overlays/ovl_11.h`,
`notes/file-groupings.md` and
`src/overlays/ovl_11/ovl_11_func_800F44B4.c`. The source SHA-256 when read was
`b7c25ac1fdbebd80a5ce7cf46ed5bc38e263f1af267611ead62b2e4a879abcee`.
This documentation pass did not rerun the gate or edit build inputs.

Evidence read:
- `src/overlays/ovl_11/ovl_11_func_800F44B4.c`: verified reader.
- `src/overlays/ovl_11/ovl_11_func_800F43CC.c`: preceding dispatch writer.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800F44B4/ovl_11_func_800F44B4.s`
  and `build/ovl_11/asm/nonmatchings/ovl_11_func_800F43CC/ovl_11_func_800F43CC.s`:
  original selector mapping, pool loads, record stride and call edge.
- `include/game_types.h` (`Ovl11DispatchRecord`) and
  `include/globals_override.h` (`Ovl11DispatchState`): current record and
  state views; their modern type names are not original-TU evidence.
- `build/ovl_11/functions.csv`, rows 922–926: fill/clear/dispatch/read
  link order, sizes and the dispatch-to-fill call.
- `notes/research/ovl_11_func_800F42A8-grouping.md`: existing reset/fill
  destination and flag-operation corroboration.

## Reader-extension evidence

The new corroboration is the reader's exact selector-to-pool mapping shared
with the immediately preceding dispatch writer `800F43CC`, not merely use
of the broadly shared `D_8006C838` work area. In both original functions:

| Selector | Record destination, offsets relative to D_8006C838 |
| --- | --- |
| 0xE8, 0x122–0x124 | Pointer at +0xDDCC, indexed with stride 0x18 |
| 0x65 | Pointer at +0xDDD4, advanced by one 0x18-byte record |
| 0x64 | Pointer at +0xDDD4, first record |

Both reach those pointer slots through the base plus 0x8000 and loads at
+0x5DCC/+0x5DD4. The reader copies the words at record +8/+0xC/+0x10 and
the halfword at +0, then returns bit 0 of the halfword at +4. These are
complementary reads of the dispatch/fill family's record fields; the
existing reset research independently ties the same destinations to the
clear operation.

Original link order is gapless: `800F4360` (0x30) → `800F4390` (0x3C) →
`800F43CC` (0xE8) → `800F44B4` (0xF0), ending at 0x800F45A4. The dispatch
calls `800F4360`; the reader is a leaf and introduces no direct call edge
to its neighbours. The shared pool mapping and complementary operations
corroborate extending the existing candidate family to the reader.

## Limits

Membership remains conditional on these complementary record helpers
sharing an original translation unit. No private static, TU-owned global,
declaration-order effect or register-variable quirk was established.
Adjacency alone cannot exclude an intervening TU boundary, and the
reader's matching switch is not proof of one original file. Confidence
therefore remains low; no original filename or boundary is claimed.
Neither the next function at 800F45A4 nor distant callers are added to the
family on this evidence.
