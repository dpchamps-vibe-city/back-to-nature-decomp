# ovl_11_func_800E3DC8: shared loader/cursor state in the dispatch cluster

The verified function strengthens its existing candidate membership in the
`ovl_11` D_800B9920 four-state dispatch cluster, 0x800E3A3C–0x800E4330.
Keep medium confidence and the existing membership; no TU boundary is proved.

New grouping evidence:
- `src/overlays/ovl_11/ovl_11_func_800E3DC8.c` and
  `src/overlays/ovl_11/ovl_11_func_800E4330.c` both derive a read's offset
  and size from consecutive `D_801240D8` entries selected by the current
  object's signed id. Both hand completed reads to 800E499C.
- Their completion paths both initialise `D_801291B0` and `D_801291B4`
  (`Ovl11RecordCursor`) to `D_8006E910`, clear `D_80129224`, call
  800EEBC8 with four zero arguments, then call 800E54C8 and set the same
  completion fields. This shared global cluster and completion sequence
  were not recorded in the dispatch-group entry.
- The intervening table handler 800E40CC consumes those cursor globals and
  updates `D_80129224`. This gives the loader/reset paths a nearby consumer,
  beyond the previously documented selector/table relationship.

Original-assembly anchors:
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E3DC8/ovl_11_func_800E3DC8.s`:
  0x800E3E50–0x800E3E80 selects consecutive offsets and subtracts them;
  0x800E4064–0x800E409C performs the common cursor/reset completion path.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E4330/ovl_11_func_800E4330.s`:
  0x800E433C–0x800E4374 is the corresponding offset calculation;
  0x800E43C4–0x800E4410 is the corresponding completion path.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E40CC/ovl_11_func_800E40CC.s`:
  0x800E411C–0x800E4210 reads/updates the cursor cluster.
- `build/ovl_11/asm/data/1740.rodata.s`, D_800B9920 at
  0x800B9920–0x800B992C, lists 800E3DC8/800E40CC/800E4280/800E4330.
  `configs/splat/ovl_11.yaml` places these handlers consecutively in that
  order. The ledger's former `1664.rodata.s` path no longer contains this
  table; the current evidence path is corrected with this update.

These are shared absolute-addressed globals, not demonstrated private statics
or GP-relative definitions establishing ownership. The shared state and
completion sequence corroborate the existing same-TU hypothesis, but could
also be interfaces between TUs. No merge with the callee/helper groups follows.

Supplied verified source/evidence identity:
`bca518b3cea4f7b5f8a86c0f17411941b65b780a287f14910dc037632d7567e9`.
Origin: agent. Supplied changed files: `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800E3DC8.c`. The handoff reports byte-exact
matching and a passed full finalize gate; no solver or build gate was rerun
for this notes-only update.
