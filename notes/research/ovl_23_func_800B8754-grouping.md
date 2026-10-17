# ovl_23_func_800B8754: timed dispatch callback membership

## Provenance

Documentation-only handoff: the target is supplied as byte-exact and having
passed the full finalize gate. Supplied source/evidence identity:
`f583dac93fa6c8ed86fc5405c7d61331d996de4b06966d22ae16bbaa2b48d621`
(origin: agent). Supplied changed files: `include/globals_override.h`,
`include/overlays/ovl_23.h` and
`src/overlays/ovl_23/ovl_23_func_800B8754.c`. This pass changes only notes;
it does not rerun matching or modify build inputs.

## New membership evidence

- **Exact counter/selector handoff, not just a shared aggregate:** the
  immediate predecessor `800B8644` stores 5 to the s16 selector at
  `D_800BF87C` and clears the halfword at +0x416. The verified target
  increments that exact counter and, after 60 updates, stores 6 to the
  same selector and clears the counter. The `800B8084` dispatcher uses
  this selector to index `D_800BB9F8`. The target also shares the
  `800B9310` updater and the `80022738` / `80017B18(0)` / `800226D8(0)`
  call sequence with its predecessor; its `8002261C(4, 0x2E)` follows
  the predecessor's `8002261C(4, 0x2D)`.
- **Callback graph agrees with gapless text order:** `D_800BB9F8[4..6]`
  contains `800B8644`, `800B8754` and `800B87F8`, respectively. Their
  boundaries agree: `0x800B8644 + 0x110 = 0x800B8754` and
  `0x800B8754 + 0xA4 = 0x800B87F8`. Thus the predecessor's transition
  selects the target, and the target's transition selects the already
  recorded successor. This extends the existing dispatch-index cluster.

## Evidence paths

- `src/overlays/ovl_23/ovl_23_func_800B8754.c` and
  `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8754/ovl_23_func_800B8754.s`:
  counter load at 0x800B8770, increment/store at 0x800B87A4–0x800B87B0,
  threshold at 0x800B87BC and selector/counter stores at
  0x800B87DC–0x800B87E0.
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8644/ovl_23_func_800B8644.s`:
  0x110-byte predecessor, shared calls and selector/counter stores at
  0x800B8730–0x800B8734.
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B8084/ovl_23_func_800B8084.s`:
  shared selector and table dispatch, also documented in the
  [800B85FC membership note](ovl_23_func_800B85FC-grouping.md).
- `build/ovl_23/asm/data/3A40.data.s`: callback entries at
  0x800BBA08, 0x800BBA0C and 0x800BBA10.
- `configs/splat/ovl_23.yaml`: consecutive text offsets 0x824, 0x934
  and 0x9D8 corroborate the body boundaries, not original TU boundaries.
- `include/globals_override.h`: `Ovl23ScreenHeader` aliases `D_800BF87C`
  and exposes the s16 selector, voice index at +0x414 and counter at +0x416.

## Membership conclusion and limits

Add only `ovl_23_func_800B8754` to the existing `D_800BF87C` dispatch-index
candidate cluster, retaining medium confidence. Under that cluster's
same-TU hypothesis, the exact predecessor-to-target state/counter handoff,
shared call sequence and corroborated callback/text order support the
previously unlisted member.

Calls, shared globals and callback tables can cross translation units. This
is not proof of original boundaries, static ownership, declaration order or
a register-variable quirk. Do not add other callbacks or merge the separate
record-driver cluster solely from this evidence.
