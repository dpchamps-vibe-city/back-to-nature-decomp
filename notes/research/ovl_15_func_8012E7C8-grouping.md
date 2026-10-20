# ovl_15_func_8012E7C8 — grouping evidence

Verified handoff identity:
`de094c7634bc361614e46aaa547f1f0de9b78445678ebb5523ca4639f128b823`.
The agent-origin handoff reports byte identity and a passed full finalize gate;
its changed files are `include/overlays/ovl_15.h` and
`src/overlays/ovl_15/ovl_15_func_8012E7C8.c`. This review changes notes only
and does not rerun matching.

## Membership support

The verified source supports candidate membership in the existing
medium-confidence `D_80137828` buffer-pointer/private-state cluster:

- It calls `ovl_15_func_8012E15C` during initialization, then maintains the
  initializer's `D_80137584`/`D_80137585` state pair and `D_8013759A` timer.
  The initializer clears all three in
  `src/overlays/ovl_15/ovl_15_func_8012E15C.c`. The same state-change reset
  and saturating timer update appear in the already-grouped neighbouring
  controllers `src/overlays/ovl_15/ovl_15_func_8012E2CC.c` and
  `src/overlays/ovl_15/ovl_15_func_8012E520.c`.
- Its specific dispatcher path supplies a lifecycle consumer for existing
  cluster members: it sets `D_80137586 = 2`, calls `ovl_15_func_80131094`,
  then consumes -1/1 from that byte to begin exit fades. In
  `src/overlays/ovl_15/ovl_15_func_80131094.c`, cases 32/33 call
  `ovl_15_func_801329D0`/`ovl_15_func_801329F8`; those already-grouped
  handlers call the initializer and write -1/1 respectively. Their sources
  are `src/overlays/ovl_15/ovl_15_func_801329D0.c` and
  `src/overlays/ovl_15/ovl_15_func_801329F8.c`. This writer/consumer path
  extends the neighbouring-controller observation in the
  [E520 note](ovl_15_func_8012E520-grouping.md), rather than relying only on
  a common helper call.
- `configs/splat/ovl_15.yaml` places E520 and this target consecutively at
  offsets 0x738 and 0x9E0. E520's 0x2A8-byte span ends at the target;
  both call the initializer and maintain the same state/timer/exit block.
  This link adjacency corroborates the shared-state tie.

Original assembly corroborates the initializer call at 0x8012E838,
state/timer maintenance at 0x8012E8E8–0x8012E928 and the dispatcher/exit-byte
protocol at 0x8012E8B8–0x8012E994 in
`build/ovl_15/asm/nonmatchings/ovl_15_func_8012E7C8/ovl_15_func_8012E7C8.s`.

Same-TU membership remains conditional on the existing initializer/private-state
cluster representing an original TU. Shared absolute globals and adjacency do
not prove static linkage or exact boundaries; retain medium confidence. Neither
the dispatcher call nor the sprite-loader call alone merges the separately
listed shared-prologue and streamed-load clusters.
