# ovl_11_func_800D6F78: paired-record draw and adjacent callee

Documentation handoff: byte-exact, full finalize gate passed. Supplied
source/evidence identity:
`5940391d4a0f58f9c0ccfa0338ff0980e8ca668c983d94933c2cb445d7e639dd`.
Verified source SHA-256:
`e9c833780f02d64f744e6e0a5c62520bb7c494c715965674f8d132a9f2e3065e`.
No solver experiments or build-input edits were made for this note.

## New grouping evidence

The existing D_800A0494 cluster already includes 800D6F78 for its scan of
the first two nine-halfword rows. Its verified body adds a paired-record
consumer and a corroborated caller/link-adjacency tie:

- `src/overlays/ovl_11/ovl_11_func_800D6F78.c` uses the selected row and
  slot to read `records[row][i].unk8/unkA` through D_8007AFF0. The view in
  `include/globals_override.h` places those records at +0x254C8, namely
  D_800A04B8, with row stride 0x10E and entry stride 0x1E. This is the same
  row/slot pairing used by 800D666C and already documented for 800F0A58 in
  [its grouping note](ovl_11_func_800F0A58-grouping.md).
- The original assembly in
  `build/ovl_11/functions/ovl_11_func_800D6F78.s` confirms the record
  dimension reads at 800D707C and 800D709C, then passes the selected slot
  and row to 800D7144 at the call at 800D710C.
- That function occupies 0x1CC bytes from 800D6F78, ending immediately
  before 800D7144. Thus its callee is also its zero-gap link successor.
  `build/ovl_11/functions/ovl_11_func_800D7144.s` independently addresses
  `D_800A04B8 + row*0x10E + slot*0x1E` at 800D715C–800D71B4 and emits
  the sprite and texture-page primitives from that record. This membership
  inference uses original assembly, not its parked, non-matching C attempt.

The shared record mapping plus direct call and zero-gap adjacency support
adding 800D7144 to the existing local candidate family. They do not prove
an original TU boundary: the tables are extern globals, and this local tie
also does not establish common-TU membership with distant consumers
800F0A58 or 800F9BE4.

## Role correction

800D6F78 looks up an identifier in rows 0–1 and draws its paired sprite
record, adjusting coordinates according to flags. The row value 0/1/-1 is
local scan state, not a function return value; the verified function is void.
