# ovl_23_func_800B85FC: dispatch-index callback membership

## Provenance

Documentation-only handoff: the target is supplied as byte-exact and having
passed the full finalize gate. Supplied source/evidence identity:
`d748d5f337a45f13b78bf1a55a94c0e28a9409b3acaeb5372417449809d568c0`
(origin: agent). Supplied changed files: `include/overlays/ovl_23.h` and
`src/overlays/ovl_23/ovl_23_func_800B85FC.c`. This pass changes only notes;
it does not rerun matching or modify build inputs.

## New membership evidence

- **Same dispatch halfword and local updater:** the verified target calls
  `ovl_23_func_800B9310`, issues `func_8002261C(4, 0x2C)`, and conditionally
  stores 4 to the s16 at `D_800BF87C`
  (`Ovl23_D_800BF87C_bounds.unk0`) when `func_800226A4()` returns 2.
  The already-recorded `800B853C` callback has the same updater/poll chain
  with argument 0x2A and transition to 2. The existing `800B8084` dispatcher
  reads this exact halfword to select `D_800BB9F8`; this is not merely
  another access within the large overlay aggregate.
- **Callback graph agrees with gapless link order:** the target is
  `D_800BB9F8[3]`, between `800B8584` at index 2 and `800B8644` at index 4.
  Their text boundaries agree: `0x800B8584 + 0x78 = 0x800B85FC`, and
  `0x800B85FC + 0x48 = 0x800B8644`. The immediate predecessor also calls
  `800B9310`, issues `func_8002261C(4, 0x2B)`, polls `func_800226A4`, and
  can store 3 to the same selector. The target's transition to 4 selects
  its immediate link-order successor. This extends the callback run
  already supported by the [800B853C membership note](ovl_23_func_800B853C-grouping.md).

## Evidence paths

- `src/overlays/ovl_23/ovl_23_func_800B85FC.c` and
  `build/ovl_23/asm/nonmatchings/ovl_23_func_800B85FC/ovl_23_func_800B85FC.s`:
  updater call at 0x800B8604, poll at 0x800B8618, and selector store at
  0x800B8630.
- `src/overlays/ovl_23/ovl_23_func_800B853C.c`: same updater/poll chain and
  dispatch-halfword view.
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8584/ovl_23_func_800B8584.s`:
  0x78-byte predecessor; shared updater/poll chain and selector writes.
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8084/ovl_23_func_800B8084.s`:
  selector load and table dispatch at 0x800B8094–0x800B80B4.
- `build/ovl_23/asm/data/3A40.data.s`: callback entries at
  0x800BBA00, 0x800BBA04 and 0x800BBA08.
- `configs/splat/ovl_23.yaml`: consecutive text offsets 0x764, 0x7DC and
  0x824 corroborate the body boundaries.
- `include/globals_override.h`: `Ovl23_D_800BF87C_bounds` aliases
  `D_800BF87C`, with `unk0` declared as s16.

## Membership conclusion and limits

Add only `ovl_23_func_800B85FC` to the existing `D_800BF87C` dispatch-index
candidate cluster, retaining medium confidence. Under that cluster's
same-TU hypothesis, the shared state-transition machinery and corroborated
callback/text order support this previously unlisted member.

Calls, globals and callback tables can cross translation units. This does
not prove original boundaries, static ownership, declaration order or a
register-variable quirk. Do not merge the separately recorded updater
cluster or add the other callbacks solely from this evidence.
