# ovl_11_func_8010CF58: candidate object-mode run membership

## Verified handoff

- Source: `src/overlays/ovl_11/ovl_11_func_8010CF58.c`.
- Supplied verified source/evidence identity:
  `84cb05d6993bc19b2c29a812743e0fa702cf52b905e5ded9eb68b895b8107dfa`.
- The handoff reports byte identity and a passed full finalize gate; this
  documentation-only review does not repeat verification or change build inputs.

## Grouping evidence

The existing object-mode dispatch section lists CF58 only as an adjacent
boundary marker. The verified body and its nearby caller support tentative
membership instead:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010C7F0/ovl_11_func_8010C7F0.s`
  walks seven `D_8007A638` records at stride 0xF8. At 0x8010C880 it passes
  the current record to CF58, then conditionally passes that same record to
  run member `ovl_11_func_8010CD80` at 0x8010C8BC or 0x8010C8D4. This ties
  CF58 to the local update/dispatch call graph, not merely to its address
  neighbours.
- CF58 reads the handler at signed halfword +0x26 and clears bit 0x800 in
  the unsigned halfword at +0xB8. Its immediate predecessor
  `src/overlays/ovl_11/ovl_11_func_8010CE80.c` writes the selected handler at
  +0x26 and clears the same bit in the same halfword. CF58's +0x30 mode
  test also agrees with the object view used by
  `src/overlays/ovl_11/ovl_11_func_8010CD80.c`.
- CF58 passes the record to `ovl_11_func_80110E34`, also called on the object
  by `src/overlays/ovl_11/ovl_11_func_8010CB6C.c`. This supports the common
  record interpretation; the shared helper alone is not TU evidence.
- `configs/splat/ovl_11.yaml` places CE80, CF58 and D04C at file offsets
  0x55060, 0x55138 and 0x5522C. Their corresponding target addresses/sizes
  are 0x8010CE80/0xD8, 0x8010CF58/0xF4 and 0x8010D04C/0xA0, also recorded
  in `build/ovl_11/ovl_11.map`: CF58 fills the gapless link-order position
  between two existing candidate members.

## Conclusion and limits

Treat CF58 as a candidate member of the existing object-mode dispatch run,
not just a boundary marker: its local shared caller and record-state coupling
corroborate the already-recorded link adjacency. Retain the section's medium
confidence. This does not establish an original TU boundary or merge the
neighbouring seven-record section into it; the object layout and movement
helper occur more widely, and no new shared static, GP-relative definition,
declaration-order effect or compiler quirk was established.

Role: mode-2 gated item action and object movement reset.
