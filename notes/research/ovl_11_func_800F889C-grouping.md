# ovl_11_func_800F889C: slot renderer in the 800F84xx candidate run

## Verified premise

The supplied handoff reports byte-exact matching and a passed full finalize
gate for `ovl_11_func_800F889C`, source/evidence identity
`50e9d1c5f790c6ff95fccffa9d80479f6d1cfbed793a839d3c98522c28d8ba2c`.
Its changed-file list names
`src/overlays/ovl_11/ovl_11_func_800F889C.c`. This notes-only review reads
that source and original disassembly; it does not rerun matching or the gate.

## New grouping evidence

The evidence supports adding the target and its range-driver caller
`800F882C` to the existing low-confidence 6-byte struct-copy helper candidate:

- The verified source reads the signed x/y pair from each 0x10-byte row of
  `D_80126E54`. The original instructions in
  `build/ovl_11/functions/ovl_11_func_800F889C.s` confirm the row stride and
  halfword loads at +0/+2. The existing candidate's matched
  `src/overlays/ovl_11/ovl_11_func_800F84F0.c` uses the same table for cursor
  coordinates and navigation links. Thus slot rendering and navigation
  share a particular menu table, not just the engine-wide object pointer.
- `build/ovl_11/functions/ovl_11_func_800F882C.s` calls the target at
  `0x800F8868`, advancing the slot index by one and the record pointer by
  six bytes on each iteration. Its last instruction is at `0x800F8898`,
  immediately before the target. This is a zero-gap caller/callee pair
  operating on the regional helpers' six-byte record layout; the target
  reads the record's ID at +0 and conditional quantity at +4.
- The existing candidate's `800F87AC` ends at `0x800F8828`, immediately before
  `800F882C` (see `build/ovl_11/functions/ovl_11_func_800F87AC.s`). This
  extends the recorded `800F84F0` / `800F87AC` link run through the driver
  and renderer, rather than inferring membership from a remote call alone.
- `build/ovl_11/functions/ovl_11_func_800F787C.s` calls the target at
  `0x800F79D0`, the range driver at `0x800F79F8` and `0x800F7A28`, navigation
  at `0x800F7A00`, and the message handler at `0x800F7A40`. The target also
  calls the existing run's classifier `800F8404` at `0x800F89E4`. These
  edges corroborate the shared-table and link-order evidence.

## Limits

These are candidate memberships, not a proven original translation-unit
boundary. The table is addressed absolutely; no static linkage, data ownership,
declaration-order effect or shared register-variable quirk was established.
The common caller remains corroborating context, not a newly claimed member.
The target's remote drawing callees are dependencies, not evidence that their
separate recorded groups should merge. In particular, its zero-gap successor
`800F8A3C` belongs to the separately recorded `D_800A0728` wrapper candidate;
this target does not directly reference that object and is not added there
merely because it calls `800F7010` and `800F7094`.
