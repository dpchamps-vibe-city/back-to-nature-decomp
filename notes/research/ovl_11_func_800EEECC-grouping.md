# ovl_11_func_800EEECC: five-slot refresh grouping bridge

The supplied handoff reports byte-exact source and a passed full finalize gate
(origin: agent; source/evidence identity
`f797afd6457be578df8d2aac074e2a8d1ae3c04c6635674521eafdaf1a07ead0`).
This is notes-only documentation, not a new matching experiment.

## New grouping evidence

The ledger already records EEECC's shared +0x44BA/+0x44BC state, its call to
EF0E8 and its gapless adjacency to EEFB8. The verified source adds a direct
query/update bridge to the following record family:

- **Paired local calls.**
  `src/overlays/ovl_11/ovl_11_func_800EEECC.c` visits indices 0 through 4,
  calling `ovl_11_func_800EF488(index)` exactly when
  `ovl_11_func_800EF0E8(index) == 1`. Thus the run head directly dispatches
  both the eligibility classifier and its complementary record snapshotter,
  rather than connecting to the snapshot/set/reset family only through the
  previously documented shared records and producer.
- **Complementary flag bank.** Before that loop EEECC clears flags
  0x26 through 0x2A with `func_8001AF70`. Verified
  `src/overlays/ovl_11/ovl_11_func_800EF488.c` selects precisely those five
  flags for indices 0 through 4 and sets the selected flag after snapshotting
  the record. Verified
  `src/overlays/ovl_11/ovl_11_func_800EF0E8.c` queries those same per-index
  flags. This supplies a coordinated clear/query/set interface across the
  three functions, not merely a common generic flag-service callee.
- **Link order agrees.** `configs/splat/ovl_11.yaml` places EEECC, EEFB8,
  EF0E8 and EF488 consecutively at file offsets 0x370AC, 0x37198, 0x372C8
  and 0x37668. The existing ledger records the gapless first three members;
  [the EF0E8 note](ovl_11_func_800EF0E8-grouping.md) establishes the gapless
  EF0E8-to-EF488 bridge, and
  [the EF488 note](ovl_11_func_800EF488-grouping.md) follows the record family
  through EF6E4 and EF870.

## Conclusion and limits

EEECC is the five-slot refresh dispatcher and a candidate same-TU member of
the adjacent EF0E8/EF488/EF6E4/EF870 record family. Its paired calls and
complementary flag bank corroborate the previously recorded bridge from the
EEECC/EEFB8/EF0E8 run into that family.

The flag service and state are globally shared, not demonstrated TU-local
statics. Consecutive functions and coordinated callers can still cross TU
boundaries. No new declaration-order effect, register-variable quirk or
GP-relative ownership is established; the original TU boundary remains
unproven. This does not justify merging the broader ledger clusters or
assigning EEECC to the distant D_80127428 TU.
