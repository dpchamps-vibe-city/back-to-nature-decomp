# ovl_11_func_800C2C74 — candidate event helper/caller pair

## Verified handoff

- Identity: `ccdd97a8631b9a9d70c4341ca648da6ad3dfd75bf598d34eb9e6831298ac7b05` (origin: agent; finalize gate passed).
- `run_output/autoloop/state.json` records that identity and `verification: passed` for the target.
- Verified source: `src/overlays/ovl_11/ovl_11_func_800C2C74.c` (SHA-256 `f29b50b58d99581b3eab7d156819391890fc168680ce95f1500e4f826a2468cc`, matching the recorded input).

## Grouping evidence

- `configs/splat/ovl_11.yaml` puts the helper at file offset 0xAE54 and
  `ovl_11_func_800C2CE8` immediately after it at 0xAEC8. The helper's
  original assembly size is 0x74: 0x800C2C74 + 0x74 = 0x800C2CE8.
- The successor directly calls the helper at 0x800C2EF8, passing its object
  pointer in the delay slot at 0x800C2EFC. Thus call graph and gapless link
  order agree on this helper/caller pair.

Original assembly witnesses are
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for
`ovl_11_func_800C2C74` and `ovl_11_func_800C2CE8`.

## Role evidence

The verified helper selects a 0xC-byte event using the object's signed
halfwords at +0xE (0xCC-byte row stride) and +0xC (event index), then invokes
`ovl_11_func_800C3B78` and `ovl_11_func_800C1F94` on that object/event pair.
The successor's original assembly updates object state and dispatches the
helper when the signed command countdown at +8 reaches -1. The successor
remains an `INCLUDE_ASM` stub; its role is inferred from that assembly, not
from a verified C implementation.

## Limits

This supports a low-confidence candidate pair, conditional on link order
preserving original source ordering. No shared-static/global ownership,
declaration-order quirk or TU boundary is established. The new pair does
not merge with the non-adjacent animation-command pair at 0x800C1F94 /
0x800C2044 or the far-buffer reader pair at 0x800C3B78 / 0x800C3CCC merely
because the helper calls their members. Modern shared argument types are
not evidence of an original translation unit.
