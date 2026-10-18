# ovl_11_func_800C8AD4: state-switch family membership

## Verified anchor

The supplied agent handoff identifies the verified source/evidence as
`3de4704798b4571e2c39ec1ae311c13b6792c47caec1c72596a589451a70cd91`
and reports byte identity and a passed full finalize gate. Changed build inputs:
`src/overlays/ovl_11/ovl_11_func_800C8AD4.c` and
`include/overlays/ovl_11.h`. This notes-only review accepts that gate and does
not modify or re-verify build inputs.

## Membership evidence

- **Same local dispatcher and object.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C7AC0/ovl_11_func_800C7AC0.s`,
  the call at 800C7F94 passes the object in saved register s2 to 800C8AD4,
  together with its signed halfword at +0x8A and the other record's signed
  halfword at +0xC. The surrounding dispatch arms pass the same object to
  existing family members 800C846C (800C7F64), 800C8334 (800C7FE0),
  800C83FC (800C8008) and 800C838C (800C8030). These arms share the
  comparison of that object's u16@+0x38 against the other record's s16@+8.
- **Original field/callee idiom agrees.** The verified target and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C8AD4/ovl_11_func_800C8AD4.s`
  gate s16@+0x8A through 800D7504, then call
  `800C7270(object, 0x66, u16@+0x38 + 0x17, -2)` on success. Existing
  member `src/overlays/ovl_11/ovl_11_func_800C83FC.c` uses the same
  predicate and state-change argument idiom, with state 0x5B instead.
- **Call graph corroborates immediate link adjacency.**
  `configs/splat/ovl_11.yaml` places 800C8A08, 800C8AD4 and 800C8BA8 at
  offsets 0x10BE8, 0x10CB4 and 0x10D88. The original assembly sizes are
  0xCC for 800C8A08 and 0xD4 for the target, so both boundaries are gapless.
  The same dispatcher calls 800C8A08 at 800C7FB8. In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C8A08/ovl_11_func_800C8A08.s`,
  that predecessor also reads s16@+0x8A, clears +0x6C bit 0xC000, calls
  `800C7270(object, state, u16@+0x38 + 0x17, -2)` and uses
  `func_8002261C(2, message)` for rejection. Thus adjacency is supported
  by a common caller and repeated object-update idioms, not just addresses.

## Role and limits

800C8AD4 validates an interaction through 800D7504 and 800D0F20; on success
it clears +0x6C bits 0xC000 and selects state 0x66, returning 1. Rejection
returns 0, with a message for a zero second argument or a nonzero 800D0F20
result.

Add the target to the existing **medium-confidence candidate** 0x38/0x3C
state-switch family. The premise is that the local dispatch/helper layout
and repeated field/callee idioms reflect one original source unit. Neither
original static linkage nor TU boundaries are established. Reconstructed
typedef reuse is not independent grouping evidence, and the source's
`/* static */` annotation on the 800D0F20 declaration does not establish
original linkage. Calls to distant 800D0F20 and 800D7504 do not merge their
units into this family. The predecessor supplies local context; this review
does not assign every intervening function or the successor to the family.
