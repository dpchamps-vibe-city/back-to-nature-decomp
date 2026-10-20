# ovl_11_func_800C8BA8: input gate in the state-switch family

## Verified anchor

Supplied source/evidence identity:
`642070724626ab944cff02f9da382828ed2b0c514c7c06d8d75ed1ad53bb4788`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate for `src/overlays/ovl_11/ovl_11_func_800C8BA8.c`, its sole listed changed
file. This notes-only review accepts that gate without modifying or
re-verifying build inputs; pre-existing header changes are left untouched.

## Membership evidence

- **Gapless predecessor plus a direct dispatcher edge.**
  `configs/splat/ovl_11.yaml` places existing family member 800C8AD4 and
  the target at offsets 0x10CB4 and 0x10D88. In
  `build/ovl_11/functions/ovl_11_func_800C8AD4.s`, the predecessor occupies
  0xD4 bytes, ending exactly at 800C8BA8. The target's original assembly,
  `build/ovl_11/functions/ovl_11_func_800C8BA8.s`, passes its saved object
  to 800C7AC0 at 800C8C9C when the 800BDCE8 probe fails. In
  `build/ovl_11/functions/ovl_11_func_800C7AC0.s`, that dispatcher passes
  the same incoming object to the predecessor at 800C7F94, as well as
  existing nearby members 800C893C and 800C8A08. Thus the immediate
  predecessor is in the target's local dispatch/helper graph, not merely
  adjacent in address order.
- **An existing family caller uses the same object and flag gate.**
  `build/ovl_11/functions/ovl_11_func_800C9424.s` passes D_80071A00 and
  the input halfword from D_801231F8 to the target at 800C9544, treating
  a nonzero return as handled input. Its continuation at 800C9554 repeats
  the target's `object+0x6C & 0x40000108` test. The same caller passes
  that object to existing members 800C70CC, 800C8D9C and 800C8E98.
- **Shared transition-flag idiom corroborates the call graph.**
  The target's menu-launch arms converge at 800C8C5C, ORing 0x10000000
  and then 0x08000000 into D_8006C838+0xC. In the original 800C7AC0
  assembly, 800C7E08 calls 800F66F0 and 800C7E14–800C7E34 performs the
  same two-mask update of the same word. This is a repeated local
  transition idiom alongside the object/caller ties, not evidence of
  ownership of the widely used global by itself.

## Role and limits

The target gates input on the far mode word and interaction flags, launches
one of three menus, or probes the object before delegating interaction
handling to 800C7AC0.

Add only the target to the existing **medium-confidence candidate**
0x38/0x3C state-switch family. The conditional premise is that the local
caller/dispatcher network, repeated flag idioms and gapless helper layout
reflect one original translation unit. Original static linkage and TU
boundaries remain unproven; reconstructed typedef reuse is not independent
evidence. The target itself does not access the +0x38/+0x3C pair.

The target ends at 800C8CC8, but adjacency alone does not merge that
successor's separate state-pair grouping into this family. Calls to distant
menu entries 8011E588, 800FBE4C and 800FAAD4, or to the shared 800BDCE8
probe, likewise do not establish same-TU membership for those callees.
