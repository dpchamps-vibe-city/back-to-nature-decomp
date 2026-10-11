# ovl_11_func_80120F30: local texture-update grouping

## Verified anchor

The documentation handoff identifies the verified source/evidence as
`3ac30968f7887c667d7463b7eca984a5c6e916a0b55e687ec8fe892776f3296d`
and reports the full finalize gate passed. The source is
`src/overlays/ovl_11/ovl_11_func_80120F30.c`;
`build/pipelineReversal/ovl_11_func_80120F30/report.json` also records exact
machine identity (129/129 words). This is a notes-only review, not a new
verification run.

## New local evidence

- **Call graph agrees with link order.** `configs/splat/ovl_11.yaml` lists
  the gapless run `80120CC4` (offset 0x68EA4, size 0x26C) → `80120F30`
  (0x69110, size 0x204) → `80121134` (0x69314, size 0xD0).
  In `build/ovl_11/asm/nonmatchings/ovl_11_func_80121134/ovl_11_func_80121134.s`,
  the caller at 8012119C invokes 80120CC4 with `D_8012DB38` as its output
  buffer. When that helper returns 1, the call at 801211B4 passes the same
  buffer to 80120F30, along with the scanned column and row. The loop covers
  25 rows by 45 columns of eight-byte records at `D_8006C838+0x55C4`.
- **Concrete producer/consumer layout.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80120CC4/ovl_11_func_80120CC4.s`
  writes eight UV bytes at output offsets 0–7 and a CLUT halfword at +8
  (80120EE8–80120F2C). The verified consumer reads precisely that layout,
  writes the grid entry's UVs and CLUT, and selects forward/reversed vertex
  order through the column's bit in `D_801287C4`. This supports a local
  texture-update pipeline, rather than merely unrelated adjacent functions.
- **Adjacent, narrowly referenced data.**
  `build/ovl_11/asm/data/69960.data.s` places the producer's halfword table
  `D_80128764` in [0x80128764, 0x801287C4), immediately before the consumer's
  eight flag bytes in [0x801287C4, 0x801287CC). A scan of the original
  `build/ovl_11/asm/nonmatchings/**/*.s` finds named references to the first
  only in 80120CC4, and to the second only in 80120F30. This is supporting
  data-cluster evidence; it does not establish their original C storage class
  or exclude references through another base.

## Membership inference and limits

Candidate same-original-TU subgroup, **medium confidence**:
`ovl_11_func_80120CC4`, `ovl_11_func_80120F30`, and
`ovl_11_func_80121134`. The premise is that the contiguous producer/consumer/
caller pipeline and adjacent helper-specific tables reflect a local source
unit. Separate linked units remain possible; neither end of this run is a
proven TU boundary.

80120F30's 45-column access to `D_8007AFF0+0x23608` was already recorded in
`notes/file-groupings.md`. That wider, address-separated grid family gains no
new same-TU proof from this local subgroup. The reconstructed types and
`setUV4` spelling alone likewise do not establish original declarations,
static ownership, or a neighbour-wide SDK idiom cluster.
