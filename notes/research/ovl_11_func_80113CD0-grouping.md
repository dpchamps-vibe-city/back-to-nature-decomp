# ovl_11_func_80113CD0 — candidate grouping evidence

## Verified premise

The supplied handoff reports byte-exact matching and a passed full finalize
gate for source/evidence identity
`c263c034fe2e678c94ed1027d3ef10558ac4c288e781a0358198305ff1f14708`
(origin: agent). Its changed-file list is `include/globals_override.h`,
`notes/file-groupings.md` and
`src/overlays/ovl_11/ovl_11_func_80113CD0.c`. This notes-only review accepts
that gate; it does not rerun matching or change build inputs.

## Membership evidence

The verified [source](../../src/overlays/ovl_11/ovl_11_func_80113CD0.c)
adds a shared-record/helper fingerprint to the existing D_8012D52C family:

- It calls `ovl_11_func_80111F10`, initialises `D_8012D538` using
  `ovl_11_func_800D6014(0x68)`, then initialises nine entries of
  `D_8012D548` after `ovl_11_func_8011D400`. It forms text pointers using
  `D_80054BBC[1]`, gates entries on `D_800719F8`, and clears `D_8012D52C`.
- The immediate successor's original disassembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80113FF0/ovl_11_func_80113FF0.s`,
  uses the same three helpers and record cluster, including the same
  pointer/halfword/byte arrays and text-base addition. Its header helper
  argument is `0x69`; it also gates an entry on status bit `0x4000` and
  clears the busy flag. These are original-disassembly observations, not
  claims that its parked, disabled C attempt matches.
- The target's original disassembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80113CD0/ovl_11_func_80113CD0.s`,
  gives size `0x320`, ending with the instruction at `0x80113FEC`:
  `0x80113CD0 + 0x320 == 0x80113FF0`. The consecutive text splits in
  [the overlay configuration](../../configs/splat/ovl_11.yaml) agree:
  `0x5BEB0` → `0x5C1D0` → `0x5C364` → `0x5C374`, corresponding to
  `80113CD0` → `80113FF0` → `80114184` → `80114194`.

Together these extend the previously documented
[earlier neighbourhood](ovl_11_func_80114194-grouping.md) backward by one
initializer. The new support is shared record/helper use corroborated by
zero-gap link order, not the busy-flag write alone.

## Source detail, not membership evidence

The verified C uses typed parallel-array pointer walks and stores the header
word at +4 through an `s32` view before reading `D_800719F8`. These source
choices are not independently established shared declaration-order or
register-variable quirks and do not raise grouping confidence.

## Limits

Membership remains candidate same-TU membership; confidence is unchanged.
`D_8012D52C` is container-wide, and the shared helpers are callees, not
thereby members of this TU. No direct call between the adjacent initialisers,
private static ownership, GP-relative ownership or declaration-order
constraint is established. A single original TU joining this earlier
neighbourhood to the later `8011A9CC`–`8011B6C0` run remains unproven.
