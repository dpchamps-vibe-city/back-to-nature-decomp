# ovl_11_func_80111F88 — reset/update run extension

The agent handoff reports byte-exact source and a passed full finalize gate,
with source/evidence identity
`e40c9a28a96c02e4bdba7b08a2291502dde1bb75b48d30b2480e2bf221dc2837`.
Its changed-file list is `configs/splat/ovl_11.yaml`,
`include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_80111F88.c`.
This review is documentation only; no matching experiments or build-input
edits were performed.

## New grouping evidence

The verified body supports a low-confidence extension of the existing
`0x80111D94–0x80111F10` reset-run candidate to include `80111F88`
(all function suffixes below have the `ovl_11_func_` prefix):

- **Shared timer cluster and flag protocol.** `80111F88` decrements and
  clamps the signed halfwords at `D_8006C838+0x44D8/+0x44DA`, the exact pair
  cleared by `src/overlays/ovl_11/ovl_11_func_80111E38.c`.
  `src/overlays/ovl_11/ovl_11_func_80111E68.c` arms `+0x44DA` and sets flag
  `0x37`; the target consumes that timer and clears the same flag on expiry.
  `src/overlays/ovl_11/ovl_11_func_80111EE0.c` writes 3 to `D_80070D10`,
  the `+0x44D8` timer consumed under flag `0x38`. These are specific
  producer/reset/consumer ties, not just use of the broad shared state base.
- **Link order corroborates the data ties.** `configs/splat/ovl_11.yaml`
  places `80111F10` at offset `0x5A0F0`, the target at `0x5A168`, and
  `80112160` at `0x5A340`. The target's original listing,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80111F88/ovl_11_func_80111F88.s`,
  records size `0x1D8`, ending exactly at `80112160`. That following
  function's C source writes the same `+0x44D0/+0x44D2` stage halfwords and
  `+0x44F8` status word, and calls `80111F10`, already in the reset run.
  Thus the new timer consumer sits between an existing run member and a
  state dispatcher already connected back to that run.

## Limits

This is candidate membership, not a recovered original file boundary.
`D_8006C838` and its fields are shared game state, not established private
statics; no common declaration-order or register-variable quirk was found.
The target does not directly call its link neighbours. The following
`80112160` remains separately recorded in the `D_800719FE` cluster; these
local ties do not establish one TU spanning its distant accessor siblings.

The target's newly assigned switch-table slice at offset `0x3194` is its own
rodata, not evidence of a shared table or TU. Calls to the distant
`800D0FB4`, `800F4CEC`, `800F4CAC` and `800CD0C0` establish clients only;
they do not merge those helpers' groups into this run.
