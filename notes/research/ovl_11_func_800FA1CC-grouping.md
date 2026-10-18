# ovl_11_func_800FA1CC: state-trio run head

## Evidence

The handoff identifies the verified source/evidence as
`b7520bb3f4ed119a2381c3f4d3b49b22e2dd546dc5ecc1c7bc293380f2df4d5a`.
This is a documentation-only assessment of that source, not a new match claim.

- `src/overlays/ovl_11/ovl_11_func_800FA1CC.c` dispatches drawing through
  `800FA31C` and `800FA3A0`, passing both the same `field_D8 + 0x54` context
  and modulo-7 value.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800FA1CC/ovl_11_func_800FA1CC.s`
  records size `0x150`, with direct calls to `800FA31C` at `0x800FA2D4` and
  `800FA3A0` at `0x800FA2E8`. Its final delay slot is at `0x800FA318`:
  `0x800FA1CC + 0x150 == 0x800FA31C`.
- `configs/splat/ovl_11.yaml` preserves the corresponding consecutive starts
  at file offsets `0x423AC`, `0x424FC`, `0x42580`, and `0x425F0` for
  `800FA1CC`, `800FA31C`, `800FA3A0`, and `800FA410`. Together with the direct
  call, the zero-gap predecessor relationship supports adding the dispatcher
  to the existing state-trio candidate TU.
- The existing members' sources provide the underlying data ties:
  `800FA31C` and `800FA410` read `D_80126F8C`; `800FA3A0` and `800FA410`
  inspect the `D_8006C838+0x5488/+0x548A/+0x548C` state trio. `800FA3A0`
  does not itself read `D_80126F8C`.

The earlier ledger described `800FA1CC` as a nearby caller, without recording
its exact boundary or membership. It also attributed a direct `800FA5C8`
call to it: the verified dispatcher instead calls `800FA558`, while
`800FA3A0` calls `800FA5C8`.

## Scope of inference

Membership remains a medium-confidence candidate, not a proven original file
boundary. The new tie is call/link-order agreement at the run head; use of the
engine-wide `D_8005E3C0` or the modulo arithmetic alone would not establish
membership. Do not infer that the other helpers called by the dispatcher
belong to this TU merely from those calls.
