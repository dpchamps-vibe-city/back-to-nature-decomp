# ovl_23_func_800B853C: dispatch-index callback membership

## Provenance

Documentation-only handoff: the target is supplied as byte-exact and having
passed the full finalize gate. Supplied source/evidence identity:
`1923f371c80352516760e5ca28d5bb956cad77189deafac3a113911647268909`
(origin: agent). Supplied changed files: `include/overlays/ovl_23.h` and
`src/overlays/ovl_23/ovl_23_func_800B853C.c`. This pass changes only notes;
it does not rerun matching or modify build inputs.

## New membership evidence

- **Same dispatch halfword and local callee:** the verified target calls
  `ovl_23_func_800B9310`, then conditionally stores 2 to the s16 at
  `D_800BF87C` (`Ovl23_D_800BF87C_bounds.unk0`). Its already-recorded
  predecessor `ovl_23_func_800B850C` calls the same updater and stores 1
  to that exact halfword. The existing dispatcher `800B8084` reads the
  halfword to index `D_800BB9F8`.
- **Callback graph agrees with gapless link order:** `D_800BB9F8[0]` is
  `800B850C`, and `[1]` is the target. Their text bodies are consecutive:
  `0x800B850C + 0x30 = 0x800B853C`. The target's 0x48-byte body ends at
  `800B8584`, the next table entry and next function in link order.
  The target therefore supplies the next state transition within the
  existing callback run, rather than merely accessing another field of
  the large overlay aggregate.

## Evidence paths

- `src/overlays/ovl_23/ovl_23_func_800B853C.c` and
  `build/ovl_23/asm/nonmatchings/ovl_23_func_800B853C/ovl_23_func_800B853C.s`:
  updater call at 0x800B8544; conditional halfword store at 0x800B8570.
- `src/overlays/ovl_23/ovl_23_func_800B850C.c` and
  `build/ovl_23/asm/nonmatchings/ovl_23_func_800B850C/ovl_23_func_800B850C.s`:
  same updater call at 0x800B851C and dispatch-halfword store at 0x800B8530.
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8084/ovl_23_func_800B8084.s`:
  `D_800BF87C` load and `D_800BB9F8` dispatch at 0x800B8094–0x800B80B4.
- `build/ovl_23/asm/data/3A40.data.s`: callback pointers at
  0x800BB9F8, 0x800BB9FC and 0x800BBA00.
- `configs/splat/ovl_23.yaml`: consecutive text offsets 0x6EC, 0x71C
  and 0x764 corroborate the original body boundaries.

## Membership conclusion and limits

Add only `ovl_23_func_800B853C` to the existing `D_800BF87C` dispatch-index
candidate cluster, retaining medium confidence. Under that cluster's
same-TU hypothesis, the shared state transition, local updater and ordered
callback/text adjacency support this previously unlisted member.

Calls, globals and callback tables can cross translation units. This does
not prove original boundaries, static ownership, declaration order or a
register-variable quirk. Do not merge the separately recorded updater
cluster or add other callbacks solely from this evidence.
