# ovl_11_func_800C1F94 — candidate animation-command helper/update run

## Verified handoff

- Identity: `359bc7a57ac5f6a89e2d2f049f3ae00cd4a0660e7088886f0e54e399022a5f3b` (origin: agent; finalize gate passed).
- `run_output/autoloop/state.json` records that identity and `verification: passed` for the target.
- Verified source: `src/overlays/ovl_11/ovl_11_func_800C1F94.c` (SHA-256 `2a438082e37b41e5d3fcbafb06e54703b593887045cd843ebeeed7b39d5fdf59`).

## Grouping evidence

- `configs/splat/ovl_11.yaml` puts the helper at file offset 0xA174 and
  `ovl_11_func_800C2044` immediately after it at 0xA224. The helper's
  original assembly size is 0xB0: 0x800C1F94 + 0xB0 = 0x800C2044.
- The successor directly calls the helper at 0x800C2658, passing its object
  and current command-entry pointer. Thus call graph and link order agree.
- Both clear the same object's words at +0x50/+0x54/+0x58, test s16@+0x10,
  and call `ovl_11_func_800C32C8(object, 2, s16@+6 + 1, -2)` in the
  zero-state path. These shared fields and callee idioms corroborate the
  adjacent helper/caller relation; the modern shared type alone is not
  original-TU evidence.

Original assembly witnesses are
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for
`ovl_11_func_800C1F94`, `ovl_11_func_800C2044` and `ovl_11_func_800C2C74`.
The successor remains an `INCLUDE_ASM` stub; its role is inferred from the
original assembly, not a claimed verified C implementation.

## Extension from verified ovl_11_func_800C2678

- Supplied verified identity: `0ef4426228fd7c0f56877d9b9a44ad240c605d768fa98ac04c995b8e9d177faf`
  (origin: agent; byte-exact and full finalize gate passed).
- Verified source: `src/overlays/ovl_11/ovl_11_func_800C2678.c`
  (SHA-256 `4c9f32659db1cf28fe88b28eb70c15cd570a9cb708b8ffd3bda93926c9867384`).
  The supplied changed-file list contains only that source;
  `run_output/autoloop/state.json` records the same verified identity and
  `verification: passed`.
- `configs/splat/ovl_11.yaml` places 800C2044 at file offset 0xA224 and
  800C2678 immediately afterward at 0xA858. The predecessor's original
  assembly size is 0x634: 0x800C2044 + 0x634 = 0x800C2678.
  The new member spans 0x20C bytes, ending at 0x800C2884 (exclusive).
- The verified motion helper directly calls that gapless predecessor at
  0x800C27AC and 0x800C2860, passing the object, event-table base and
  current event. This extends the existing helper/caller pair through a
  corroborated call/link edge, not proximity alone.
- Both select 0xC-byte events in 0xCC-byte rows using object halfwords
  +0xE/+0xC, and consume event coordinates +6/+0xA as object X/Z at
  +0x30/+0x38. The motion helper integrates +0x50/+0x58 into fixed-point
  +0x40/+0x48; the predecessor clears those motion deltas when advancing
  the entry. This is concrete shared state/data flow, not evidence from
  the modern shared argument type alone.
- Both index the s16 table D_80125F24 with object s16@+0x2E: the predecessor
  reads it at 0x800C2548–0x800C2560, and the new member at
  0x800C27CC–0x800C27E0. This corroborates the adjacent pair, but does not
  establish table ownership or pull in the distant reader 800C0F84.

Assembly witnesses: `build/ovl_11/functions/ovl_11_func_800C2678.s` and
`build/ovl_11/asm/nonmatchings/ovl_11_func_800C2044/ovl_11_func_800C2044.s`.
800C2678 updates X/Z motion and invokes the predecessor when proximity or
command-specific state permits advancement; 800C2044 remains a stub whose
role and shared-state accesses are supported by original assembly.

## Limits

This supports a low-confidence three-member candidate run, conditional on
link order preserving original source ordering. There is no shared-static/global
ownership or declaration-order evidence establishing a TU boundary.
`ovl_11_func_800C2C74` is another caller, at 0x800C2CCC, but is not adjacent;
that call alone does not merge the separate event-update run with this one.
The preceding `D_80076220` record-init/clear family ends at the helper's entry;
the helper touches no such global, so adjacency does not merge the families.
The new member's read of D_80122F88 and its callers in the separate event-update
run do not merge those groups. Nor do its calls alone add the non-adjacent
800C3828, 800BDB88 or 8010734C. The endpoint at 800C2884 marks the measured
extent only, not a proven original source-file boundary.
