# `ovl_11_func_800BE2C4`: local collision-family placement

Documentation handoff identity:
`142b10c1948ad7e908d3174df4fc10efed84df8befe77b1043b6a46a218934b9`.
The handoff reports byte-exact source and a passed full finalize gate; this
notes-only review does not repeat matching or change build inputs.

## New grouping evidence

The verified `src/overlays/ovl_11/ovl_11_func_800BE2C4.c` supports a local
collision-family hypothesis beyond its already-recorded use of the widely
shared `D_8006C858` item table:

- It and `src/overlays/ovl_11/ovl_11_func_800BE94C.c` consume the same
  collision-context layout: position pointer at +0, hit-output pointer at +8,
  and flags-output pointer at +0x10. Both test strict X/Z bounds using signed
  halfword extents at ItemData +0x24/+0x26, narrow the computed bounds to s16,
  then record a hit and update flags.
- Their common caller builds that context at `$sp+0x10`.
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800BEB28/ovl_11_func_800BEB28.s`
  calls `800BE94C` at 0x800BEBFC and `800BE2C4` at 0x800BEC90,
  0x800BED10, 0x800BED8C, 0x800BEDCC, 0x800BEE00, 0x800BEE70 and
  0x800BEEB0. Every site passes this same context as the first argument.
- `configs/splat/ovl_11.yaml` gives the gapless order
  `800BE2C4` (0x1E4) → `800BE4A8` (0xF8) → `800BE5A0` (0x144) →
  `800BE6E4` (0x268) → `800BE94C` (0x1DC) → `800BEB28` (0x43C), ending
  at 0x800BEF64. The same caller assembly also calls all three intervening
  helpers with the context at `$sp+0x10`; the intervening code therefore
  belongs to the same collision call-graph neighbourhood, not merely an
  unexplained address gap.

## Conditional conclusion

`800BE2C4` is a low-confidence candidate same-TU sibling of the existing
`800BE94C` / `800BEB28` collision pair. Shared context and bounds idioms,
corroborated by local call/link order, support that placement; they do not
prove original TU boundaries or require every intervening helper to share
one TU. No shared static, declaration-order effect or register-variable
quirk was established. The broad item-table accessor TU remains a separate,
unproven relationship for this function.
