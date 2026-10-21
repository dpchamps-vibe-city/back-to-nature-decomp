# ovl_11_func_800C3838 — adjacent command/heading run

## Verified handoff

- Supplied identity: `a8333dffdac906ee70d3da3fa1a6c158e582506decca8f3498670ec73d92db63`
  (origin: agent; byte-exact and full finalize gate passed).
- `run_output/autoloop/state.json`, under
  `completions.ovl_11_func_800C3838`, records that identity,
  `verification: passed`, and the sole changed file
  `src/overlays/ovl_11/ovl_11_func_800C3838.c`.
- The source SHA-256 is
  `e90c966c59a1d90d8f9ae7a3ab9d84c9fb37e8513beb5823e4dbf316b57d616f`,
  matching the recorded verification input. No solver or build-input changes
  are part of this documentation update.

## New membership evidence

`configs/splat/ovl_11.yaml` places four functions in this order:

| Function | File offset | Original size | End / next start |
| --- | --- | --- | --- |
| 800C3828 | 0xBA08 | 0x10 | 800C3838 |
| 800C3838 | 0xBA18 | 0x340 | 800C3B78 |
| 800C3B78 | 0xBD58 | 0x154 | 800C3CCC |

The verified target supplies direct calls on both sides of its own gapless
link-order position:

- At 0x800C3858 it calls immediate predecessor 800C3828. That getter loads
  the value of D_8007AFF0 and returns it plus 0x1000. The target uses the
  result as the base of 0xCC-byte command rows, selected by the object's
  current table entry, with 0xC-byte commands within each row. This is a
  concrete getter/consumer edge, not adjacency alone.
- It calls immediate successor 800C3B78 at 0x800C39D0, 0x800C3AF4 and
  0x800C3B28, passing the same object and a selected command pointer.
  The target initializes object +0x30/+0x34/+0x38 from command +6/+8/+0xA
  and sets or advances the +0xC command index. The successor consumes that
  index and the +0x30/+0x38 coordinates, choosing its heading halfword
  from command +4 or -8 and computing X/Z motion. Its command-code tests
  for 3/9/0x11/0x12 agree with the target's command handling. Thus the
  adjacent caller/helper edge also has specific command/object data flow.

Original assembly witnesses:

- `build/ovl_11/functions/ovl_11_func_800C3828.s`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800C3838/ovl_11_func_800C3838.s`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800C3B78/ovl_11_func_800C3B78.s`.

The corresponding C sources are in `src/overlays/ovl_11/`.

## Existing pair and limits

The ledger already paired 800C3B78 with its gapless successor 800C3CCC:
0xBD58 + 0x154 = 0xBEAC. Both dereference the far-buffer pointer slot at
`(u8 *)&D_8007AFF0 + 0x25388`; 800C3B78 reads pointed-object u16@+8 bit
0x1000 to choose D_80122F7C or D_80122F80 for its trigonometric motion
scaling, while 800C3CCC reads pointed-object s16@+0 to gate its record
countdown sweep. That shared slot is widely used in the overlay and is
not the same access as 800C3828's load of D_8007AFF0 itself. The old
800C3CCC association remains weaker than the new adjacent call edges.

This supports extending the existing low-confidence candidate group with
800C3838 and its getter 800C3828, conditional on link order preserving
original source ordering. It does not establish TU boundaries or ownership
of either buffer. The target's absolute D_80128B68 access and modern shared
argument types do not prove common TU membership. Its calls to 800C2678,
800C32C8, 800C3EE8 and 800C3F6C do not by themselves merge those functions
or their existing groups into this run.
