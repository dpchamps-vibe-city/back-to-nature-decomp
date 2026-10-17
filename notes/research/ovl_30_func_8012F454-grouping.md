# ovl_30_func_8012F454: translation-unit grouping evidence

Documentation-only review of the supplied passing-finalize handoff, identity
`eaf3d958335fdd594e20dd7e9e08b8489ba3fa077ce4a75a1e2a8486d3efc7b0`.
The reviewed `src/overlays/ovl_30/ovl_30_func_8012F454.c` has SHA-256
`9125db2ff6ac54cf877699a6bc2ac15ecee9b5bf2dacc219973acded42a4a052`.
No solver work, build-input edits or new build verification were performed.

## New evidence: a common caller for the mirror siblings

The previously recorded pair evidence remains: the adjacent leaves both call
`func_80020B80(2, 0)`, `func_80020B80(1, 0)` and `func_8001FBF0(k, 0)` in
that order, differing only in `k` (0x3E7 for `8012F3D4`, 0x3E8 for `8012F410`).

The existing `8012F3D4`/`8012F410` candidate pair can be extended to include
`8012F44C` and `8012F454`, at medium confidence:

- The verified `8012F454` calls `8012F410` when leaving menu row 8 and
  `8012F3D4` when entering it. The original calls are at 0x8012F6E8 and
  0x8012F738 for `8012F410`, and 0x8012F780 for `8012F3D4`. These are
  complementary operations in one menu controller, not merely two leaves
  with matching call shapes.
- At 0x8012F480 the controller also calls `8012F44C`, the empty leaf between
  the sibling pair and the controller. That gives the intervening leaf a
  direct call-graph tie rather than assuming membership from location alone.
- `configs/splat/ovl_30.yaml` and the original instruction ranges agree on
  the gapless order: `8012F3D4` + 0x3C = `8012F410`; `8012F410` + 0x3C =
  `8012F44C`; `8012F44C` + 0x8 = `8012F454`; the controller occupies
  0x998 bytes, ending at `8012FDEC` (exclusive).

Source evidence: the four corresponding files in `src/overlays/ovl_30/`.
Original instruction evidence:
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012F454/ovl_30_func_8012F454.s`,
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012F3D4/ovl_30_func_8012F3D4.s`,
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012F410/ovl_30_func_8012F410.s`
and
`build/ovl_30/asm/nonmatchings/ovl_30_func_8012F44C/ovl_30_func_8012F44C.s`.

## Limits and already-recorded ties

This candidate rests on corroborated call/link adjacency and the siblings'
complementary roles. It assumes the tightly associated controller/helpers
were kept together in the original source; calls can cross translation units,
so neither membership nor an outer TU boundary is proven. The empty leaf has
no independent shared-data or source-idiom fingerprint.

The shared `D_80134B14` bridge from `8012F454` to `8012FDEC` and `8012F084`
was already recorded in
[the parent grouping note](ovl_30_func_8012F084-grouping.md). Its initialization,
display and editor call in the verified source confirm that evidence; they
are not a new reason to merge the wider init/display cluster with this run.
The new switch-table ownership in `configs/splat/ovl_30.yaml` expresses the
verified reconstruction, not independent evidence of an original file boundary.
