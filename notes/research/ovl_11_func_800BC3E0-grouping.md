# ovl_11_func_800BC3E0 — counter-state core corroboration

## Verified handoff

- Identity: `9f9eb98082608bbf8d1592d294c1de41ae8e998b060ca1fdf72f29afcdb35a92` (origin: agent).
- The supplied handoff reports byte-exact source and a passed full finalize
  gate. This is notes-only documentation, not a new verification run.
- Verified source: `src/overlays/ovl_11/ovl_11_func_800BC3E0.c`.
- Supplied changed files: that source and `configs/splat/ovl_11.yaml`.

## New grouping evidence

The existing ledger already records the dispatcher's `D_801287F4` use and
its gapless neighbours. The verified source adds a specific shared-flag
protocol and direct local calls supporting its membership in that core:

- In cases 5/13, `800BC3E0` clears `D_801217A4` immediately before calling
  its immediate link successor, `800BC868`. That helper conditionally writes
  the flag to 0 or 1. In cases 7/15, the dispatcher reads the flag and later
  calls `800BC9AC`, which also reads it to select its audio path. The helper
  sources are `src/overlays/ovl_11/ovl_11_func_800BC868.c` and
  `src/overlays/ovl_11/ovl_11_func_800BC9AC.c`.
- Call adjacency agrees with link order: `configs/splat/ovl_11.yaml` places
  `800BC3E0`, `800BC868` and `800BC9AC` at offsets 0x45C0, 0x4A48 and
  0x4B8C. Their original sizes, 0x488 and 0x144 for the first two, leave
  no gaps. The dispatcher also directly calls the already-listed
  `800BCD28` in cases 4/12.

The original assembly witness is
`build/ovl_11/asm/nonmatchings/ovl_11_func_800BC3E0/ovl_11_func_800BC3E0.s`:
`800BC50C` calls `800BCD28`; `800BC520` calls `800BC868` with the flag clear
in its delay slot at `800BC524`; `800BC560` reads the flag; `800BC670`
calls `800BC9AC`. The neighbour's original assembly at
`build/ovl_11/asm/nonmatchings/ovl_11_func_800BC868/ovl_11_func_800BC868.s`
shows its flag writes at `800BC94C` and `800BC954`.

## Limits

This corroborates the existing medium-confidence candidate core; it does
not establish an original TU boundary or ownership/private linkage of the
flag. Shared globals and caller/callee relationships can cross TU boundaries.
No additional member or distant callee is inferred, and the wrapper extension
and address-apart initializer candidates retain their existing confidence.
