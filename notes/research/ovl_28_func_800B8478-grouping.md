# ovl_28_func_800B8478 — handler shared-state evidence

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with source/evidence identity
`a838a0ab26bb2dc2d90d42de895f7a75b037a35dd0ab684030409a4e04d53a6f`.
Its changed build inputs are `include/globals_override.h`,
`include/overlays/ovl_28.h` and
`src/overlays/ovl_28/ovl_28_func_800B8478.c`. This note reads the verified
body and neighbouring evidence; it does not rerun matching or alter build inputs.

## New grouping evidence

- `src/overlays/ovl_28/ovl_28_func_800B8478.c` consumes and updates the
  `D_800B93AE`/`D_800B93B0`/`D_800B93B2` command-index/timing state and
  reads and clears `D_800B93B8`. These globals are seeded by
  `src/overlays/ovl_28/ovl_28_func_800B8414.c` and
  `src/overlays/ovl_28/ovl_28_func_800B7E80.c`. The ledger already recorded
  their setup, but not this handler's use of the wider cluster.
- The handler sets `D_800B93BC` on command 0xFF.
  `src/overlays/ovl_28/ovl_28_func_800B8124.c` calls through `D_800B9614`
  and then tests that flag in phase 2. Together with `800B8414.c` installing
  this handler in `D_800B9614`, this supplies the completion producer for the
  dispatcher consumer recorded in
  [the 800B8124 grouping note](ovl_28_func_800B8124-grouping.md).
- The handler writes `D_800B93C0` on commands 0xF2/0xF3. The adjacent slot 8
  setup, `src/overlays/ovl_28/ovl_28_func_800B865C.c`, resets that same word;
  `800B7E80.c` also resets it. This adds a shared-state link between the slot
  7 successor and the already-listed slot 8 setup, beyond their table relation.

## Membership conclusion and limits

`configs/splat/ovl_28.yaml` places `800B8414`, `800B8478` and `800B865C` at
consecutive code offsets 0x5F4, 0x658 and 0x83C: the setup's 0x64-byte body
ends at this handler, whose 0x1E4-byte body ends at slot 8 setup. The new
shared-cluster and callback/completion links corroborate membership in the
existing `D_800B95F8` handler-pointer-table group.

No new member or TU boundary is established; confidence remains medium.
Shared globals and callbacks may cross translation units, so the conclusion
remains conditional on the existing adjacency-based grouping hypothesis.
The `D_800B93B4` setup/division tie and the slot 7 installation were already
recorded and are not counted as new evidence. The command stream
`D_800B936C` alone supplies no additional same-file tie.
