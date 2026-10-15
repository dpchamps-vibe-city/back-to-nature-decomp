# ovl_11_func_80100644: D_801273DC handler-family extension

## Evidence identity

Documentation of the supplied byte-exact, finalize-gated source/evidence identity
`13e5762cfdd508fc483b0a0976df933bae411628ad607eb114efc01ad6ae1da4`.
The gate is supplied by the handoff, not rerun here. Reviewed inputs include
`src/overlays/ovl_11/ovl_11_func_80100644.c`,
`include/globals_override.h` and `include/overlays/ovl_11.h`.

## New membership evidence

The existing `D_801273DC` handler-table family gains candidate members
`ovl_11_func_8010021C` and `ovl_11_func_80100644`:

- **Shared selector producer/consumer:** the original predecessor stores
  `20 * D_801273E8 + D_801273E7` into byte `D_801273EC` at 0x8010062C.
  The verified target loads that byte at 0x8010066C and passes it to
  `ovl_11_func_80100B50(selector, 0)`. The reset member
  `src/overlays/ovl_11/ovl_11_func_800FFDCC.c` clears both this selector and
  `D_801273DC`, tying the pair to the established state block.
- **Reciprocal handler installation plus link adjacency:** the predecessor
  installs 80100644 in `D_801273DC` at 0x8010035C; the target installs
  8010021C at 0x801006E4 on its `buttons & 0x70` path. Both can instead
  install 80103714. `configs/splat/ovl_11.yaml` puts the pair at offsets
  0x483FC and 0x48824; the predecessor's 0x428-byte extent ends exactly at
  the target. Thus the handler graph and link order corroborate the
  selector tie, rather than relying on adjacency alone.
- **Event-family continuity:** both begin with `func_800226D8(0)` and
  `func_80017B18(0)`, then `func_8002261C(3, case)` with adjacent cases
  0x3B8 and 0x3B9. They use the existing dispatcher in
  `src/overlays/ovl_11/ovl_11_func_800FFF7C.c`, which invokes the installed
  `D_801273DC` handler.

Original instruction evidence:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010021C/ovl_11_func_8010021C.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80100644/ovl_11_func_80100644.s`

The predecessor remains an assembly-backed source stub; these observations
come from its original instructions, not an inferred C implementation.

## Limits

This supports extending the existing candidate same-TU family, conditional on
its shared-state grouping premise. Absolute-addressed globals and indirect
transitions do not prove private linkage or recover original TU boundaries;
confidence in the extension is medium. The renderer call alone does not merge
this pair with the separate `D_80070D04` button-byte run, and no membership is
claimed for other intervening functions.
