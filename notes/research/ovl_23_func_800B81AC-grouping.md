# ovl_23_func_800B81AC: screen-record initializer run extension

Documentation-only handoff: the target is supplied as byte-exact and having
passed the full finalize gate. Supplied source/evidence identity:
`6bb4afa17627901c60703b864f535e9c1da799f10b5f9f9507b8d9ec40971442`
(origin: agent). Supplied changed files: `include/globals_override.h`,
`include/overlays/ovl_23.h`, and
`src/overlays/ovl_23/ovl_23_func_800B81AC.c`. This pass changes only notes.

## New membership evidence

- **Shared initialization cluster:** the verified target clears the halfword
  at `D_800BF87C+0` and initializes the halfwords at +0x39C through +0x3B2.
  Existing member `800B823C` initializes the same aggregate's header and
  invokes the record initializers `800B8290`, `800B8354` and `800B937C`.
  These are complementary parts of one initialization path, not merely
  unrelated consumers of an overlay-wide base.
- **Call edge and gapless link adjacency:** the target is 0x90 bytes,
  ending exactly at `800B823C`, and calls that immediate successor before
  calling `800B97E0`. Splat offsets 0x38C and 0x41C corroborate the boundary.
  This extends the existing `800B823C`/`800B8290`/`800B8354` initializer run
  backwards by one function.
- **Connection, not a wider merger:** `800B97E0` fills the six halfwords at
  `D_800BF892 = D_800BF87C+0x16`. The target's second call connects that
  separately recorded family to the initialization path, but does not by
  itself establish a common TU for the intervening functions.

## Evidence paths and limits

- `src/overlays/ovl_23/ovl_23_func_800B81AC.c`
- `src/overlays/ovl_23/ovl_23_func_800B823C.c`
- `src/overlays/ovl_23/ovl_23_func_800B97E0.c`
- `include/globals_override.h` (`Ovl23MovementBounds`,
  `Ovl23ScreenHeader` and their `D_800BF87C` aliases).
- `configs/splat/ovl_23.yaml` (consecutive text entries).
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B81AC/ovl_23_func_800B81AC.s`
  (original size, aggregate stores and both call targets).

Retain medium confidence for the extended candidate group. Shared aggregate
initialization and corroborated call/link adjacency support suspected same-TU
membership, not exact original boundaries or static linkage. The recovered
struct views do not establish original declarations; no compiler quirk,
declaration-order or register-variable premise is asserted.

## 800B8354: interleaved initialization-table evidence

The verified `800B8354` handoff passed the full finalize gate (identity
`68b7423a2aca3dcbe82108fa2555764b5a26d13652b46545555d0c010ec12a61`).
Its source adds a table-cluster corroboration not recorded above:

- `800B8354` consumes six two-halfword pairs at `D_800BBA64`, an observed
  0x18-byte footprint ending at `D_800BBA7C`.
- Existing member `800B8290` consumes six halfwords at `D_800BBA7C`, ending
  at `D_800BBA88`, and selects one of four halfwords at `D_800BBA88` through
  `func_80012A34(4)`, reaching the next table at `D_800BBA90`.
- `800B8354` consumes that `D_800BBA90` speed table. Its complete bound is
  not established here.

Thus the two initializers consume interleaved, abutting table footprints,
in addition to their already-recorded aggregate, caller and link-order
agreement. Evidence: their verified sources under `src/overlays/ovl_23/`
and original listings under `build/ovl_23/functions/`; the latter show the
six-iteration bounds, pair-pointer step four and selector argument four.

This corroborates the existing medium-confidence candidate group; it adds
no distant member or exact TU boundary. The newly shared 0x50-record view
with `800BA494` alone does not justify merging that consumer into this run,
and the table observations do not establish original static linkage.
