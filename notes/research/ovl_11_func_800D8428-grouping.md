# ovl_11_func_800D8428: grid-entry/callback grouping

## Verified anchor

The documentation handoff reports byte identity and the full finalize gate
passed for source/evidence identity
`8237cf9e28890164b0fff4e31c66008319e8ac9b3c6be65847c93a9d2e4c95c3`.
Its changed files are `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800D8428.c`. This review changes notes only;
it does not rerun matching or alter the verified inputs.

## New membership evidence

- **Shared entry and callback cluster.** The verified source calls
  `ovl_11_func_800DAF60(arg0, arg1, arg2)` and dispatches through
  `D_80123DFC[arg0](arg1, arg2, variant)`, just like existing members
  `ovl_11_func_800D81BC` and `ovl_11_func_800D8320`. The lookup helper's
  source, `src/overlays/ovl_11/ovl_11_func_800DAF60.c`, selects the
  `D_80071DFC` 25×45 or `D_80074124` 7×7 eight-byte records. Original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D8428/ovl_11_func_800D8428.s`
  corroborates the lookup at 800D846C and callback-table load/call at
  800D8594–800D85B0.
- **Link order reinforces the shared call graph.**
  `configs/splat/ovl_11.yaml` places 800D81BC, 800D8320, 800D8428 and
  800D85F8 at offsets 0x2039C, 0x20500, 0x20608 and 0x207D8.
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D8320/ovl_11_func_800D8320.s`
  has size 0x108, ending exactly at 800D8428; the target's size 0x1D0
  ends at 800D85F8. Thus the new member extends the already-recorded
  800D81BC/800D8320 neighbour association, rather than joining solely
  because of a distant global reference.
- **Related state transitions.** The target resets the entry's two tag
  halfwords to 0x168 or 0x169, clears bytes +4/+5 and flag bit 0x8000,
  then updates the cell through the shared callback. The verified
  `src/overlays/ovl_11/ovl_11_func_800D81BC.c` clears the same bytes/bit
  before dispatch; `src/overlays/ovl_11/ovl_11_func_800D8320.c` advances
  the same tag states before dispatch. These are neighbouring operations
  on the same record/callback pipeline.

## Inference and limits

Add 800D8428 to the existing Ovl11D124Entry table cluster and D_80123DFC
callback-table cluster, retaining **low confidence** in original-TU
membership. The premise is that the adjacent, related entry-state operations
sharing both lookup and callback reflect a local source unit. Neither global
is proven TU-private, adjacency does not exclude a TU boundary, and the
reconstructed type names are not independent grouping evidence. No original
file boundaries, SDK cluster, declaration-order effect or shared register
variable are established. The broader clusters are not merged or upgraded.
