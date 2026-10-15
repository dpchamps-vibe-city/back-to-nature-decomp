# ovl_11_func_801006F8: D_801273DC handler-family extension

## Evidence identity

Documentation of the supplied byte-exact, finalize-gated source/evidence identity
`0e8021a7425ea09e1d7b4f013ad502765f650a65ad5850de605175e5dec9b86c`
(origin: agent). The gate is supplied by the handoff, not rerun here. Its changed
files are `src/overlays/ovl_11/ovl_11_func_801006F8.c` and
`include/overlays/ovl_11.h`; both were reviewed without modification.

## New membership evidence

The existing `D_801273DC` handler-table family gains candidate member
`ovl_11_func_801006F8`:

- **Shared selector and adjacent handler template:** the verified target and
  `src/overlays/ovl_11/ovl_11_func_80100644.c` have the same handler body apart
  from event case 0x3B9 versus 0x3BA and renderer mode 0 versus 1. Both read
  `D_801273EC`, pass it to `ovl_11_func_80100B50`, and install either
  80103714 or 8010021C in `D_801273DC` after the same button tests. The
  target's selector load is at 0x80100720 and its handler store at 0x80100798.
  `include/globals_override.h` identifies `Ovl11_D_801273DC` as a named view
  of that same symbol. The existing reset member
  `src/overlays/ovl_11/ovl_11_func_800FFDCC.c` clears both globals.
- **Handler graph corroborates link order:** the original selection handler
  8010021C installs the target at 0x801003EC when 80100A8C returns zero;
  the target's `buttons & 0x70` arm installs 8010021C again. That selection
  handler already installs the adjacent twin 80100644 on another path.
  `configs/splat/ovl_11.yaml` places 8010021C, 80100644 and 801006F8 at
  offsets 0x483FC, 0x48824 and 0x488D8. The 0x428-byte selection handler
  ends at 80100644, whose 0xB4-byte extent ends exactly at 801006F8.
  Thus the shared selector, reciprocal transitions and successive event
  cases agree with the gapless link run, rather than adjacency standing alone.

Original instruction evidence:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010021C/ovl_11_func_8010021C.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_801006F8/ovl_11_func_801006F8.s`

The selection handler remains an assembly-backed source stub; its installation
edge is observed in the original instructions. Prior family evidence is in
[ovl_11_func_80100644-grouping.md](ovl_11_func_80100644-grouping.md).

## Limits

This supports candidate same-TU membership conditional on the existing
shared-state family premise; confidence in this extension is medium. Absolute
addressing and indirect transitions do not prove private linkage or an original
TU boundary. The shared renderer call does not merge this family with the
separate `D_80070D04` renderer/probe group. No membership is inferred for the
next link neighbour, 801007AC.
