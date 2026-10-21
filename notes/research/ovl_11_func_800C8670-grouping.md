# ovl_11_func_800C8670: adjacent interaction-twin membership

## Verified anchor

The supplied agent handoff identifies the verified source/evidence as
`cc87daec6812589810931d6aee84135b7d741b3b1ae896a5e45465daea5840b6`
and reports byte identity and a passed full finalize gate. Its changed-file
list is `src/overlays/ovl_11/ovl_11_func_800C8670.c`. This notes-only review
accepts that gate without modifying or re-verifying build inputs.

## Membership evidence

- **Common local dispatcher and arguments.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C7AC0/ovl_11_func_800C7AC0.s`,
  the call at 800C81E8 passes the object in s2 and descriptor in s1 to
  800C8670. The call at 800C81FC passes the same arguments to existing
  member 800C857C. The dispatcher source at
  `src/overlays/ovl_11/ovl_11_func_800C7AC0.c` associates these calls with
  cases 41 and 40, respectively.
- **Original adjacent twin, not just reconstructed type reuse.** The
  verified source and existing member
  `src/overlays/ovl_11/ovl_11_func_800C857C.c` share the item gate,
  object/descriptor direction predicate (including descriptor values 6
  and 7), flag-0x4000 update at object+0x6C, and the same four callees in
  the same order. Their original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C8670/ovl_11_func_800C8670.s`
  and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C857C/ovl_11_func_800C857C.s`
  confirms the repeated loads, direction checks, updates and call sequence,
  with four parameter differences: item 0x68 versus 0x69, state 0x5F versus 0x5E,
  800D0EA4 mode 1 versus 0, and rejection code 0x1E4 versus 0x1E5.
- **Call graph agrees with gapless link adjacency.**
  `configs/splat/ovl_11.yaml` places 800C857C, 800C8670 and existing
  member 800C8764 at offsets 0x1075C, 0x10850 and 0x10944. The two twins'
  original assembly gives each a size of 0xF4, so 800C8670 exactly fills
  the interval between those existing members.

## Role and limits

The target selects state 0x5F for direction-compatible item-0x68
interactions, sets flag 0x4000, calls `func_8001FABC(7)` and then
`800D0EA4(descriptor[2], 1)`. A different item code reports rejection.

Add only 800C8670 to the existing **medium-confidence candidate**
0x38/0x3C state-switch family. The conditional premise is that the shared
local dispatcher, original twin idiom and gapless placement reflect one
original translation unit. Neither original static linkage nor TU
boundaries are proven. The call to distant 800D0EA4 does not establish
shared TU ownership with that callee; no other membership is inferred.
