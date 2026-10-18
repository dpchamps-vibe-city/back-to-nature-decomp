# ovl_15_func_8012E520 — grouping evidence

Verified handoff identity:
`8b12c7dd935efee048552b5ff25606d0ae014a4dd18667b56a005b31a0d86597`.
The agent-origin handoff reports byte identity and a passed full finalize gate;
its changed files are `include/globals_override.h`, `include/overlays/ovl_15.h`
and `src/overlays/ovl_15/ovl_15_func_8012E520.c`. This review changes notes only
and does not rerun matching.

## New membership support

The verified source supports adding this lifecycle controller to the existing
medium-confidence `D_80137828` buffer-pointer/private-state cluster:

- It calls `ovl_15_func_8012E15C` during initialization, then maintains the
  same state block that initializer clears. It compares `D_80137584` against
  the previous-state byte `D_80137585`, updates the latter on a change and
  resets `D_8013759A`, otherwise incrementing that timer up to 0xFF.
  `src/overlays/ovl_15/ovl_15_func_8012E15C.c` initializes all three objects.
  This is a shared-state ownership tie, not just a common initializer call.
- Before calling `ovl_15_func_801301A0`, it sets `D_80137586 = 2`; afterward
  it consumes that byte's -1/1 values to initiate exit transitions. Existing
  cluster members `src/overlays/ovl_15/ovl_15_func_80130FC8.c` and
  `src/overlays/ovl_15/ovl_15_func_80130FF0.c` call the initializer and write
  -1 and 1 respectively. The target supplies the lifecycle consumer of this
  already-grouped writer/reset protocol. Original assembly corroborates the
  initializer call at 0x8012E590, state/timer maintenance at
  0x8012E640–0x8012E680 and dispatch/exit-byte consumption at
  0x8012E690–0x8012E6D4 in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012E520/ovl_15_func_8012E520.s`.
- `configs/splat/ovl_15.yaml` places the target between
  `ovl_15_func_8012E2CC` and `ovl_15_func_8012E7C8`, at offsets
  0x4E4 / 0x738 / 0x9E0. Both neighbours' assembly under
  `build/ovl_15/asm/nonmatchings/` shows the same previous-state/timer and
  exit-byte protocol (0x8012E3BC–0x8012E450 and 0x8012E8B8–0x8012E94C).
  This corroborates a local controller family, but does not independently
  establish either neighbour's original TU membership.

Candidate same-TU membership remains conditional on the existing
initializer/private-state cluster representing an original TU. Shared absolute
globals do not prove static linkage or exact boundaries; retain medium
confidence. The dispatcher and sprite-loader calls do not by themselves merge
the separately listed shared-prologue and streamed-load clusters.
