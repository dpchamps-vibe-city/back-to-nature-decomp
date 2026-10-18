# ovl_11_func_800F7D34: record-dispatcher grouping evidence

## Verified premise

The supplied handoff reports byte-exact matching and a passed full finalize
gate for `ovl_11_func_800F7D34`, source/evidence identity
`d077d275f2e315f004866e7d740511097ffd9767f7ea698a3f29f72746c55aa4`.
Its changed-file list names `src/overlays/ovl_11/ovl_11_func_800F7D34.c`.
This notes-only review uses that source and original disassembly; it does not
repeat matching or build verification.

## New grouping evidence

The existing low-confidence 6-byte struct-copy helper group can include
`800F7D34` as a candidate dispatcher, supported by call/link-order agreement:

- `configs/splat/ovl_11.yaml` and `build/ovl_11/functions.csv` place
  `800F7AA0` (0x294 bytes), `800F7D34` (0x104 bytes), `800F7E38`
  (0x174 bytes), `800F7FAC` (0x1DC bytes), and `800F8188` in a gapless run.
- The original `build/ovl_11/functions/ovl_11_func_800F7AA0.s` calls the
  target at `0x800F7C40`. Thus the target's immediate predecessor is also
  its caller, not merely a nearby menu routine.
- `build/ovl_11/functions/ovl_11_func_800F7D34.s` calls its immediate
  successor `800F7E38` at `0x800F7DB8` and `0x800F7DE4`, and the next two
  functions `800F7FAC` and `800F8188` at `0x800F7E04` and `0x800F7E18`.
  It obtains both record pointers from the existing regional selector
  `800F82FC` (calls at `0x800F7D64` and `0x800F7D74`).
- The verified source shows that these are alternate operations on the same
  two selected records: dispatch depends on whether their initial halfwords
  are zero, unequal, or equal. This connects the target to the group's
  record-operation helpers rather than relying on generic call proximity.

## Limits

This adds a candidate member to the existing regional group, not a proven
original translation-unit boundary. No shared static, GP-relative ownership,
declaration-order effect, or register-variable quirk was established. The
target directly accesses no globals. Its predecessor and other dependencies
are corroborating context, not newly asserted members.
