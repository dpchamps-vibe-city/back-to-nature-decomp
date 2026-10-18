# ovl_11_func_80112EC4: local grid-trigger caller/helper grouping

## Verified anchor

The handoff reports byte identity and the full finalize gate passed for
source/evidence identity
`119fa552b661afa3ee9b355eab1d5044b123822ea3713373bf8c1e7ee249bf76`.
Its changed files are `src/overlays/ovl_11/ovl_11_func_80112EC4.c` and
`include/globals_override.h`. This is a notes-only review, not a new
verification run.

## New grouping evidence

- **Direct calls agree with gapless original link order.**
  `configs/splat/ovl_11.yaml` places 80112DA8 at offset 0x5AF88 and
  80112EC4 at 0x5B0A4; `configs/symbols/ovl_11.txt` records the consecutive
  addresses. The original caller in
  `build/ovl_11/functions/ovl_11_func_80112DA8.s` spans 0x11C bytes:
  `0x80112DA8 + 0x11C = 0x80112EC4`. Calls at 80112E74 and 80112E98 both
  invoke this immediate successor.
- **The shared state word connects the caller's gate to the helper's effect.**
  At 80112E18–80112E24 the caller rejects any set bit in mask 0xE000 of
  `D_8012D110 + 0x400`. The verified helper tests a byte equal to 2 in the
  same buffer's 10x10x10 grid at +0x16 and sets 0x4000 or 0x8000 in that
  word. These accesses are also visible in
  `build/ovl_11/functions/ovl_11_func_80112EC4.s` at 80112F14–80112F54.
- **Complementary plane-selection caller and cell-trigger helper.**
  The caller obtains two signed coordinates through 801136D0, tries plane
  `*(s16 *)D_8012D110 - 1` with helper mode 0 when the initial halfword is
  nonzero, then tries the current plane with mode 1 if needed. It propagates
  helper success. The verified helper uses those plane/cell coordinates to
  test the grid and latch the mode-dependent flags before its event calls.

## Membership inference and limits

Candidate same-original-TU pair, **medium confidence**:
`ovl_11_func_80112DA8` and `ovl_11_func_80112EC4`. The premise is that the
contiguous caller/helper and complementary state gate/update reflect one
source unit; separate adjacent linked units remain possible. Neither end is
a proven TU boundary. The caller remains assembly-backed; its role comes
from original instructions, not a compiled C recovery.

The wider `D_8012D110` family already shares this absolute buffer, so its
storage layout alone is not new TU evidence. Do not merge this pair with
80112D10, the 80113124/80113208 texture-update pair, or 80113B80/80113C3C
on that basis. No sole-caller, static-linkage, declaration-order or
register-variable claim is made.
