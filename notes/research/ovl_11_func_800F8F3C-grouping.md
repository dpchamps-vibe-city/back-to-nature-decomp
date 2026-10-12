# ovl_11_func_800F8F3C: candidate texture-draw helper pair

## Verified premise

The supplied finalize identity is
`39092a4813e208e4c5ded2a01a17a42dd5e2a78d6e3132bcd4d7919af7c0416d`.
The target entry in `run_output/autoloop/state.json` records verification
passed and the sole changed file as
`src/overlays/ovl_11/ovl_11_func_800F8F3C.c`. Its recorded source SHA-256,
`92d8dc7fcec3d2f8ba9ef49ce75b10b3802e445afcf990abc94b48d6dc8b24e6`,
matches the source read for this documentation. No solver or new build was
run.

## New membership evidence

Candidate members: `ovl_11_func_800F8CF8` and
`ovl_11_func_800F8F3C` (medium confidence, not proven original TU boundaries).

- **Shared texture-record layout.** The verified helper draws a `POLY_FT4`
  from `D_80129648` records at stride `0x22`: texture coordinates at `+0/+2`,
  dimensions at `+8/+A`, and CLUT coordinates at `+18/+1A`. Its immediate
  predecessor indexes the same table at the same stride and consumes those
  same six fields for sprite drawing. Original evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F8CF8/ovl_11_func_800F8CF8.s`
  (`0x800F8D4C–0x800F8D64`, `0x800F8D98–0x800F8DA0`,
  `0x800F8DF4–0x800F8E04`, `0x800F8E44–0x800F8E78`) and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F8F3C/ovl_11_func_800F8F3C.s`.
- **SDK idiom corroborates the shared data.** Both draw with neutral RGB
  `(128,128,128)`, disable semitransparency, and construct texture-page bits
  from `x & 0x3C0`, `y & 0x100`, and the extra `y & 0x200` contribution.
  The predecessor emits those page bits in a draw-mode packet
  (`0x800F8EAC–0x800F8ED4`); the target places them in the quad's `tpage`
  (`0x800F9018–0x800F9044`). The verified source expresses the target's
  construction through `getTPage` macros. This supports sibling drawing
  roles, rather than relying on a shared SDK include alone.
- **Gapless original link order.** The predecessor's `0x244` bytes end
  exactly at the target: `0x800F8CF8 + 0x244 == 0x800F8F3C`.
  `configs/splat/ovl_11.yaml` independently places the two at offsets
  `0x40ED8` and `0x4111C`. The target occupies `0x238` bytes and ends at
  `0x800F9174`.

## Limits

No direct call between the pair, common caller, shared static ownership,
GP-relative ownership boundary, or declaration-order/register-variable
quirk was established. The shared layout is access evidence, not proof of
an identical original C type: the predecessor calls `GetClut` with signed
halfword loads, whereas the target uses the inline `getClut` expression
with unsigned loads.

The table also has other consumers. In particular,
`build/ovl_11/asm/nonmatchings/ovl_11_func_800F6E38/ovl_11_func_800F6E38.s`
searches its records and calls the target at `0x800F6EAC`; that remote
caller edge is a dependency, not a TU boundary. No table-wide TU is claimed.
The adjacent successor `800F9174` reads no texture table and instead
calls `800F8404` and `800F9210`, so it is not added on adjacency alone.
The existing `800F9210`/`800F9BE4` candidate and numeric-row display group
remain separate. The pair's membership inference rests only on the shared
texture layout, drawing idiom, and immediate original link adjacency.
