# ovl_11_func_800CA5D4: adjacent saved-object helpers

## Verified anchor

Supplied source/evidence identity:
`4fc92b57a9450b00217b51b2f3fb5fdcd963b5e33298451dd18750936d4f9b0b`
(origin: agent). The handoff reports byte identity and a passed full finalize
gate, listing `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800CA5D4.c` as changed files. This notes-only
review accepts that gate without modifying build inputs or repeating the solve.

## New membership evidence

- **Shared saved-object pointer.** The verified target loads
  `Ovl11SavedInteractionObject`, whose declaration in
  `include/globals_override.h` aliases `D_80128CF4`. The original assembly in
  `build/ovl_11/functions/ovl_11_func_800CA5D4.s` loads that pointer at
  800CA5D8/800CA5E0, writes the saved object's +0x22 halfword and updates its
  sprite at +0x78. Its immediate predecessor loads the same pointer at
  800CA584/800CA588 in
  `build/ovl_11/functions/ovl_11_func_800CA580.s`, copies four words to the
  saved object's +0x38..+0x44, then passes it to 800CFE40. These are
  complementary draw-state and animation updates of the same saved object,
  not merely a shared reconstructed parameter type.
- **Gapless placement.** `configs/splat/ovl_11.yaml` places 800CA580 and
  800CA5D4 at offsets 0x12760 and 0x127B4. The original predecessor's size is
  0x54, ending exactly at 800CA5D4. This adjacent shared-global pair was not
  previously recorded in the ledger.

## Roles and limits

Record **800CA580/800CA5D4 as a low-confidence candidate same-TU pair**:
800CA580 copies draw-state words into the saved object and draws it;
800CA5D4 synchronizes its direction and animation from the incoming object.
The conditional premise is that the shared saved-object pointer and gapless
placement reflect one original source file rather than adjacent files sharing
interaction state. Both pointer loads are absolute; neither original static
linkage nor ownership is established, and the pair has no direct call edge.

The original assemblies identify different callers: 800CA24C calls 800CA580
at 800CA4F4, while 800C70CC calls 800CA5D4 at 800C7238. This is not evidence
to merge their existing draw and state-switch groups. Other pointer users
include 800C907C and 800C580C; pointer sharing alone does not add them to this
pair. The earlier `ovl_11_func_800C907C-grouping.md` explicitly leaves this
pointer's owning TU unresolved. The adjacent 800CA6C4 is not included, and
no TU boundary or broader group's confidence changes.
