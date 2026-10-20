# ovl_11_func_8010E6E8: local handler code/data adjacency

## Verified handoff

- Source: `src/overlays/ovl_11/ovl_11_func_8010E6E8.c`.
- Supplied verified source/evidence identity:
  `90bbb391160bc21dba4a0f08393bf6fe08d839d6c37b0ea309874e06416ae2a2`.
- The handoff reports byte identity and a passed full finalize gate. This
  notes-only review does not repeat matching or modify build inputs.

## New grouping evidence

The existing object-mode dispatch section already records the gapless handler
block. The verified body adds a code/data-order tie within that block:

- E6E8 uses `D_80127D78` as an eight-signed-halfword message-base lookup,
  indexed by the unsigned object halfword at +0xAC and passed with small
  offsets to `func_8002261C`. In
  `build/ovl_11/asm/data/69960.data.s`, that table occupies
  0x80127D78–0x80127D87, immediately before `D_80127D88`, the keyframe table
  used by `src/overlays/ovl_11/ovl_11_func_8010EA6C.c`.
- The consumers are also immediate code neighbours: E6E8's 0x384 bytes end
  exactly at 0x8010EA6C. `configs/splat/ovl_11.yaml` records consecutive
  offsets 0x568C8 and 0x56C4C; `build/ovl_11/functions.csv` corroborates the
  size. Thus the newly identified message-table consumer and the known
  keyframe-table consumer agree in code and data order.
- `build/ovl_11/asm/data/2BC4.rodata.s` places E6E8 at
  `D_800BAABC[6]` and EA6C at `[5]`. Their shared dispatch-table membership
  was already implicit in the ledger; it corroborates, rather than itself
  constitutes, the new adjacency evidence. Both bodies use object state
  +0x28 and flag 0x800 at +0x34, and their state -1 paths set the +0x2C timer
  to 0x12C, clear that flag and return 1.

The original E6E8 listing at
`build/ovl_11/asm/nonmatchings/ovl_11_func_8010E6E8/ovl_11_func_8010E6E8.s`
corroborates the lookup at 0x8010E8F0/0x8010E9BC and the terminal path at
0x8010EA0C.

## Conclusion and limits

Record E6E8 as a low-confidence candidate same-TU neighbour of EA6C within
the existing object-mode handler block. Adjacent consumer-specific tables,
adjacent code and a common dispatch/state contract support that local
hypothesis, but prove neither static linkage nor either outer file boundary.
They do not establish common TU ownership with the dispatcher or the distant
80110658/80110944 callees. E6E8 is not a keyframe-handler template member:
it does not call 800D04D4. No declaration-order or register-variable evidence
was established, and no neighbouring ledger groups are merged.

Role: availability-gated dialogue/choice state handler.
