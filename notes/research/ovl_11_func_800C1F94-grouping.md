# ovl_11_func_800C1F94 — candidate animation-command helper/caller pair

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

## Limits

This supports a low-confidence candidate pair, conditional on link order
preserving original source ordering. There is no shared-static/global
ownership or declaration-order evidence establishing a TU boundary.
`ovl_11_func_800C2C74` is another caller, at 0x800C2CCC, but is not adjacent;
that call alone does not add it or the common callee to this pair. The
preceding `D_80076220` record-init/clear family ends at the helper's entry;
the helper touches no such global, so adjacency does not merge the families.
