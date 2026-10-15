# ovl_11_func_801007AC: D_801273DC handler-family extension

## Evidence identity

Documentation of the supplied byte-exact, finalize-gated source/evidence identity
`98163c2790d1b56317b16a418defcc4147d9102ea6f1555b772124224e664e5c`
(origin: agent). The gate is supplied by the handoff, not rerun here. Reviewed
changed files are `src/overlays/ovl_11/ovl_11_func_801007AC.c`,
`include/globals_override.h` and `include/overlays/ovl_11.h`; none was modified
by this documentation step.

## New membership evidence

The existing `D_801273DC` handler-table family gains candidate member
`ovl_11_func_801007AC`:

- **Shared selector and state-block handoff:** the verified target reads
  `D_801273EC` and calls `ovl_11_func_80100B50(selector, 0)`, as does existing
  member `src/overlays/ovl_11/ovl_11_func_80100644.c`. Its choice-1 arm copies
  that selector into `D_801273E6` (original store at 0x80100818) and installs
  8010289C in `D_801273DC` (store at 0x80100880). The installed handler reads
  `D_801273E6` at 0x801028B8. The established reset member
  `src/overlays/ovl_11/ovl_11_func_800FFDCC.c` clears `D_801273DC` and
  `D_801273EC` and initializes `D_801273E6` to 0xFF. This ties the new
  producer to the existing shared state block, not just a common callee.
- **Reciprocal transitions corroborate link order:** the selection handler
  8010021C installs 801007AC at 0x80100410 on its nonzero-80100A8C path;
  the target's choice-3 arm reinstalls 8010021C. Its default installs the
  family's common exit handler 80103714. `configs/splat/ovl_11.yaml` places
  8010021C, 80100644, 801006F8 and 801007AC consecutively at offsets
  0x483FC, 0x48824, 0x488D8 and 0x4898C. Their event cases advance
  0x3B8/0x3B9/0x3BA/0x3BB, and 801006F8's 0xB4-byte extent ends exactly
  at the target. The shared selector and transition graph therefore agree
  with the gapless case-handler run.

Original instruction evidence:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_801007AC/ovl_11_func_801007AC.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010021C/ovl_11_func_8010021C.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010289C/ovl_11_func_8010289C.s`

8010021C and 8010289C remain assembly-backed source stubs; their observations
above come from the original instructions. Prior adjacent-member evidence is
in [ovl_11_func_801006F8-grouping.md](ovl_11_func_801006F8-grouping.md).

## Limits

This supports candidate same-TU membership conditional on the existing
shared-state family premise; confidence in this extension is medium. Absolute
addressing and indirect transitions do not establish private linkage or original
TU boundaries. Calls to 80100B50/80100F9C and adjacency to 80100898 do not
justify merging the separate `D_80070D04` renderer/probe group into this family.
