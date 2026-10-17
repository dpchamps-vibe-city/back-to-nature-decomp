# ovl_11_func_800FB648: shared-flag run extension

## Verified handoff

The documentation handoff reports `ovl_11_func_800FB648` byte-exact and through
its full finalize gate, with source/evidence identity
`ad8d958154799dfca8920112b50e6babe22d9d4d0b6f540897fa5e557c8a625b`.
The verified source is `src/overlays/ovl_11/ovl_11_func_800FB648.c`
(SHA-256 `4a0b17adb242c95442b0b19f386ce42ea80ea82d41787bc3938fbd9cb0e6d818`).
The handoff's other changed files are `include/globals_override.h` and
`include/overlays/ovl_11.h`. This notes-only pass did not repeat the matching gate.

## New evidence relative to the recorded trio

- The verified source reads the same `s32 D_80127208` that the existing
  `800FB5FC` / `800FB608` / `800FB628` group clears, sets conditionally and
  clears conditionally, respectively. Their sources are
  `src/overlays/ovl_11/ovl_11_func_800FB5FC.c`,
  `src/overlays/ovl_11/ovl_11_func_800FB608.c` and
  `src/overlays/ovl_11/ovl_11_func_800FB628.c`.
- `build/ovl_11/functions.csv` gives the gapless run
  `800FB5FC (0xC) -> 800FB608 (0x20) -> 800FB628 (0x20) -> 800FB648 (0x2C)`.
  `configs/splat/ovl_11.yaml` corroborates it with consecutive offsets
  `0x437DC`, `0x437E8`, `0x43808`, `0x43828` and the next start `0x43854`.
  Thus the target is the immediate successor of the existing flag trio,
  not merely another distant reader of an overlay-wide global.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800FB648/ovl_11_func_800FB648.s`
  loads the flag with `lw` at `0x800FB64C`, skips the call on zero, and calls
  `ovl_11_func_800FB674` at `0x800FB65C`. This confirms the complementary
  consumer role: the flag controls whether the body is invoked.

## Grouping conclusion and limits

Extend the existing **medium-confidence candidate same-TU flag group** with
`ovl_11_func_800FB648`, on shared state plus gapless link order. The match and
new header declarations alone do not establish original TU membership.
Absolute addressing does not identify the flag's defining TU or original
linkage; neither a shared static nor a declaration-order constraint was recovered.

The direct callee `800FB674` is also the gapless successor, but its original
assembly (`build/ovl_11/asm/nonmatchings/ovl_11_func_800FB674/ovl_11_func_800FB674.s`)
does not access `D_80127208`. Leave it outside this shared-flag group; the
call/link edge is boundary context, not proof that it shares the flag's TU.
Likewise, the distant multi-helper caller `800F65B8` is not added on its call
alone. Original file boundaries remain unconfirmed.
