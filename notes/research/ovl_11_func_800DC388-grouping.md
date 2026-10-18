# ovl_11_func_800DC388: ID resolver/caller grouping

## Evidence

The verified source is `src/overlays/ovl_11/ovl_11_func_800DC388.c`;
the supplied source/evidence identity is
`9320a861e62ba2aaa02ff16f0a90842370f53f18fd69ebef5223e70b4f29a05e`.
The handoff reports byte identity and a passed full finalize gate. This note
records grouping evidence only; it does not repeat verification.

- **Gapless link order:** `configs/splat/ovl_11.yaml` places 800DC388 at
  offset 0x24568 and 800DC560 immediately next at 0x24740. The original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800DC388/ovl_11_func_800DC388.s`
  gives size 0x1D8 and ends with the return delay slot at 800DC55C:
  0x800DC388 + 0x1D8 = 0x800DC560.
- **Direct caller and result flow:**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800DC560/ovl_11_func_800DC560.s`
  loads `D_8007AFF0+0x2547C` at 800DC5AC, calls 800DC388 at 800DC5B0,
  then stores the resolved ID back to that slot at 800DC5C4 before
  record-dependent setup.
- **Related flag handling:** the resolver queries `func_8001AF44(0x42)`
  and `(0x43)` at 800DC4A0 and 800DC51C. Its adjacent caller queries the
  same flags at 800DC618 and 800DC64C and clears them through
  `func_8001AF70` at 800DC644 and 800DC67C while setting coordinates.
  This supports a related operation, alongside the direct call and adjacency.

## Membership and limits

800DC388 and 800DC560 are a **low-confidence candidate same-TU pair**, assuming
this helper/caller adjacency lies within an original object rather than across
an object boundary. No shared static, local symbol visibility or declaration-
order effect establishes that boundary. 800DC560 remains an assembly stub.

The new `Ovl11MessageState` view in `include/globals_override.h` documents
field offsets, not original ownership. Shared access to the broad
`D_8006C838`/`D_8007AFF0` work areas alone does not join this pair to other
ledger families, including the distant +0x44D2 readers. Nor do calls to
800DD274, 800E3D88 or 800C141C establish membership in those callees' units.
No extension to predecessor 800DC148 or successor 800DC990 is proposed.
