# ovl_11_func_800C8E98: adjacent item-slot helper grouping

## Verified anchor

Supplied source/evidence identity:
`cbec5497b6bed108285a3f6883644d99e02f7d7bb20de0e16a8cc929ac26bc4d`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, listing `notes/file-groupings.md` and
`src/overlays/ovl_11/ovl_11_func_800C8E98.c` as changed files. This notes-only
review accepts that gate without modifying or re-verifying build inputs.

## Membership evidence

- **Gapless counterpart to an existing member.**
  `configs/splat/ovl_11.yaml` places 800C8D9C and 800C8E98 at offsets
  0x10F7C and 0x11078. The original predecessor assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C8D9C/ovl_11_func_800C8D9C.s`,
  has size 0xFC, ending exactly at the target. Both scan eight six-byte
  entries, then gate an associated item through 800D5810: the predecessor
  uses array +0x90 / item +0x84, the target array +0xC0 / item +0x8A.
- **The same local dispatcher supplies the same object.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C9424/ovl_11_func_800C9424.s`,
  s0 is initialized to D_80071A00 at 800C9430. The dispatcher passes it
  to 800C8D9C at 800C95DC and 800C963C, and to 800C8E98 at 800C967C.
  The two adjacent helpers therefore serve parallel branches of the same
  local dispatcher, corroborating their link-order relationship.
- **Original update/copy idiom agrees with that relationship.** The verified
  target source and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C8E98/ovl_11_func_800C8E98.s`
  clear +0x6C bits 0x18000000, invoke an acquisition-helper pair, reload
  the associated item, select a state through 800C7270, copy the item to
  +0xF0 through 800D7348, set +0x6C bit 0x8000 and call func_8001FABC(7).
  The predecessor repeats this sequence with different item offsets,
  acquisition helpers and state arguments. These original field/callee
  patterns, rather than reconstructed typedef reuse, support membership.

## Role and limits

800C8E98 scans the +0xC0 entries and checks the +0x8A item. If a held item
passes 800D5810, it calls 800CB618. With entries present but that item slot
empty, it calls 800C9D98/800C9DE8; success with a newly nonzero item selects
state 0xA using u16@+0x38 + 0xB and argument -2, then copies the item to
+0xF0 and performs the shared flag/sound tail. Unlike 800C8D9C, its accepted
held-item branch does not enter the acquisition/state-change path.

Retain the target in the existing **medium-confidence candidate** 0x38/0x3C
state-switch family. The conditional premise is that the gapless counterpart
pair, shared dispatcher/object and repeated update idiom reflect one original
translation unit. Original static linkage, declaration-order effects and TU
boundaries are not established. The pair's relationship is more directly
supported than the broader family's single-TU interpretation. This does not
assign the target's other callees or its successor 800C8FA8 to the same TU.
