# ovl_11_func_8010C090: dispatcher-family slot-17 handler

## Verified handoff

The agent-origin handoff reports byte-exactness and a passed full finalize
gate for `ovl_11_func_8010C090`, with source/evidence identity
`9c887ed7cac2005c42f127aee66903d6c3ca4fdae93ad0bb90983761be0d7b09`.
Its supplied changed-file list names
`src/overlays/ovl_11/ovl_11_func_8010C090.c`.
This documentation pass read that source and the original assembly; it did
not rerun matching or change build inputs.

## New grouping evidence

- **Dispatch slot and flag agree.**
  `build/ovl_11/asm/data/2BB0.rodata.s` places the target at
  `D_800BAA34[17]` (0x800BAA78). The existing family member
  `src/overlays/ovl_11/ovl_11_func_8010B57C.c` forces selection 0x11 when
  bit 0x01000000 of the object's +0x34 word is set, then records the
  selection at +0x26. `ovl_11_func_8010B4D8.c` dispatches through that
  selection. The verified target tests the same special-state bit when
  choosing +0x24 and deciding whether to clear flag 0x800 and the +0x48
  vector. This ties its behavior to the named table's special slot, not
  merely to a common engine callee.
- **Shared flag/counter lifecycle.** The target sets flag 0x800, clears it
  when +0x7A bits 0x300 are present without the special-state bit, and
  writes 300 to s16@+0x2C unless flag 0x2000 is set. The dispatcher
  `8010B4D8` checks that same flag 0x800 and counter >= 300 immediately
  after the indirect handler call to decide whether to refresh selection.
  The original target instructions at 0x8010C14C–0x8010C18C corroborate
  these accesses in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010C090/ovl_11_func_8010C090.s`.
- **Sibling handler adjacency.** The existing candidate extension
  `8010BFEC` occupies the next table slot, `D_800BAA34[18]`, but immediately
  precedes the target in code: `0x8010BFEC + 0xA4 = 0x8010C090`
  (`build/ovl_11/functions.csv`; consecutive splat offsets 0x541CC and
  0x54270 in `configs/splat/ovl_11.yaml`). Both verified sources call
  `800D12B8`, test the three s32 fields at +0x48/+0x4C/+0x50 to select
  s16@+0x24, and manage flag 0x800; `8010BFEC` clears flag 0x800000 that
  the target conditionally sets. Table coupling and complementary behavior
  corroborate adjacency; there is no direct call edge between the pair.

## Conclusion and limits

Add the target as a **low-confidence candidate handler extension** of the
existing `D_800BA9E4` / `D_800BAA34` dispatcher family. Do not raise the
family's confidence or merge the separately recorded neighboring groups.
The slot-specific flag and counter protocol strengthens the functional
connection, and the adjacent sibling supplies local link-order evidence,
but externally visible dispatch tables can span translation units. Original
TU boundaries and static linkage remain unproven. No declaration-order or
register-variable quirk was established; the reconstructed shared type and
common engine callees alone are not original-TU evidence.
