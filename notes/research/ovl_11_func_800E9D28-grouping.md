# ovl_11_func_800E9D28 grouping evidence

The supplied handoff reports byte-exact source and a passed full finalize gate
(origin: agent; source/evidence identity
`c2b3a5dc282922d73e098af299b8e1ab839ddb351124caa40a5302c5cb913dfb`).
This records grouping evidence only; no matching or build inputs were changed.

## Evidence

- **Shared table and accessor interface.** Verified
  `src/overlays/ovl_11/ovl_11_func_800E9D28.c` reads and writes the
  `D_80129560` s32 slots, optionally resolves operands through them, and
  guards several result-slot writes with an index of -1. Its selector-29
  arm snapshots work-area +0xE77C into an optional slot before calling
  `ovl_11_func_800EFDA0`; selector 30 passes a mutable slot pointer to
  `ovl_11_func_800EFE34`. Existing accessor-family members
  `src/overlays/ovl_11/ovl_11_func_800E78E4.c` and
  `src/overlays/ovl_11/ovl_11_func_800E69F8.c` respectively share those
  guarded-write/compare and slot-pointer/update interfaces.
  `include/globals_override.h` identifies the table as a 0x50-byte,
  absolute-addressed ovl_11 table, not a demonstrated TU-local static.
- **Target corroboration.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E9D28/ovl_11_func_800E9D28.s`
  shows the table base and index-times-four write at 0x800EAC88–0x800EACA4,
  the compare-helper call at 0x800EACBC, and the table-slot pointer passed
  to the update helper at 0x800EACCC–0x800EACEC.
- **Gapless adjacency within the existing family.**
  `configs/splat/ovl_11.yaml` places the existing members 800E9C34 and
  800E9CE4 at file offsets 0x31E14 and 0x31EC4, then this dispatcher at
  0x31F08. The target listing
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E9CE4/ovl_11_func_800E9CE4.s`
  gives size 0x44, so 0x800E9CE4 + 0x44 = 0x800E9D28. That predecessor
  consumes the same table at stride four. Shared table use thus extends
  the recorded local accessor run by one function, rather than supplying
  only another scattered global reference.

## Conclusion and limits

Add 800E9D28 as a candidate member of the existing `D_80129560` accessor
same-TU family: its role is a state-command dispatcher with optional table
operand resolution and result-slot writes.

This supports family membership, not a proven original TU boundary. The
immediate predecessor does not call this dispatcher, and the table may be
shared across original TUs. Shared helper calls corroborate the accessor
interface, not co-location by themselves. Calls into the distant
EF488/EF6E4 record family likewise do not merge that family with this one
or establish this dispatcher's membership there.
