# ovl_11_func_800D8C6C: grid-clear action grouping

## Verified anchor

The handoff reports byte identity and a passed full finalize gate for
source/evidence identity
`d4b24fa3918fa88c5a8b808c11b08cbd4bf89d1c5945c7d9dac2be19ec413674`.
Its changed-file list contains only
`src/overlays/ovl_11/ovl_11_func_800D8C6C.c`. This documentation review
changes notes only and does not rerun matching.

## New membership evidence

- **Adjacent action sibling.** The verified source and the original assembly
  for immediate predecessor `800D88D0` share the `800DAF60` entry lookup,
  `800D5868`-guarded byte-+4 state selection, and unsigned comparisons against
  `800D5E64`/`800D5ED4`. Their action predicates differ (`800D5E00` here,
  `800D5D9C` there), but both clear directional 2×2 footprints through
  `800DA390` and dispatch `800DAC08` at the corresponding corner. Both use
  main-grid row stride 0x168 and cell stride 8, including bases
  `D_80071F64 = D_80071DFC + 0x168` and
  `D_80071C94 = D_80071DFC - 0x168`. Evidence: the target source and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D88D0/ovl_11_func_800D88D0.s`.
  Existing cluster member `src/overlays/ovl_11/ovl_11_func_800D876C.c`
  also uses the lookup/state-selection/action pipeline and `800DAC08`.
- **Link order corroborates the pipeline.** `configs/splat/ovl_11.yaml`
  places `800D876C`, `800D88D0`, `800D8C6C` and `800D8FC8` consecutively
  at offsets 0x2094C, 0x20AB0, 0x20E4C and 0x211A8. Their original
  assembly under `build/ovl_11/asm/nonmatchings/<function>/<function>.s`
  gives sizes 0x164, 0x39C and 0x35C for the first three: each ends
  exactly at the next function. The target therefore sits between its
  action sibling and existing table-walker member `800D8FC8`, rather than
  being associated only through a distant global.
- **Parallel callers reinforce the sibling relationship.** Original assembly
  in the same nonmatchings directory for `800C523C` and `800C541C` calls
  the target at 800C5388 and its predecessor at 800C5568, respectively.
  Both sites follow `800D806C`, pass the same selector/coordinate pattern,
  an updated `D_80128C64` value and a halfword from object offset +0x86,
  then test for return value 1. This supports related action variants,
  not shared-TU membership of the callers themselves.

## Inference and limits

Add the target to the existing Ovl11D124Entry table cluster as a
**low-confidence same-TU candidate**. The premise is that this adjacent
lookup/state/threshold/footprint-action family reflects a local source unit.
The shared globals and helpers are not proven TU-private, so a boundary
within the run remains possible. Reconstructed types and ordinary register
allocation are not independent membership evidence. No cluster is merged or
upgraded, and no direct callback-table membership is inferred from a sibling.
