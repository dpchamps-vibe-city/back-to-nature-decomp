# ovl_15_func_8012E2CC — grouping evidence

Verified handoff identity:
`d03584c69c82042d90ed828f63fe93f975c680fa0ee5036c24a9222b46b0c1e1`.
The agent-origin handoff reports byte identity and a passed full finalize gate;
its changed files are `include/overlays/ovl_15.h` and
`src/overlays/ovl_15/ovl_15_func_8012E2CC.c`. This review changes notes only
and does not rerun matching.

## Membership support

The verified source supports adding this lifecycle controller to the existing
medium-confidence `D_80137828` buffer-pointer/private-state cluster:

- It calls `ovl_15_func_8012E15C` during initialization and maintains that
  initializer's `D_80137584`/`D_80137585` state pair and `D_8013759A` timer.
  This is the same shared-state ownership tie as the already-listed
  `src/overlays/ovl_15/ovl_15_func_8012E520.c`, not just a common helper call.
- Its specific dispatcher path connects it to existing cluster members:
  it sets `D_80137586 = 2`, calls `ovl_15_func_8012EA18`, and consumes -1/1
  from that byte to begin exit fades. In
  `src/overlays/ovl_15/ovl_15_func_8012EA18.c`, cases 23/24 call
  `ovl_15_func_801300E4`/`ovl_15_func_8013010C`; those already-grouped
  handlers call the initializer and write -1/1 respectively. Their sources
  are `src/overlays/ovl_15/ovl_15_func_801300E4.c` and
  `src/overlays/ovl_15/ovl_15_func_8013010C.c`.
- `configs/splat/ovl_15.yaml` places the initializer, target and E520
  controller consecutively at offsets 0x374 / 0x4E4 / 0x738. The initializer's
  0x170 bytes end exactly at the target, whose 0x254 bytes end at E520.
  The direct initializer call corroborates this gapless adjacency; the
  adjacent E520 controller also calls the initializer and maintains the
  same state/timer block.

Original assembly corroborates the initializer call at 0x8012E33C, state/timer
maintenance at 0x8012E3EC–0x8012E42C, and the exit-byte/dispatcher protocol at
0x8012E3BC–0x8012E480 in
`build/ovl_15/asm/nonmatchings/ovl_15_func_8012E2CC/ovl_15_func_8012E2CC.s`.
The initializer size is recorded in
`build/ovl_15/asm/nonmatchings/ovl_15_func_8012E15C/ovl_15_func_8012E15C.s`.
The earlier [E520 note](ovl_15_func_8012E520-grouping.md) already observed
this neighbour's common protocol; the target-specific dispatcher/writer path
and call-corroborated initializer adjacency support its membership here.

Candidate same-TU membership remains conditional on the existing
initializer/private-state cluster representing an original TU. Shared absolute
globals and call/link adjacency do not prove static linkage or exact boundaries;
retain medium confidence. The dispatcher and sprite-loader calls alone do not
merge the separately listed shared-prologue and streamed-load clusters.
