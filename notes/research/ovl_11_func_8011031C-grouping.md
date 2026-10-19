# ovl_11_func_8011031C: grouping corroboration

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `f78a302038fe9fa4649a8a609fe65e8760d059f280138cbc0e6641095b5417ff`
and changed files `include/sdk_types.h` and
`src/overlays/ovl_11/ovl_11_func_8011031C.c`. Matching and build checks were
not repeated; this review changes only notes.

## New evidence relative to the ledger

The existing 0x582F8 far-state/reset band already lists `8011031C` in its
link-order chain, but does not record its role or its shared probe setup.
The verified function and its immediate successor `801103E8` both call
`ovl_11_func_800D0408` with the object's u16 at +0x22, a local component
record, and constant 0x64. Both then add the object's s32 +0x38 and +0x40
to the record's +0 and +8 words before passing that record to an interaction
query. This is the same calculation, not merely a shared engine callee or
parameter type. Their original assemblies confirm these accesses and
arguments; their boundaries are gapless: `0x8011031C + 0xCC = 0x801103E8`.

`8011031C` additionally clears s32 +0xDC, the action-state word also cleared
by band members `80110118`, `80110E28` and `801110CC`. It does not clear
+0xB6, so it is not another occurrence of the band's full two-field reset.
Its role is to probe the offset position using the by-value tag at +0x2E
and return `800D85F8`'s result when the collision flags include 0x20.

The original call graph supplies supporting context: `8011031C` is called
by `8010F6C0`, which also calls band members `80110118` and `80110188`;
its zero-gap successor `8010F80C` calls `801103E8`. Thus adjacent callers
use the adjacent probe pair. This corroborates the shared setup without
claiming a direct call between the probes.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_8011031C.c`
- `src/overlays/ovl_11/ovl_11_func_801103E8.c`
- `src/overlays/ovl_11/ovl_11_func_80110118.c`
- `src/overlays/ovl_11/ovl_11_func_80110E28.c`
- `src/overlays/ovl_11/ovl_11_func_801110CC.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8011031C/ovl_11_func_8011031C.s`
  (setup and reset at 0x80110324–0x8011039C)
- `build/ovl_11/asm/nonmatchings/ovl_11_func_801103E8/ovl_11_func_801103E8.s`
  (matching component setup at 0x80110424–0x80110460)
- `build/ovl_11/functions.csv` (sizes, order and caller/callee edges)
- `configs/splat/ovl_11.yaml` (consecutive offsets 0x584FC and 0x585C8)

## Limits

Assuming the repeated probe calculation and corroborating local link/call
order reflect source organization, this strengthens `8011031C`'s membership
in the existing medium-low-confidence band. Object-field overlap alone does
not establish a shared TU, and the distant engine helpers are not proposed
as members. No shared static, TU-owned global, declaration-order effect or
register-variable quirk was established. The original TU boundaries and the
extent of the wider band remain unproven; no confidence increase or new
group is warranted.
