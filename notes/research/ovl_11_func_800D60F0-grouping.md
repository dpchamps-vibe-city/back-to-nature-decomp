# ovl_11_func_800D60F0 grouping evidence

The verified function supports low-confidence candidate membership in the
existing `D_8006C858` item-table accessor family. Its shared table/field
lookup idiom is corroborated by gapless adjacency to existing member
`ovl_11_func_800D61D8`; no original TU boundary is established.

Supplied verified source/evidence identity:
`3e2dd79aba7a816682dbdccb5c565c50b688405be874c9da9d472c7f8f2fe783`.
The handoff reports byte identity and a passed full finalize gate, with
changes to `include/globals_override.h`, `include/overlays/ovl_11.h`,
`plans/100-percent-completion.md` and
`src/overlays/ovl_11/ovl_11_func_800D60F0.c`. This documentation changes
none of those files.

## Evidence

- `src/overlays/ovl_11/ovl_11_func_800D60F0.c` indexes the item table
  through `D_8006C838+0x20` (the `D_8006C858` pointer slot), using a signed
  item ID and 0x28-byte stride. It reads the signed row at item offset
  +0x0E, returning -1 for the sentinel, then indexes the packed-property
  table through the +0x34 pointer. The `ItemData` and
  `Ovl11PackedItemGlobals` views in `include/globals_override.h` express
  those accesses.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800D60F0/ovl_11_func_800D60F0.s`
  confirms the +0x20 load at `800D6110`, the `*40` multiply chain, the
  signed +0x0E load at `800D611C` and the +0x34 load at `800D6150`.
  Existing member `src/overlays/ovl_11/ovl_11_func_800D5B3C.c` shares the
  +0x20/0x28-stride lookup and signed +0x0E/-1 row test before a secondary
  table access; `build/ovl_11/functions/ovl_11_func_800D5B3C.s` confirms
  that sequence at `800D5B40`–`800D5B68`. Its secondary table is +0x24,
  not the target's +0x34.
- The immediate successor, `src/overlays/ovl_11/ovl_11_func_800D61D8.c`,
  is already a candidate accessor-family member. It shares the signed
  item ID, +0x20 pointer-slot access and 0x28-byte stride before another
  secondary-table lookup. Its original
  `build/ovl_11/functions/ovl_11_func_800D61D8.s` confirms these accesses.
  `configs/splat/ovl_11.yaml` places the pair consecutively at offsets
  `0x1E2D0` and `0x1E3B8`; the target's original 0xE8-byte extent ends
  exactly at `800D61D8`.

## Limits

These are main-binary globals, not demonstrated TU-private data. The new
+0x34 property-table view alone establishes a layout, not membership.
The candidate tie depends on the shared accessor idiom plus local link
adjacency; the two leaves do not call one another. No shared static,
declaration-order effect, register-variable quirk or SDK fingerprint is
established. The intervening helper before the target, `800D60D4`, does
not access the item table and is not added on adjacency alone. Nor does
this evidence merge distant callers of `800D60F0` into the accessor TU
hypothesis or prove the entire accessor family occupied one TU.
