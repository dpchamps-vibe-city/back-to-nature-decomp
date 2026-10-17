# ovl_11_func_800E6744 grouping evidence

The supplied byte-exact, full-finalize-gate-passed function supports a
candidate local same-TU pair with its immediate successor
`ovl_11_func_800E67D4`, already recorded in the `D_80129560` accessor family.
This is a helper-chain and link-order tie, not a new direct table accessor
or a proven original TU boundary.

Handoff source/evidence identity:
`6b8408bb3847680086556169334a1a81a0e890ce37e0fb066ca0e35c9e7395c2`.
The supplied changed files are `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800E6744.c`; neither is modified by this
documentation.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800E6744.c` resolves the signed-halfword
  `arg0`/`arg1` pair through `ovl_11_func_800EFC68`, then forwards the result
  to `ovl_11_func_800EFDA0(result, arg3, arg2)` unless its flag guard rejects
  the operation.
- `src/overlays/ovl_11/ovl_11_func_800E67D4.c` uses the same two-helper
  chain and preserves `arg2` in the comparison helper's third argument, but
  supplies `D_80129560[arg3]` rather than raw `arg3` as the comparison value.
  This makes the neighbours raw-value/table-slot counterparts, beyond
  merely sharing an overlay or one general-purpose callee.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E6744/ovl_11_func_800E6744.s`
  confirms calls at 0x800E6774 and 0x800E67B0 and a 0x90-byte extent ending
  exactly at 0x800E67D4. The successor's corresponding assembly confirms
  the same helper chain at 0x800E67F8/0x800E6818 and its table load.
- `configs/splat/ovl_11.yaml` places the pair consecutively at offsets
  0x2E924 and 0x2E9B4. Their shared call-graph shape therefore corroborates
  the gapless link adjacency.

Limits: `800E6744` does not itself reference `D_80129560`, and the neighbours
do not call each other. The flag guard (`arg0 == 0x14`,
`func_8001AF44(0x1B) == 1`, `arg3 != -1`) is a role distinction, not separate
grouping evidence. No shared static, declaration-order effect or
register-variable quirk is established. The ledger association is limited
to the local candidate pair; it does not merge the distant helpers or the
whole accessor band into one TU.
