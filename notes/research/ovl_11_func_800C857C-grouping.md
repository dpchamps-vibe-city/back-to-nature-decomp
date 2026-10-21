# ovl_11_func_800C857C: state-switch family membership

## Verified anchor

The supplied agent handoff identifies the verified source/evidence as
`75aa3aa4310795e80e3ad1a23932e0c8685ccc917a670bda9bd86b2b24b3aed5`
and reports byte identity and a passed full finalize gate. Its changed-file
list is `src/overlays/ovl_11/ovl_11_func_800C857C.c`. This notes-only review
accepts that gate without modifying or re-verifying build inputs.

## Membership evidence

- **Common dispatcher and arguments.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C7AC0/ovl_11_func_800C7AC0.s`,
  the call at 800C81FC passes the object in s2 and record in s1 to 800C857C.
  The same dispatcher passes those same arguments to existing member
  800C846C at 800C7F64 and passes the same object to 800C84DC at 800C820C
  and 800C8764 at 800C8188. This is a local interaction-helper cluster,
  not just unrelated consumers of a common state setter.
- **Repeated original field/callee idiom.** The verified source and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C857C/ovl_11_func_800C857C.s`
  gate on s16@+0x8A == 0x69, compare u16@+0x38 with the record's s16@+0x8
  (also accepting direction pairs for record values 6 and 7), and call
  `800C7270(object, 0x5E, u16@+0x38 + 0x17, -2)`. Rejection uses
  `func_8002261C(2, 0x1E5)`. Existing member
  `src/overlays/ovl_11/ovl_11_func_800C846C.c` and its original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C846C/ovl_11_func_800C846C.s`
  share the signed item gate, object/record direction comparison,
  state-change argument shape and rejection-call shape. The immediately
  preceding 800C84DC also shares the item gate, state-change argument shape
  and `func_8001FABC(7)` call.
- **Call graph agrees with gapless link adjacency.**
  `configs/splat/ovl_11.yaml` places 800C84DC at offset 0x106BC and 800C857C
  at 0x1075C. The predecessor's original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C84DC/ovl_11_func_800C84DC.s`
  has size 0xA0, ending with its delay-slot instruction at 800C8578. Thus
  800C857C immediately follows an existing member with the same dispatcher
  and repeated object-update idiom.

## Role and limits

The target selects state 0x5E for direction-compatible item-0x69 interactions,
setting object flag 0x4000, calling `func_8001FABC(7)` and then
`800D0EA4(record[2], 0)`. A different item code reports rejection.

This supports adding only 800C857C to the existing **medium-confidence
candidate** 0x38/0x3C state-switch family. The conditional premise is that
this local dispatcher/helper arrangement, repeated field/callee idiom and
link adjacency reflect one original source unit. Original static linkage
and TU boundaries remain unproven. Reconstructed typedef reuse is not
independent evidence; the original loads and calls provide corroboration.
The call to distant 800D0EA4 does not establish shared TU ownership with
that callee. No successor or other unlisted functions are newly assigned.
