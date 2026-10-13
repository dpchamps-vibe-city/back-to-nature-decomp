# ovl_11_func_800CE96C: candidate pool reset/update pair

## Scope and identity

Documentation-only review of the byte-exact, finalize-gated source
`src/overlays/ovl_11/ovl_11_func_800CE96C.c`. Supplied source/evidence
identity: `83eb94ef559aa80ac8044bb8bb670f7d4d3c1f6980fd6eec8debc78a14bc53fe`.
No matching experiments or build-input edits were performed in this review.

## New grouping evidence

The verified reset and its immediate link successor
`ovl_11_func_800CEBC8` form a **low-confidence candidate same-TU pair**:

- Shared pool sequence: CE96C resets ten 0xB4 records at `D_800742EC`,
  walks twenty 0xB8 records at `D_800749F4` twice, and resets six 0xB0
  records at `D_80075BC4`. CEBC8 updates the same ten records, walks the
  same twenty-record array twice through different handlers, then updates
  the same six records. CE96C's two `records_81BC` loops both reach
  `D_8006C838 + 0x81BC == D_800749F4`; they are not separate pools. The
  repeated pass over this same array makes the shared sequence more
  specific than merely accessing the widely shared state buffer.
- Complementary state gate: CE96C ends by clearing mask 0x8 of
  `D_8006C838 + 0x4450`. CEBC8 reads that exact word and skips its entire
  pool-update sweep when that mask is set (`lw` at 0x800CEBE0, `andi` at
  0x800CEBE8, branch at 0x800CEBEC).
- Gapless original link order: CE96C occupies 0x25C bytes,
  `0x800CE96C + 0x25C == 0x800CEBC8`. The splat offsets independently
  place the pair at 0x16B4C and 0x16DA8.

Evidence paths:

- `src/overlays/ovl_11/ovl_11_func_800CE96C.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800CE96C/ovl_11_func_800CE96C.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800CEBC8/ovl_11_func_800CEBC8.s`
  (pool loops at 0x800CEBF8, 0x800CEC28, 0x800CEC58 and 0x800CEC88)
- `configs/splat/ovl_11.yaml` (offsets 0x16B4C / 0x16DA8 / 0x16E90)

## Limits

These are shared main-RAM pools, not proven private statics, and the pair
has no direct caller/callee edge. The inference rests on the complementary
reset/update sequence, its duplicated array pass, the shared gate and
link contiguity; adjacent original TUs could still explain it. No original
TU boundary or data ownership is established. In particular, this does not
merge the distant `D_80075BC4` consumers or the `D_80075854` helper copies
into the pair's TU. The existing ledger section is the consumer-family
container for this narrower candidate, not a claim that all its members
share one original file.
