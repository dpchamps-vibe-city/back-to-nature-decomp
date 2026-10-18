# ovl_11_func_800E9914 grouping evidence

The finalized function supports candidate local same-TU membership with
`ovl_11_func_800E9838` and `ovl_11_func_800E99EC` in the existing
`D_80129560` accessor family. Shared table use corroborates gapless link
adjacency; it does not establish an original TU boundary.

Supplied verified source/evidence identity:
`2a672a28da5e30c6c316d625ae4ea23a6939be7a98635517f9e205ebfb033df1`.
The handoff reports byte identity and a passed full finalize gate, with
changes to `include/globals_override.h`, `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800E9914.c`. This documentation changes
none of those files.

## Evidence

- `src/overlays/ovl_11/ovl_11_func_800E9914.c` optionally writes the signed
  halfword at `D_8006C838+0x51F0` into `D_80129560[arg2]`, guarded by
  `arg2 != -1`, before clearing the halfword. This is another producer
  using the family's guarded snapshot-write idiom.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E9914/ovl_11_func_800E9914.s`
  confirms the absolute table base, index scaled by four and `lh`/`sw`
  snapshot at `800E9988`–`800E99A4`. Unlike the family's s16-indexed
  accessors, this slot index is an untruncated s32 argument; no common
  signed-halfword index fingerprint is inferred here.
- The immediate predecessor, `src/overlays/ovl_11/ovl_11_func_800E9838.c`,
  is an existing family member that also snapshots signed halfwords into
  guarded slots of the same table. The immediate successor,
  `src/overlays/ovl_11/ovl_11_func_800E99EC.c`, is an existing slot consumer.
  `configs/splat/ovl_11.yaml` places them consecutively at offsets
  `0x31A18`, `0x31AF4` and `0x31BCC`: `800E9838 + 0xDC = 800E9914`,
  and the target's original 0xD8-byte extent ends at `800E99EC`.
- `build/ovl_11/asm/data/168C.rodata.s` registers those same three handlers
  consecutively at `800B9884`/`800B9888`/`800B988C`, agreeing with text order.
  This is corroborating registration order, not a direct call edge.

## Limits

An overlay-private global need not be TU-private, and the adjacent handlers
do not call one another. No shared static, declaration-order effect or
register-variable quirk is established. The distant call to
`ovl_11_func_800D60F0` and the read of engine-global `D_80070C88` do not
merge their users into this TU hypothesis. The evidence adds a candidate
member and a local adjacency tie, not a single-TU claim for the entire
accessor band.
