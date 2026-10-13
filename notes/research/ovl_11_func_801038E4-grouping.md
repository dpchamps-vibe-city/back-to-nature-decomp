# ovl_11_func_801038E4: grouping corroboration

Documentation-only review of the verified source. The supplied handoff reports
byte-exact matching and a passing full finalize gate, with source/evidence
identity `3a323ec7879fb3439020d4c36223ee47d1d3076bd7a32dff5fb4f62b23ab99e2`
and changed file `src/overlays/ovl_11/ovl_11_func_801038E4.c`. Matching and
build checks were not repeated; this review changes only notes.

## New evidence relative to the ledger

The existing D_80127428 cluster already records this function's shared-state
read and gapless link adjacency to `80103964`; those are not new findings.
The verified source additionally calls that immediate successor when
`D_80127428 != 0`. Original assembly places the call at 0x80103940, and
`801038E4 + 0x80 = 80103964`. This previously unrecorded caller/callee tie
corroborates the existing candidate membership: the state-gated wrapper
invokes the adjacent mutator of the same state, rather than being a leaf
reader as the ledger previously described.

## Evidence paths and limits

- `src/overlays/ovl_11/ovl_11_func_801038E4.c`
- `src/overlays/ovl_11/ovl_11_func_80103964.c`
- `build/ovl_11/functions/ovl_11_func_801038E4.s` (state load at
  0x801038E8, local call at 0x80103940, size 0x80)
- `configs/splat/ovl_11.yaml` (consecutive offsets 0x4BAC4 and 0x4BB44)

Assuming the shared state and corroborated call/link order reflect source
organization, this strengthens the existing medium-confidence same-TU
candidate. It adds no member and proves no original TU boundary, private
static ownership or declaration/register quirk. Shared engine helpers and
screen-dimension globals alone are not grouping evidence.
