# ovl_11_func_800E44D4 grouping evidence

Verified source: `src/overlays/ovl_11/ovl_11_func_800E44D4.c`.
The documentation handoff reports byte-exact output and a passed full finalize
gate, with source/evidence identity
`9699dc89ac407e2f8c340085e1992c613bdb901260f3bdbac7c1599ce9900186`.

## Tentative extension of the box-fixup caller run

- The original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E44D4/ovl_11_func_800E44D4.s`
  calls `ovl_11_func_800E46A8` at `0x800E4534`, after updating the entry's
  timer through `ovl_11_func_80107DEC`. The matched source confirms this is a
  scan driver for the existing group's object-relative tester.
- That assembly gives the function size as `0x94`, ending immediately before
  `0x800E4568`. `build/ovl_11/ovl_11.map` corroborates the gapless sequence
  `800E44D4 (0x94) → 800E4568 (0xA0) → 800E4608 (0xA0) → 800E46A8 (0x224)`.
  This is address-order evidence, not evidence that the reconstruction's
  per-function object splits reproduce original translation units.
- Both verified `800E44D4` and
  `src/overlays/ovl_11/ovl_11_func_800E4568.c` gate their scan drivers on
  `!(D_8006C844 & 0x800000)`. The adjacent drivers feed different testers in
  the same existing box-fixup family.

Together these support a low-confidence extension of that candidate group,
conditional on the adjacent drivers and testers having been compiled together.
Neither the common engine flag nor the call alone establishes TU membership;
no shared static, ownership fingerprint or declaration-order quirk was found.
This does not extend the group to `80107DEC` or the preceding `800E4428`.
