# ovl_15_func_8012F7D4 — grouping evidence

Verified handoff identity:
`d17cbbddb061fa24b74c42f78ffae6451f443b83208e6c6f99356da0521a20bf`.
The handoff reports byte identity and a passed full finalize gate. This review
changes notes only and does not rerun matching.

## New membership in the existing shared-prologue cluster

The verified body supports adding this function to the medium-confidence
`D_80053350/func_8001AC10` cluster at 0x8012ED18–0x80132934:

- `src/overlays/ovl_15/ovl_15_func_8012F7D4.c` uses the cluster's draw idiom:
  one `D_8005E3C0->field_D8` load, offsets +0x18/+0x14, and an updater
  address (`D_8005316C`) plus `D_80054BBC[0]`. Original assembly confirms
  this at 0x8012F7D8–0x8012F804 in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012F7D4/ovl_15_func_8012F7D4.s`.
- Beyond the common helper call, it shares the `D_8013759A >= 2` guard,
  `D_8013758C` input and `D_80137584` result-state byte with the listed
  `src/overlays/ovl_15/ovl_15_func_8012F8DC.c`. Both map operation result
  4 to state 0x15. The target calls `ovl_15_func_80136470`, whose original
  assembly in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_80136470/ovl_15_func_80136470.s`
  calls `MemCardSync` and `MemCardFormat`, establishing the formatting role.
- Link order and the call graph agree: `configs/splat/ovl_15.yaml` places
  listed predecessor `ovl_15_func_8012F6C0` at offset 0x18D8, the target at
  0x19EC and listed successor `ovl_15_func_8012F86C` at 0x1A84. The original
  assembly sizes (0x114 and 0x98 for predecessor and target) establish
  gapless adjacency. Both neighbours share the draw idiom and state-byte
  writes; see their sources in `src/overlays/ovl_15/` and the predecessor
  assembly in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012F6C0/ovl_15_func_8012F6C0.s`.
  The `D_80137584` dispatcher calls that same trio in consecutive arms at
  0x8012EB0C, 0x8012EB1C and 0x8012EB2C in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012EA18/ovl_15_func_8012EA18.s`.

Together these support candidate same-TU membership, not proven static linkage
or exact original boundaries; retain medium confidence. The new updater extern
is not ownership evidence, and the formatting-helper call does not justify
merging the separately recorded MemCard SDK cluster.
