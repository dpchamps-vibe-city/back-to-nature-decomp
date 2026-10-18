# ovl_11_func_800C893C: state-switch family membership

## Verified anchor

The supplied agent handoff identifies the verified source/evidence as
`75bbf05fb2e09ff62576b411150f49c08582e24ce2e13a5c777813e16cd59e1c`
and reports byte identity and a passed full finalize gate. Its changed-file
list is `src/overlays/ovl_11/ovl_11_func_800C893C.c`. This notes-only review
accepts that gate without modifying or re-verifying build inputs.

## Membership evidence

- **Common local dispatcher and object.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C7AC0/ovl_11_func_800C7AC0.s`,
  the call at 800C7ED0 passes the object in s2 to 800C893C. The same dispatcher
  passes that object to its immediate successor 800C8A08 at 800C7FB8 and
  existing family member 800C8AD4 at 800C7F94. This ties the target to the
  local interaction-helper cluster rather than merely to a shared callee.
- **Original field/callee idiom.** The verified source and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C893C/ovl_11_func_800C893C.s`
  gate s16@+0x8A, clear +0x6C bits 0xC000 on success, and call
  `800C7270(object, 0x65, u16@+0x38 + 0x17, -2)`. This repeats the
  field/update/call idiom of existing family members, including nearby
  800C8AD4; rejection uses `func_8002261C(2, message)`.
- **Gapless structural twin corroborates link locality.**
  `configs/splat/ovl_11.yaml` places the target, 800C8A08 and 800C8AD4 at
  offsets 0x10B1C, 0x10BE8 and 0x10CB4. The target and 800C8A08 each occupy
  0xCC bytes, so both boundaries are gapless. In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C8A08/ovl_11_func_800C8A08.s`,
  the successor has the same instruction/control-flow structure, stack
  layout, field offsets and call sequence as the target, differing in item,
  state and message constants, counter-helper callees and relocated jump
  destinations. Both request an item through 800CB540 when +0x8A is zero;
  both otherwise clear flags, update a counter, call `func_8001FABC(7)` and
  select a state through 800C7270. The common dispatcher and repeated idiom
  therefore corroborate adjacency to the already documented helper run.

## Role and limits

800C893C requests item 0x68 through 800CB540 when +0x8A is zero and
800F3A18(1) returns 1. When +0x8A is 0x68 it updates flags/counters and
selects state 0x65; other cases report message 0x19C.

Add only the target to the existing **medium-confidence candidate** 0x38/0x3C
state-switch family. The conditional premise is that the local dispatcher,
structural-twin adjacency and repeated object-update idiom reflect one
original translation unit. Original static linkage and TU boundaries remain
unproven. Reconstructed typedef reuse is not independent grouping evidence.
Calls to distant 800F397C, 800F3A18 and 800CB540 do not establish same-TU
ownership with those callees. The successor supplies corroboration; this
review does not assign additional neighbouring functions to the ledger.
