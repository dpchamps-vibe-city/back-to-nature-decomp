# ovl_11_func_800D759C: local mode-1 texture-update grouping

## Verified anchor

The documentation handoff identifies the verified source/evidence as
`45201940c915b2470e3b4d6c99e0d495988354968f2a368573a86c07ee39e0cf`
and reports byte identity and the full finalize gate passed. Its changed-file
list names `include/globals_override.h`, `include/overlays/ovl_11.h`, and
`src/overlays/ovl_11/ovl_11_func_800D759C.c`. This is a notes-only review of
the verified source and original assembly, not a new verification run.

## New grouping evidence

- **Call graph agrees with gapless link order.**
  `configs/splat/ovl_11.yaml` places 800D759C at offset 0x1F77C,
  800D78D0 at 0x1FAB0, and 800D79E8 at 0x1FBC8. The original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D759C/ovl_11_func_800D759C.s`
  gives the verified helper size as 0x334, ending exactly at 800D78D0.
  Its immediate successor has size 0x118 and calls the helper directly at
  800D79A4 in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D78D0/ovl_11_func_800D78D0.s`.
- **Caller and helper form a concrete whole-grid update pipeline.**
  800D78D0 walks the eight-byte `D_80071DFC` records with nested loops
  bounded by 45 columns and 25 rows (800D79AC–800D79C8). It derives a
  texture index from each record's tags/state and passes column, row and
  that index to 800D759C. The verified helper gates on mode 1 at
  `D_8007AFF0+0x25476`, reads the corresponding 45-column pointer-grid
  entry at `D_8007AFF0+0x23608`, and updates its UVs, tpage and CLUT from
  `D_80123CA0`, using `D_80123DCC` for column orientation. This supports
  a local caller/helper unit, not merely adjacent unrelated functions.

## Membership inference and limits

Candidate same-original-TU pair, **medium confidence**:
`ovl_11_func_800D759C` and `ovl_11_func_800D78D0`. The premise is that the
contiguous texture helper and its whole-grid caller reflect a local source
unit. Separate linked units remain possible; neither end of the pair is a
proven TU boundary.

The `D_80123DFC` callback association was already recorded in the ledger.
It does not establish TU-private ownership or justify merging this pair with
its distant mode-6 counterpart, 8011E1E8/8011E45C, or the wider pointer-grid
data family. Shared reconstructed types and `setUV4` spelling likewise do
not establish original declarations or a same-TU SDK idiom cluster. The
preceding range-check trio and following 800D79E8 are not added to this
pair on adjacency alone.
