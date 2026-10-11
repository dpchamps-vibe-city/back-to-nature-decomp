# ovl_11_func_800CA044: candidate object-draw helper run

## Verified premise

The supplied finalize identity is
`8b849d2720d892b29b32e81704cc77d82349a3e776df864dfcabeb35d74a1084`.
Its entry in `run_output/autoloop/state.json` records verification passed;
the current target source and two changed headers match that entry's input
hashes. This is documentation of the verified source, not a new solver or
verification run.

## Membership evidence

Candidate members: `ovl_11_func_800C9F20`, `ovl_11_func_800CA044`, and
`ovl_11_func_800CA24C`.

- **Gapless original link order.** `configs/splat/ovl_11.yaml` places them at
  offsets `0x12100`, `0x12224`, and `0x1242C`. The original assembly lengths
  are respectively `0x124`, `0x208`, and `0x334`: `0x800C9F20 + 0x124 ==
  0x800CA044`, and `0x800CA044 + 0x208 == 0x800CA24C`.
- **Call graph agrees with that run.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800CA24C/ovl_11_func_800CA24C.s`,
  the successor calls `800C9F20` at `0x800CA3E0` then `800CA044` at
  `0x800CA3F0`; another branch reverses them at `0x800CA400` and
  `0x800CA410`. Both receive the same `D_80071A00` object, `$s3` depth,
  and `D_80128B88` coordinate storage.
- **The helper roles fit the paired dispatch.** The verified
  `src/overlays/ovl_11/ovl_11_func_800CA044.c` submits the object's embedded
  sprite, with optional signed-halfword substitutions. The original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C9F20/ovl_11_func_800C9F20.s`
  selects a companion draw from object flags/type and `D_8012315C` offsets.
  Both select the ordering-table slot through
  `D_8005E3C0->field_120 + ((depth >> 2) * 4)` and consume coordinate
  halfwords at offsets `0` and `4`.

## Limits

This is a low-confidence same-TU candidate, not proof of original file
boundaries. The renderer globals are shared infrastructure; the target's
`D_8012308C/98/A4` tables alone do not tie another function to its TU.
The reconstructed argument type is not evidence of an original shared
header or declaration order. No shared static, GP-ownership boundary, or
register-variable quirk was established. The surrounding functions and
other callees are not added to this run by inference.
