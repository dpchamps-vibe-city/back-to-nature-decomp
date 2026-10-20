# ovl_11_func_800D5224 grouping evidence

Verified source/evidence identity supplied by the handoff:
`d687886d68a54bdd7ddbae02305f11d5890d1afb63fe601f651a6e8667d4b931`.
The handoff reports byte identity and a passing full finalize gate for
`src/overlays/ovl_11/ovl_11_func_800D5224.c`; this notes-only review does
not repeat verification or change build inputs.

## New membership evidence

The verified body supports low-confidence candidate same-TU membership with
its immediate successor, `ovl_11_func_800D539C`, in the existing D_800B8EC0
state-dispatch group:

- **Reciprocal state transitions:**
  `build/ovl_11/asm/data/104C.rodata.s` identifies `800D5224` as
  `D_800B8EC0[0x14]` and `800D539C` as `[7]`. The verified target calls
  `800D3104(arg0, 7)` when object flag 0x2000 clears; its original assembly
  corroborates this at 0x800D5364–0x800D537C in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D5224/ovl_11_func_800D5224.s`.
  The successor's verified source calls `800D3104(arg0, 0x14)` after
  selecting a pool slot; its original assembly corroborates the call at
  0x800D5598–0x800D55A4 in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D539C/ovl_11_func_800D539C.s`.
  These are indirect state-machine edges, not direct calls between the two
  handlers: the original `800D3104` assembly stores the selected state at
  object +0x26, and `800D3048` dispatches that field through D_800B8EC0.
  Their witnesses are the corresponding per-function `.s` files under
  `build/ovl_11/asm/nonmatchings/`.
- **Gapless code adjacency:** `build/ovl_11/ovl_11.map` places `800D5224`
  at 0x800D5224 with size 0x178, ending exactly at `800D539C`. The original
  target assembly ends at 0x800D5398 with its final four-byte instruction.
  Thus reciprocal state-machine edges corroborate original code adjacency;
  dispatch-table order alone would not establish this relationship.
- **Shared pool-selection contract:** both verified bodies use object +2
  as the pool index and +0x16 as the selector between 0xB8- and 0xB4-byte
  records. The target follows the selected record's position via
  `800D05D0`; the successor places the selected record and selects a new
  slot before returning to state 0x14. This corroborates the local handler
  relationship, but the pools and setter are not private-TU evidence.

## Limits

The inference depends on the original dispatch table, code layout and
state-selection semantics represented by these witnesses. It does not
recover a TU boundary or establish a shared static, declaration-order effect
or register-variable quirk. Modern per-function object boundaries are not
original translation-unit boundaries. This adds only `800D5224` beside the
already-recorded handler-tail pair; it does not absorb other dispatch-table
handlers, prove ownership of D_8006C838 or merge the separate progress-flag
pair merely because it precedes the target.
