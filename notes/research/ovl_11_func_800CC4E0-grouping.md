# ovl_11_func_800CC4E0: translation-unit grouping evidence

Documentation-only review of `src/overlays/ovl_11/ovl_11_func_800CC4E0.c`.
The supplied handoff reports byte-exact matching and a passing full finalize
gate, with source/evidence identity
`b242014b91a899f1cc6c4dcc5e41db69ec0bf4bb4319f37cea7e5f9ec39830da`.
No solver work or build-input changes were made during this review.

## New evidence

- **Shared sprite initializer/draw pair.** The verified function initializes
  `D_80128C80` from `D_800977F8` through `func_80015704`, calls
  `func_80015840` on it with argument 0x17, and sets the object's +0x6 state to 3.
  Its immediate predecessor `ovl_11_func_800CC0F8` draws that same sprite
  through `func_80015C50` at 0x800CC394 for object states 3/4; its state-3
  path also tests bit 0x100 at `D_80128C82` (sprite +2), at 0x800CC2F4.
  These are complementary producer/consumer uses, not merely references
  to a broadly shared engine work-area base.
- **Gapless link order.** Original assembly sizes give
  `800CC0F8 + 0x3E8 = 800CC4E0`,
  `800CC4E0 + 0x1C8 = 800CC6A8`, and
  `800CC6A8 + 0x124 = 800CC7CC`.
  `800CC7CC` occupies 0x3C4 bytes, ending at 0x800CCB90.
  `configs/splat/ovl_11.yaml` lists the same four functions consecutively
  at file offsets 0x142D8, 0x146C0, 0x14888 and 0x149AC.
- **Corroborating local call graph.** `800CC0F8` calls `800CC7CC` at
  0x800CC21C. That dispatcher calls `800CC4E0` at 0x800CC9D0 and
  `800CC6A8` at 0x800CCB48, linking the adjacent sprite transition and
  grid-query helpers to the update/draw driver.

Assembly evidence is in
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for the four named
functions. The matched grid helper's source is
`src/overlays/ovl_11/ovl_11_func_800CC6A8.c`.

## Limits

This supports a medium-confidence original-TU hypothesis, strongest for
`800CC0F8`/`800CC4E0`; the wider run rests on corroborated call/link adjacency.
It does not establish a private static, GP-relative ownership, a TU-specific
compiler quirk, or exact file boundaries. The two-argument sprite-loader idiom
and accesses to `D_8007AFF0`/`D_8006C838` alone do not justify merging distant
families. Likewise, the `D_8012DB80` flag's distant consumers are not added to
this group merely because the verified function writes that flag.
