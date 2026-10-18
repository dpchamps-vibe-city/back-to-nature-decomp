# ovl_11_func_80113124: local grid-texture caller/helper grouping

## Verified anchor

The handoff reports byte identity and the full finalize gate passed for
source/evidence identity
`850ab09aab93d9add7640bbe9196a0c6f1429b3d5d88931f26ff0f43a6e27c1e`.
Its changed files are `src/overlays/ovl_11/ovl_11_func_80113124.c` and
`include/overlays/ovl_11.h`. This is a notes-only review, not a new
verification run.

## New grouping evidence

- **Call graph agrees with gapless original link order.**
  `configs/splat/ovl_11.yaml` places 80113124 at offset 0x5B304 and
  80113208 at 0x5B3E8. The original assembly in
  `build/ovl_11/functions/ovl_11_func_80113124.s` spans 0xE4 bytes:
  `0x80113124 + 0xE4 = 0x80113208`. Its calls at 8011319C and 801131BC
  both invoke that immediate successor, with column, row and selector 1/2.
- **Complementary whole-grid caller and cell updater.** The verified C scans
  the selected 10x10 byte plane at `D_8012D110 + 0x16 + 100 * plane`, where
  `plane` is the initial signed halfword. Cells equal to 1 or 2 select the
  corresponding helper call. In
  `build/ovl_11/functions/ovl_11_func_80113208.s`, the helper reads
  `(row * 45 + column)` in the pointer grid at `D_8007AFF0 + 0x23608` and,
  for supported tagged entries, copies UV bytes and a CLUT halfword from
  the selected ten-byte `D_801281D0` record. These roles corroborate a
  local scan/update unit rather than adjacency alone.

## Membership inference and limits

Candidate same-original-TU pair, **medium confidence**:
`ovl_11_func_80113124` and `ovl_11_func_80113208`. The premise is that the
contiguous whole-grid caller and its per-cell texture helper reflect one
source unit; separate adjacent linked units remain possible. Neither end
is a proven TU boundary. The helper remains assembly-backed/parked; its
role here comes from original instructions, not its disabled C draft.

The grid helper also has calls from 80113310 (80113408, 801134DC, 801135B0
and 801135D4 in
`build/ovl_11/asm/nonmatchings/ovl_11_func_80113310/ovl_11_func_80113310.s`).
No sole-caller or static-linkage claim is made, and that neighbour's
membership is not assessed here. Shared `D_8012D110` storage alone does
not merge this pair with the distant state-buffer consumers; the wider
45-column grid family likewise gains no common-TU proof. No declaration
order, register-variable quirk or original static ownership is inferred.
