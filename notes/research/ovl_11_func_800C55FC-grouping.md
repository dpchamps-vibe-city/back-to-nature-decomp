# ovl_11_func_800C55FC: local grid-action dispatcher family

## Verified anchor

The documentation handoff identifies the verified source/evidence as
`1e9adc9089c4255643b43c26e79e50ce552d6ae4d1f9869fa10675f97e3c1838`
and reports byte identity and the full finalize gate passed. The verified
source is `src/overlays/ovl_11/ovl_11_func_800C55FC.c`; the handoff also lists
`include/game_types.h`, `include/globals_override.h`,
`include/overlays/ovl_11.h` and `include/sdk_types.h` as changed inputs.
This is a notes-only review, not a new verification run.

## New grouping evidence

The ledger previously mentioned 800C55FC only as the endpoint of a
"dispatch-caller run" in the tier/clamp section. The verified body and
original assembly support a more specific local helper/dispatcher candidate:

- **Shared callback table across a gapless neighbour.** Both 800C541C and
  800C55FC call `ovl_11_func_800C9D64`, exclude result 2, and dispatch through
  `D_80123234` using the remaining selector and position/column/row pointers.
  The original assembly references the table at 800C54A0–800C54BC and
  800C5638–800C5654, respectively. `build/ovl_11/asm/data/69960.data.s`
  defines its two entries as 800D7EF8 and 8011E090. This is a shared
  table/dispatch fingerprint, not merely a reconstructed common type.
- **Call graph corroborates link order.**
  `configs/splat/ovl_11.yaml` places 800C541C, 800C55FC, 800C5738 and
  800C580C at offsets 0xD5FC, 0xD7DC, 0xD918 and 0xD9EC. The first three
  original assembly sizes are 0x1E0, 0x13C and 0xD4: each ends exactly at
  the next function. In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C580C/ovl_11_func_800C580C.s`,
  the dispatcher calls 800C5738 at 800C5E8C, 800C55FC at 800C62E8 and
  800C541C at 800C65D4, passing the same object pointer from `$s1` to each.
  Thus the three contiguous helpers have a common nearby caller, rather
  than adjacency being the only connection.

Helper assembly evidence is in
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C541C/ovl_11_func_800C541C.s`,
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C55FC/ovl_11_func_800C55FC.s`
and
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C5738/ovl_11_func_800C5738.s`.

## Membership inference and limits

Candidate same-original-TU local family, **medium confidence**:
`ovl_11_func_800C541C`, `ovl_11_func_800C55FC`,
`ovl_11_func_800C5738` and `ovl_11_func_800C580C`. The premise is that
this contiguous helper/dispatcher arrangement and the neighbouring helpers'
shared callback-table idiom reflect a local source unit. Separately linked
units remain possible; neither end is a proven original TU boundary.

Earlier functions in the 800C4A2C–800C523C band also use `D_80123234`.
The evidence does not establish which of those belong to this same TU, nor
prove that the callback table was TU-private. The shared public grids
`D_80071DFC`/`D_80074124` and reconstructed object/position types alone do
not establish ownership. In particular, calls to distant grid/texture
helpers do not merge their candidate TUs with this one, and this local
family is not assigned to the tier/clamp run under which it was previously
cross-referenced.
