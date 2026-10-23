# ovl_11_func_800FE14C: threshold-renderer neighbour candidate

## Verified handoff

The documentation handoff reports `ovl_11_func_800FE14C` byte-exact and through
its exact-diff, full configured build, scope and clean-source gates, with
source/evidence identity
`5f7b982df73d2d509d7208750136b1b06d55572283f3fcadfa934eff4b77a191`.
The verified source is `src/overlays/ovl_11/ovl_11_func_800FE14C.c`, SHA-256
`57d9fe4320136d3f4a8c0ab883a648f152891b3c8dc4bff56df0f6c0aeb86cdd`.
The supplied changed-file list also contains `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800FDD74.c`. This notes-only pass did not
repeat the matching gate or change those build inputs.

## New grouping evidence

- Original assembly `build/ovl_11/functions/ovl_11_func_800FE068.s` gives
  `800FE068` a size of `0xE4`, so its end is exactly `0x800FE14C`.
  `configs/splat/ovl_11.yaml` lists them consecutively at offsets `0x46248`
  and `0x4632C`. These are link-order witnesses, not original source splits.
- Both matched renderers use the six-halfword threshold table `D_80127214`.
  `src/overlays/ovl_11/ovl_11_func_800FE068.c` compares the stored value against
  successive thresholds to select text draws. The newly matched `800FE14C`
  interpolates a gauge's fill width between neighbouring thresholds and
  selects its colours from `D_80127388` and `D_8012739C`.
- Their shared caller is independently visible in
  `build/ovl_11/functions/ovl_11_func_800FDD74.s`: calls to `800FE068` and
  `800FE14C` occur at `0x800FDE18` and `0x800FDE3C`. The matched caller passes
  both the same stored halfword from each of five rows, giving a concrete
  functional connection between the adjacent threshold renderers.
  `build/callGraph.json` corroborates those call edges; its cached pre-match
  `decompiled` flag for `800FE14C` is not used as a status witness.

## Membership and limits

`ovl_11_func_800FE14C` is a **low-confidence same-TU candidate** with its
already recorded predecessor `ovl_11_func_800FE068`: a four-edge gauge renderer
with a background and threshold-scaled gradient fill.

The new evidence combines zero-gap adjacency, a shared specialised table and
an original caller using both helpers for the same rows. It strengthens the
existing low-confidence helper extension; it does not establish an original
TU boundary or turn the larger UI family into one proven source file.
`800FE14C` does not read the parent cluster's `D_800719FE`, and the shared
`D_8005E3C0` render context is not a private-storage fingerprint. No shared
static ownership, original register-variable declaration or declaration-order
constraint was established. The earlier `800FE068` compiler-flag grouping claim
was withdrawn by its separate audit and is not evidence for this pair.
