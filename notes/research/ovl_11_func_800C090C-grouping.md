# ovl_11_func_800C090C — shared input-state reset

## Verified handoff

- Identity: `f1710d8a044a475f30fd15966180010b2e52c26e775ba1f4332bdbbd8ba72c94` (origin: agent; full finalize gate passed).
- `run_output/autoloop/state.json`, completion `ovl_11_func_800C090C`, records that identity and passing verification; its three changed source/header files match the recorded input hashes.
- Verified source: `src/overlays/ovl_11/ovl_11_func_800C090C.c`.

## New grouping evidence

The existing D_80128B50 / D_80128B5C input-state group recorded this target
only as a D_80128B50 clearer. The verified initializer additionally supplies
the same queue/state reset fingerprint as its immediate successor,
`ovl_11_func_800C09D0`:

- Both fill 0xC8 bytes at `D_80128A88` with -1, clear the adjacent s32 words
  `D_80128B54`, `D_80128B50`, `D_80128B58` in that order, and clear the
  `D_8006C838` fields s32@+0xD4 and s16@+0xD0. The target therefore supplies
  a second witness for the queue-to-B5x cluster bridge previously attributed
  to 09D0, not merely another B50 access.
- Original assembly witnesses: the target's memset call is at 0x800C095C
  and its B54/B50/B58 stores are at 0x800C0974/0x800C097C/0x800C0984;
  09D0's corresponding call is at 0x800C09F4 and its stores are at
  0x800C0A10/0x800C0A18/0x800C0A1C. See
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800C090C/ovl_11_func_800C090C.s`
  and `build/ovl_11/asm/nonmatchings/ovl_11_func_800C09D0/ovl_11_func_800C09D0.s`;
  the latter also has source at `src/overlays/ovl_11/ovl_11_func_800C09D0.c`.
- The existing zero-gap link adjacency corroborates that shared reset:
  the target's 0xC4-byte body ends at 0x800C09D0. The corresponding offsets
  in `configs/splat/ovl_11.yaml` are 0x8AEC and 0x8BB0.

## Limits

This strengthens the existing medium-confidence candidate membership; it
does not establish an original TU boundary or static ownership. Both routines
can access these absolute-addressed globals from separate TUs. The helper-call
pair is already recorded in `ovl_11_func_800C0FC0-grouping.md`; it is not new
evidence here. Do not expand the group or merge the following D_80122F0C
lookup-table family on the strength of this initializer's calls alone.
