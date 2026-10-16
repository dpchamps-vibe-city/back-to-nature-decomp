# ovl_17_func_800BA808 — verified backdrop renderer

The function passed exact diff (263/263 words), the full linked build, scope,
and clean-source gates. Documentation handoff identity:
`645306e4ee4d0f651d29717bbdfce4e3cc2ad2a5702e59bade28eefebb1d97f9`.

## Grouping evidence

The verified source uses the same `D_800BDA74 - 0x22C` work-area view and
`func_80015840`/`func_80015EE8` rendering pair as matched
`ovl_17_func_800BAF50`. Its original extent ends at `0x800BAC24`
(`0x800BA808 + 0x41C`), the entry of an existing display-cluster member.
This corroborates membership in the low-confidence D_800BD848 cluster;
it does not establish a translation-unit boundary or a direct call between
800BA808 and 800BAC24.

## Function and storage

- Two signed-halfword walks draw repeating tiles at 64- and 76-pixel steps.
  Additional coordinate guards draw edge sprites from the same object.
- `D_800BB5F0` contains stride-8 records: a signed halfword id at +0 and a
  signed fixed-point x at +4, terminated by id 255.
- The decoration pass selects a frame through `D_800BB5D4` and draws
  `D_800BDDF8`. The original data stores the frame indices as halfwords;
  this caller loads their low byte unsigned.

## Measured compiler result

The direct-C candidate preserved at ledger source hash
`6e907284331cfcb92847bbe616cd97a8f77c37aad5b1c49111571abac7a6f116`
had staged key `[0,0,0,4]`: only the two tile-loop allocation residuals
remained. Factoring their rendering operation into the local static inline
`drawTile` helper produced `[0,0,0,0]` and byte identity. Removing unused
locals and reading the frame table directly retained that identity.

This establishes a working clean-C operation boundary under this toolchain,
not that the original author used an inline helper. No flag override or
register pinning was needed. The experiment closures are conditional on the
measured sources and declarations, not proofs against other source forms.

### Signature caveat

This caller declares the frame selector argument of `func_80015840` as `u8`;
the matched engine definition currently declares it `s8`. The callee only
stores that argument as a byte, so its body alone cannot settle signedness.
The caller's unsigned load is observed; this match does not resolve the
project-wide declaration inconsistency.

## Evidence

- Verified source: `src/overlays/ovl_17/ovl_17_func_800BA808.c`.
- Original instructions:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800BA808/ovl_17_func_800BA808.s`.
- Original tables: `build/ovl_17/asm/data/3634.data.s`.
- Neighbour: `src/overlays/ovl_17/ovl_17_func_800BAF50.c`.
- Callee byte store: `src/func_80015840.c`.
- Measurements and preserved complete sources:
  `build/experimentLedger/ovl_17_func_800BA808.jsonl` and
  `build/experimentLedger/sources/ovl_17_func_800BA808/`.
- Initial exact helper candidate: ledger source hash
  `dc97c1fce18b2c335a296af21c1bd529352e29635f78ca4f6e5497ad4959f3c6`.
