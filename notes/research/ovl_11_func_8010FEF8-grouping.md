# ovl_11_func_8010FEF8: candidate predecessor to the far-state/reset band

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `bd6ad6f56a83e499773869865cc0d1a9ed56e4ecb3d877a78e67cd08cc267463`.
Its changed-file list is `configs/splat/ovl_11.yaml`,
`include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_8010FEF8.c`. Matching and build checks were
not repeated; this review changes only notes.

## New grouping evidence

The existing far-state/reset band began at `80110118`; `8010FEF8` was not
listed. Original assembly gives `8010FEF8` a size of 0x220, so its end is
exactly `0x8010FEF8 + 0x220 = 0x80110118`. The configured consecutive offsets
0x580D8 and 0x582F8 independently record the same gapless order.

This adjacency agrees with the call graph: at 0x801100EC the original function
calls its immediate successor, `ovl_11_func_80110118`, with the same object.
That call completes the seven-state cycle. Earlier, the driver stores the
record-query output halfword into object +0xB6; the successor consumes that
halfword through `800F3BCC` and clears it along with +0xDC. The verified driver
also calls existing band helpers `80110188`, `80110520`, `80110544` and
`80110944`. Thus the new tie is a local caller/reset-helper relationship
corroborated by link order, not merely a shared engine callee or parameter type.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_8010FEF8.c` (seven-state driver)
- `src/overlays/ovl_11/ovl_11_func_80110118.c` (record-id consumption and reset)
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010FEF8/ovl_11_func_8010FEF8.s`
  (0x220 size; +0xB6 store at 0x80110068; successor call at 0x801100EC;
  final instruction at 0x80110114)
- `configs/splat/ovl_11.yaml` (consecutive code offsets 0x580D8 and 0x582F8)
- `include/globals_override.h` (`Ovl11FEF8State` pool-pointer view)

## Limits

Assuming the corroborated caller/link adjacency reflects source organization,
`8010FEF8` is a low-confidence predecessor extension of the existing
medium-low-confidence band. Adjacency can cross an original TU boundary; no
shared static, TU-owned global, declaration-order effect or register-variable
quirk was established. The `D_8006C838+0xDDCC` pool-pointer view and the local
switch-table split do not independently establish same-TU membership. No
confidence increase for the core band, wider extension or inclusion of the
distant record/vector helpers is warranted.
