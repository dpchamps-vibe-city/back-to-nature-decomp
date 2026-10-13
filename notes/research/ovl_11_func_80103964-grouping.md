# ovl_11_func_80103964: grouping corroboration

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `a4fb5fba04bd3ad5a1d12f8488115cca6a59dd17b9ca8fb78d3e87282ab1bfd6`
and changed file `src/overlays/ovl_11/ovl_11_func_80103964.c`. This review
changes only notes; matching and build checks were not repeated.

## New evidence relative to the ledger

The existing D_80127428 cluster already records this function as its state
mutator, its gapless adjacency to `80103B24`, and its initial calls to
`80103EB8`, `80103F8C` and `801040A8`. Those are not new findings.

The verified dispatcher also owns the restore half of the cluster's saved
row/accumulator pair. On `80103C00` returning -1, it copies `D_8012CF1C` back
into `D_8012CF10[D_8012742C]` and `D_8012CF24` back into `D_8012CF20`.
Neighbouring `80103D44` saves those exact values in the opposite direction
before returning 2; the dispatcher uses that result to enter state 2 and
invoke `80103C00`, which edits the same indexed row. `801037EC` clears the
index and saved pair. This adds a previously unrecorded `D_8012742C` reader
and binds the core state mutator directly to the shared `0x8012CFxx` storage.

The local call graph corroborates the data tie: original calls to `80103D44`,
`80103C00` and `80103B24` occur at 0x801039A0, 0x801039D8 and 0x80103AA8.
Link order is gapless through the same functions:
`80103964 + 0x1C0 = 80103B24`, `80103B24 + 0xDC = 80103C00`,
`80103C00 + 0x144 = 80103D44`. Thus the restore path strengthens the
existing candidate membership rather than introducing a new family.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_80103964.c`
- `src/overlays/ovl_11/ovl_11_func_80103D44.c`
- `src/overlays/ovl_11/ovl_11_func_80103C00.c`
- `src/overlays/ovl_11/ovl_11_func_801037EC.c`
- `build/ovl_11/functions/ovl_11_func_80103964.s` (restore accesses at
  0x801039F0–0x80103A28 and the local calls above)
- `configs/splat/ovl_11.yaml` (consecutive offsets 0x4BB44, 0x4BD04,
  0x4BDE0 and 0x4BF24)
- `include/globals_override.h` (shared state/buffer and saved-pair declarations)

## Limits

Assuming this specific shared storage and corroborated local call/link order
reflect source organization, they support the existing medium-confidence
same-TU candidate. They do not establish an original TU boundary, private
static ownership, GP-relative ownership or a shared declaration/register
quirk. No new member or confidence increase is warranted. The distant
`80103964` callee `800BCF28` and shared engine helpers are not same-TU evidence.
