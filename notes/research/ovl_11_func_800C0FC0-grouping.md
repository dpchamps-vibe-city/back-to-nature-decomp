# ovl_11_func_800C0FC0 — candidate input-state helper membership

## Verified handoff

- Identity: `f462e965661485661b6bd4da4b95d685bc64ed9003662ade56fe2f0cb6c9339a` (origin: agent; finalize gate passed).
- `run_output/autoloop/state.json`, completion `ovl_11_func_800C0FC0`, records that identity and verification result; the three changed source/header files still match its input hashes.
- Verified source: `src/overlays/ovl_11/ovl_11_func_800C0FC0.c`.

## New grouping evidence

The target and its immediate successor `ovl_11_func_800C11C4` are candidate
helpers of the ledger's D_80128B50 / D_80128B5C input-state family:

- Link adjacency: `configs/splat/ovl_11.yaml` places them at file offsets
  0x91A0 and 0x93A4. The target's original size is 0x204, ending exactly at
  0x800C11C4; the successor ends at 0x800C1224.
- Call-order corroboration: existing members `ovl_11_func_800C090C` and
  `ovl_11_func_800C0A4C` both call the target, then `ovl_11_func_800C11C4`.
  The former's calls are at 0x800C0980/0x800C098C; the latter's are at
  0x800C0CF8/0x800C0D1C, with two flag clears between them.
- Shared helper idiom: both clear a bounded flag range through
  `func_8001AF70`, then select one flag using a signed-halfword value.
  This is corroboration alongside adjacency and the two shared callers,
  not sufficient membership evidence on its own.

Assembly witnesses are
`build/ovl_11/asm/nonmatchings/<function>/<function>.s`, for each of the
four functions named above. The successor's C is
`src/overlays/ovl_11/ovl_11_func_800C11C4.c`.

## Limits

This supports candidate membership, not a proven original TU boundary:
the target provides no shared static or small-data ownership proof. Do not
infer membership of the intervening `ovl_11_func_800C0F84` between the
recorded input-state run and this helper pair, or merge the following
D_80122F0C lookup-table run merely because it is adjacent.
