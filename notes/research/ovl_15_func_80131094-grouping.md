# ovl_15_func_80131094 — grouping evidence

Verified handoff identity:
`3993da6536bfb8ae53bcc6642011ea4f8e092fb9e8069312a8566f54780fd58e`.
The handoff reports byte identity and a passed full finalize gate for
`src/overlays/ovl_15/ovl_15_func_80131094.c`,
`include/overlays/ovl_15.h` and `configs/splat/ovl_15.yaml`.
This review changes notes only and does not rerun matching.

## Supported membership

Add the dispatcher to the existing medium-confidence
`D_80053350/func_8001AC10` shared-prologue/state cluster, alongside the
already listed dispatcher `ovl_15_func_801301A0`. The target itself has no
text-draw prologue; its evidence is shared state and corroborated call/link
adjacency:

- The verified target's switch on `D_80137584` directly calls the handler
  sequence from `ovl_15_func_8013143C` through `ovl_15_func_801329F8`.
  `configs/splat/ovl_15.yaml` places the target at offset 0x32AC; its
  0x3A8-byte span ends exactly at its state-0 callee
  `ovl_15_func_8013143C` (0x3654). That handler's 0x5C-byte span ends at
  the state-1 callee `ovl_15_func_80131498` (0x36B0). Both handlers are
  existing cluster members, so the adjacency agrees with direct calls.
- The target shares the card-selection, status-cache and alternating-index
  tail of `src/overlays/ovl_15/ovl_15_func_801301A0.c`: select through
  `D_8013758C` or `D_8013758A`, poll `ovl_15_func_80135E9C`, compare and
  update `D_80137594`, toggle `D_8013758A` in two-card mode, and write
  transitions to `D_80137584`. The status-4 transition is 0x1E here rather
  than the sibling's 0x10. This is a corresponding state/cache-maintenance
  sequence, not just a shared polling callee.
- The directly following handlers maintain that same state block:
  `src/overlays/ovl_15/ovl_15_func_80131498.c` refreshes both cached
  statuses and selects the next menu state;
  `src/overlays/ovl_15/ovl_15_func_801316B0.c` consumes
  `D_80137594[D_8013758C]` and also routes to state 0x1E. Both use the
  established cluster's text-draw idiom with `D_80052FC4`.

Original assembly corroborates dispatch at 0x80131094–0x801312F4 and
card selection, polling, cache updates and state transitions at
0x801312F8–0x80131424 in
`build/ovl_15/asm/nonmatchings/ovl_15_func_80131094/ovl_15_func_80131094.s`.

These observations support candidate same-TU membership conditional on the
existing shared-prologue/state cluster representing an original TU. Retain
medium confidence: shared state and adjacency do not prove static linkage or
exact TU boundaries. The polling call alone does not justify merging the
separately recorded MemCard SDK cluster into this group.
