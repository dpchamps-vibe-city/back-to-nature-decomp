# ovl_30_func_80133218: translation-unit grouping evidence

Documentation-only review of the verified source
`src/overlays/ovl_30/ovl_30_func_80133218.c`. The handoff and
`run_output/autoloop/state.json` completion report byte-exact matching and a
passing finalize gate under source/evidence identity
`368279c73ce0a8d787f666cb1422323f9183773c83f8befa9b09a940a1cccd73`.
The source SHA-256 is
`9b9458e202f0fc2b8375e5f3d8586d8edfe210d291b15bf0137dfffdb3cde189`,
also recorded by the exact experiment in
`build/experimentLedger/ovl_30_func_80133218.jsonl`.
No solver work or build-input changes were made in this review.

## New grouping evidence

The verified function supports a previously unrecorded candidate group with
`ovl_30_func_80132EE0` and `ovl_30_func_80132F18`:

- **Call graph agrees with gapless link order.** Original assembly sizes give
  `80132EE0 + 0x38 = 80132F18`,
  `80132F18 + 0x300 = 80133218`, and
  `80133218 + 0x23C = 80133454`.
  `configs/splat/ovl_30.yaml` lists the first three consecutively at offsets
  0x50F8, 0x5130 and 0x5430. The two editor functions are the only direct
  callers of 80132EE0 found in this overlay's original function assemblies:
  80132F18 calls it at 0x801330E8, 0x80133134, 0x80133180 and 0x801331CC;
  the verified function calls it at 0x801333AC.
- **Shared editor/SDK idiom and rodata.** Both editors pass `D_8012EF2C`
  (three newlines) and the selected/unselected markers
  `D_8012DF2C`/`D_8012DF30` to `FntPrint`, apply the same helper to the
  selected value, and return their incoming argument unchanged. The
  predecessor's BUY/SELL/LIFE/TIRED format strings at 0x8012EF30–0x8012EF60
  sit directly before the verified editor's tool names and formats at
  0x8012EF6C–0x8012EF9C.
- **Adjacent overlay-local selector state.** The predecessor uses the two
  words `D_80134AB4`/`D_80134AB8`; the verified editor uses the immediately
  following words `D_80134ABC`/`D_80134AC0`, followed by its five-name pointer
  table `D_80134AC4`. Both use input bits 0x2000/0x8000 for the item selector
  and 0x1000/0x4000 for the field selector. This is a paired editor-state
  cluster, not a claim that the two functions access the same selector.

Original instruction evidence is in
`build/ovl_30/asm/nonmatchings/<function>/<function>.s` for the three named
members. Original rodata and selector/table layouts are in
`build/ovl_30/asm/data/0.rodata.s` (0x8012EF2C–0x8012EF9C) and
`build/ovl_30/asm/data/6188.data.s` (0x80134AB4–0x80134AD4).
The helper's readable source is
`src/overlays/ovl_30/ovl_30_func_80132EE0.c`.

## Limits

This supports a medium-confidence original-TU hypothesis, not exact file
boundaries. It assumes the adjacent code/data and common helper reflect
source-local editor organization; a shared cross-TU helper or pooled strings
could also explain individual ties. No private static, GP-relative ownership,
declaration-order effect or TU-specific register quirk was established.
`D_8005E3A8` and `FntPrint` alone are too broadly shared to justify membership.
The external table `D_801231C0` supplies the verified editor's values but adds
no demonstrated same-file tie.

The next function, 80133454, is already in a separate field-writer candidate
group. Mere adjacency to it does not justify merging the groups; this review
adds only the helper and its two corroborated adjacent editor callers.
