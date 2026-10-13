# ovl_11_func_800F42A8: spawn-record reset caller

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`5fef5c95540109c5d99f79ecd2d7cdca1c158edef795932d527d01e539f59093`.
The supplied changed-file list contains
`src/overlays/ovl_11/ovl_11_func_800F42A8.c`; its SHA-256 when read was
`a7bb57ac1ad4956a83fa6ca79a54169912c516568e89019bd42719fff731459e`.
This documentation pass did not rerun verification or edit build inputs.

Evidence read:
- `src/overlays/ovl_11/ovl_11_func_800F42A8.c`: verified reset caller.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800F42A8/ovl_11_func_800F42A8.s`:
  original calls, pool slots and two bit-0 clears.
- `src/overlays/ovl_11/ovl_11_func_800F43CC.c` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F43CC/ovl_11_func_800F43CC.s`:
  original dispatch destinations and call to the fill leaf.
- `src/overlays/ovl_11/ovl_11_func_800F4360.c` and
  `src/overlays/ovl_11/ovl_11_func_800F4390.c`: complementary fill/clear
  operations on the record flag word at +4.
- `build/ovl_11/functions.csv`, rows 919–924: original link order, sizes
  and call edges.
- `notes/file-groupings.md`, existing spawn-record fill/clear/dispatch run:
  the target's adjacency and calls to `800F4390` were already recorded.

## New corroboration

The new evidence is a shared destination/flag-operation cluster, not a newly
found address adjacency. With `base = D_8006C838 + 0x8000`, `800F42A8`
unconditionally clears the pool at pointer slot +0x5DD0 using the signed
count at +0x5DFA. When its second argument is zero, it also clears the pool
at +0x5DCC using the signed count at +0x5DF8, then clears bit 0 of the
halfword at +4 in records 1 and 0 of the pool at +0x5DD4 (stride 0x18).

Those last destinations are precisely the two records selected by
`800F43CC` for identifiers 0x65 and 0x64 respectively. That dispatcher also
selects indexed records from +0x5DCC and calls `800F4360`, which sets bit 0
at +4. The target therefore resets the same specific pools and fixed pair
that the neighbouring dispatch/fill functions populate; its direct clears
also agree with the +4 bit-0 operation in its helper `800F4390`.

This corroborates membership in the existing low-confidence candidate TU
family. The already documented gapless chain is
`800F4240` (0x68) → `800F42A8` (0xB8) → `800F4360` (0x30) →
`800F4390` (0x3C) → `800F43CC` (0xE8). The call graph agrees:
`800F42A8` calls `800F4390`, while `800F43CC` calls `800F4360`.

## Limits

Membership remains conditional on this complementary reset/fill/dispatch
cluster sharing an original TU. No private static, TU-owned global,
declaration-order effect or register-variable quirk was established, and
link contiguity does not rule out a boundary between these functions.
The broad work-area symbol or a modern record-view name alone is not TU
proof. Confidence remains low; no original filename or boundary is claimed.

The target's other calls to `800F40A4`, `800F5404`, `800F5210` and
`800F508C` do not establish a merger with their separately documented
families. No distant caller or shared-work-area consumer is added here.
