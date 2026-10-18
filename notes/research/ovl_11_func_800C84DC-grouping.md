# ovl_11_func_800C84DC: state-switch family membership

## Verified anchor

The supplied agent handoff identifies the verified source/evidence as
`a75938144229944d62131db329fd64755377df153828dcff0fc1fc60d4418532`
and reports byte identity and a passed full finalize gate. Its changed-file
list is `src/overlays/ovl_11/ovl_11_func_800C84DC.c`. This notes-only review
accepts that gate without modifying or re-verifying build inputs.

## Membership evidence

- **Common dispatcher and object.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C7AC0/ovl_11_func_800C7AC0.s`,
  the call at 800C820C passes the object in s2 to 800C84DC. The same dispatcher
  passes that object to existing members 800C846C at 800C7F64, 800C8334 at
  800C7FE0, 800C83FC at 800C8008, 800C838C at 800C8030 and 800C8764 at
  800C8188. The target is another helper of this local interaction dispatcher,
  not merely an unrelated caller of the state setter.
- **Repeated original field/callee idiom.** The verified source and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C84DC/ovl_11_func_800C84DC.s`
  gate the object's s16@+0x8A on 0x3F/0x13D and call
  `800C7270(object, 0x5D, u16@+0x38 + 0x17, -2)`. The immediately preceding
  member `src/overlays/ovl_11/ovl_11_func_800C846C.c` tests the same signed
  field through 800D753C and uses the same state-change tuple with state
  0x59. Existing member `src/overlays/ovl_11/ovl_11_func_800C8764.c` also
  combines this field/argument idiom with a subsequent `func_8001FABC` call.
- **Call graph agrees with gapless link adjacency.**
  `configs/splat/ovl_11.yaml` places 800C846C at offset 0x1064C and the target
  at 0x106BC. The predecessor's original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C846C/ovl_11_func_800C846C.s`
  has size 0x70, ending with the delay-slot instruction at 800C84D8. Thus
  the target immediately follows an existing member with both a common
  dispatcher and the repeated object-update idiom.

## Role and limits

The target selects state 0x5D for item codes 0x3F/0x13D. For a zero item code,
if 800F3950(1) succeeds, it instead requests item 0x3F through 800CB540.

This supports adding only 800C84DC to the existing **medium-confidence
candidate** 0x38/0x3C state-switch family. The conditional premise is that
this local dispatcher/helper arrangement and repeated field/callee idiom
reflect one original source unit. Original static linkage and TU boundaries
remain unproven. Reconstructed typedef reuse is not independent evidence;
the original loads and calls supply the corroboration. Calls to distant
800F3898, 800F3950 and 800CB540 do not establish shared TU ownership with
those callees. No intervening or successor functions are newly assigned.
