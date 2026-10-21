# ovl_11_func_800C2C74 — candidate event helper/update run

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

## Extension from verified ovl_11_func_800C3020

- Supplied verified identity: `8a23e7df2ffc777d82b8e58d62bbdfb5022678de641afc95fa604b61cfa9b73a`
  (origin: agent; full finalize gate passed).
- Verified source: `src/overlays/ovl_11/ovl_11_func_800C3020.c`
  (SHA-256 `3b203fdd1cb6c95329aba7ea6da7d4fb4e16a06fd8c67751547da95148880131`).
- `configs/splat/ovl_11.yaml` and the original assembly extend the gapless
  run: 0x800C2C74 + 0x74 = 0x800C2CE8; + 0x28C = 0x800C2F74;
  + 0xAC = 0x800C3020; + 0x8C = 0x800C30AC (exclusive end).
  Thus 800C3020 is not immediately adjacent to the original pair:
  800C2F74 is the intervening member.
- Call edges corroborate that bridge: 800C2F74 calls 800C2CE8 at
  0x800C3008; 800C2CE8 calls 800C3020 at 0x800C2EA4 and 0x800C2F08;
  800C3020 calls the run's event helper 800C2C74 at 0x800C3094.
  Each passes the same object pointer.
- The verified countdown helper admits states 2, 5 and 6 at u16@+0x1A,
  suppresses updates for bit 4 of u16@+0x1E, and calls 800C2678 when
  s16@+0x10 is already zero or 800C2C74 when decrementing it reaches zero.
  The dispatcher sets that same flag at 0x800C2E34–0x800C2E38 and clears
  it at 0x800C2EA0/0x800C2EA8 around its call to the helper. This is a
  concrete object-state producer/consumer tie, not just a shared C type.
- The intervening 800C2F74 clears transient flag 0x8000 at +0x1E and
  conditionally invokes the dispatcher; its role remains assembly-derived.

Additional assembly witnesses use the same path convention above for
`ovl_11_func_800C2F74` and `ovl_11_func_800C3020`; the dispatcher witness
supplies both calls and the flag writes.

## Limits

This supports extending the low-confidence candidate pair to the four-member
run, conditional on link order preserving original source ordering. No
shared-static/global ownership, declaration-order quirk or TU boundary is
established; the run's endpoints are not claimed as original file boundaries.
The run does not merge with the non-adjacent animation-command pair at
0x800C1F94 / 0x800C2044, the far-buffer reader pair at 0x800C3B78 /
0x800C3CCC, or 800C2678 merely because its members call them. Modern shared
argument types are not evidence of an original translation unit.
