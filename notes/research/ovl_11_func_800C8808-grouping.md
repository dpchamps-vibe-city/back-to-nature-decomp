# ovl_11_func_800C8808: local interaction-helper membership

## Verified anchor

The supplied agent handoff identifies the verified source/evidence as
`5aadb503c474dea821865e6e2e6b35da59158e54c6f128c390dd27a4bb654b98`
and reports byte identity and a passed full finalize gate. Its changed-file
list is `include/globals_override.h`, `include/sdk_types.h` and
`src/overlays/ovl_11/ovl_11_func_800C8808.c`. This notes-only review accepts
that gate without modifying or re-verifying build inputs.

## Membership evidence

- **Common local dispatcher and object.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C7AC0/ovl_11_func_800C7AC0.s`,
  the call at 800C80C8 passes the object in s2 to 800C8808, a stack vector
  copied from the descriptor's +0x1A/+0x1C/+0x1E fields as a1, and the
  descriptor in s1 as a2. The same dispatcher passes that object to existing
  family members 800C8764 at 800C8188 and 800C893C at 800C7ED0. This is a
  local helper relationship, not merely reuse of a distant service.
- **Gapless adjacency on both sides.** `configs/splat/ovl_11.yaml` places
  800C8764, 800C8808 and 800C893C at offsets 0x10944, 0x109E8 and 0x10B1C.
  Their original assembly under `build/ovl_11/asm/nonmatchings/` gives the
  predecessor size as 0xA4 and the target size as 0x134. Thus the target
  exactly fills the interval between two existing members called by the
  same dispatcher.
- **Original object/descriptor idiom corroborates the call edges.** The
  verified source and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C8808/ovl_11_func_800C8808.s`
  compare u16@object+0x38 with s16@descriptor+0x8, allowing descriptor value
  5 as a wildcard, and update flags at object+0x6C on ordinary success.
  The dispatcher's 800C8040 and 800C8158 cases compare those same fields;
  nearby helpers 800C857C/800C8670 also compare that object/descriptor pair
  and update +0x6C before calling the family's 800C7270 state setter. These
  are original loads and stores, not evidence inferred from reconstructed
  typedef reuse.

## Role and limits

800C8808 probes a record through 800BF1D0, validates its ID and the requested
object direction, then starts an interaction. Ordinary success sets flag
0x800000, stores the record ID at object+0x22 and dispatches sound; a special
record/state combination instead requests action 0x16.

Add only the target to the existing **medium-confidence candidate**
0x38/0x3C state-switch family. The conditional premise is that the common
local dispatcher, repeated object/descriptor idiom and gapless placement
between its helpers reflect one original translation unit. Original static
linkage and TU boundaries remain unproven. Unlike several neighbours, the
target does not call 800C7270 directly. Its accesses to D_8006C838 and
D_8007AFF0 do not establish private-global ownership, and calls to 800BF1D0,
800CB4E0, 800F0FB0 and 80118A68 do not assign those callees to this TU.
No additional neighbours are newly assigned.
