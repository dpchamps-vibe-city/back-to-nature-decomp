# ovl_11_func_8011E1E8: local mode-6 texture-update grouping

## Verified anchor

The documentation handoff identifies the verified source/evidence as
`fac9b2ba37f3f424d2ed530eabe0673afd16dc29c531d3f9a6f8cf7724f31ff3`
and reports byte identity and the full finalize gate passed. Its changed-file
list names `src/overlays/ovl_11/ovl_11_func_8011E1E8.c`. This is a notes-only
review of that source and original assembly, not a new verification run.

## New grouping evidence

- **Call graph agrees with gapless link order.**
  `configs/splat/ovl_11.yaml` places 8011E1E8 at offset 0x663C8 and
  8011E45C at 0x6663C, followed by 8011E574 at 0x66754. The original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8011E1E8/ovl_11_func_8011E1E8.s`
  gives the verified helper size as 0x274, ending exactly at 8011E45C.
  In `build/ovl_11/asm/nonmatchings/ovl_11_func_8011E45C/ovl_11_func_8011E45C.s`,
  the call at 8011E530 invokes that immediately preceding helper with column,
  row and a texture index. Its nested loops cover the 7x7 `D_80074124`
  records, selecting the texture index from each record's tags/state.
  This supports a local caller/helper unit rather than adjacency alone.
- **Concrete grid-family membership.** The verified helper gates on mode 6
  at `D_8007AFF0+0x25476`, reads the 45-column pointer grid at
  `D_8007AFF0+0x23608`, and updates a cell's UVs, tpage and CLUT from
  ten-byte records in `D_801282FC`. In its original assembly the grid offset
  is split between the +0x20000 base at 8011E204–8011E208 and the +0x3608
  load at 8011E240; a search for the literal 0x23608 alone misses this reader.
  It is an additional member of the ledger's wider grid data family.

## Membership inference and limits

Candidate same-original-TU pair, **medium confidence**:
`ovl_11_func_8011E1E8` and `ovl_11_func_8011E45C`. The premise is that the
contiguous texture helper and its whole-grid caller reflect a local source
unit; adjacent separately linked units remain possible. Neither end of the
pair is a proven TU boundary.

The preceding `ovl_11_func_8011E090` also reads the grid and mode-6 gate, but
no direct call between it and this pair is established here. The shared
work-area base does not prove it belongs to the same original TU. Likewise,
the similar verified `ovl_11_func_80120F30` texture writer remains in its
separate local candidate subgroup; shared reconstructed types and `setUV4`
spelling do not justify merging the address-separated groups.

The existing `D_80123DFC` callback-table association was already recorded;
it supplies no new TU-private ownership evidence. An additional direct call
at 800C5684 in
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C55FC/ovl_11_func_800C55FC.s`
also shows that 8011E1E8 is used outside the local pair, without establishing
same-TU membership for that distant caller. No original static/global storage
class or declaration order is inferred from the reconstructed C.
