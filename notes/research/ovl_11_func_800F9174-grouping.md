# ovl_11_func_800F9174: extension of the image/table caller run

## Verified premise

The supplied finalize handoff reports byte-exact matching and a passed full
gate for `ovl_11_func_800F9174`, with evidence identity
`2565a36c182204e6973c6490aeedb802e301c1a18e6ab5aff6b19f40c3e33a96`.
Its changed files are `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800F9174.c`. This documentation reads that
source and the original disassembly; no solver or new build was run.

## New membership evidence

Extend the existing low-confidence `800F9210` → `800F9BE4` candidate run
backward to include `ovl_11_func_800F9174`.

- **Gapless original link order:**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F9174/ovl_11_func_800F9174.s`
  gives size `0x9C` and a final instruction at `0x800F920C`:
  `0x800F9174 + 0x9C == 0x800F9210`. The split offsets in
  `configs/splat/ovl_11.yaml`, `0x41354` → `0x413F0`, agree.
- **Direct calls agree with adjacency:** the same original disassembly
  calls `800F9210` at `0x800F91CC` and `0x800F91EC`. The verified C calls
  `800F8404` for each category argument and dispatches only when exactly
  one result is 2, ordering the two pointed-to `u16` IDs accordingly and
  forwarding the fifth argument to the adjacent image/table handler.

## Existing run evidence retained

The prior candidate pair remains supported by
`build/ovl_11/asm/nonmatchings/ovl_11_func_800F9210/ovl_11_func_800F9210.s`:
its first call, at `0x800F9254`, reaches `800F9BE4`, and its final word at
`0x800F9BE0` ends exactly where that callee begins. The split offsets are
`0x413F0` → `0x41DC4`, a difference of `0x9D4`.

`src/overlays/ovl_11/ovl_11_func_800F9210.c` uses `D_80129638` and
`D_80129648` for image transfers and drives the D_800A0494 row helpers
`800D666C` and `800D6730`, obtaining row indices through `800D5750`.
The existing D_800A0494 ledger cluster records `800F9BE4` as the slot
reconciler that reads rows 0–1 and calls `800D666C`. The former candidate
entry also records its two calls from the remote orchestrator `800F7AA0`
(at `0x800F7C28` and `0x800F7C48`); those are dependencies, not grounds
for extending this contiguous run.

## Limits

This is a candidate same-TU run, not a proven original translation unit.
The extension rests on call/link adjacency, not shared static data,
GP-relative ownership, or a declaration-order/register-variable quirk.
The target accesses no globals directly. Its remote classifier `800F8404`
is not added on a call edge alone. Its adjacent predecessor `800F8F3C`
has no demonstrated shared data or direct call with the target, so the
[texture-draw pair](ovl_11_func_800F8F3C-grouping.md) stays separate.
Likewise, the existing run's `800F9BE4` → `800F9D3C` adjacency does not
establish membership in the following D_80126F7C menu-state run.
