# ovl_15_func_8012EA18 — grouping evidence

Verified handoff identity:
`3308d77a569dca5d4f685a81d4970156eb3e5cd95b7a80a9b272cc892336c449`.
The agent-origin handoff reports byte identity and a passed full finalize gate;
its changed files are `configs/splat/ovl_15.yaml`, `include/overlays/ovl_15.h`
and `src/overlays/ovl_15/ovl_15_func_8012EA18.c`. This review changes notes
only and does not rerun matching.

## New membership support

The verified source supports adding this dispatcher to the existing
`D_80053350/func_8001AC10` shared-prologue/state cluster, despite the
dispatcher itself having no text-draw prologue:

- Its switch on `D_80137584` calls the handler run from
  `ovl_15_func_8012ED18` through `ovl_15_func_8013010C` in link order
  (states 0–24, with no case 12). `configs/splat/ovl_15.yaml` places the
  target at offset 0xC30; its 0x300 bytes end exactly at its state-0 callee
  `ovl_15_func_8012ED18` (0xF30). That handler's 0x54 bytes end exactly at
  the state-1 callee `ovl_15_func_8012ED6C` (0xF84). Both callees already
  belong to the cluster, so direct calls corroborate the adjacency.
- Beyond dispatch, it reads and updates the two cached card statuses in
  `D_80137594`, selects a card through `D_8013758C`/`D_8013758A`, and writes
  transitions to `D_80137584`. The already-grouped
  `src/overlays/ovl_15/ovl_15_func_8012ED6C.c` refreshes both cache entries
  and sets that menu-state byte;
  `src/overlays/ovl_15/ovl_15_func_8012F4F4.c` reads the selected cached
  status to choose further states. This is a shared writer/consumer
  protocol, not just a common helper call.
- The already-grouped `src/overlays/ovl_15/ovl_15_func_801301A0.c` provides
  a parallel dispatcher: the same card-selection objects, alternating-card
  index and guarded cache updates, with different handler targets and a
  different status-4 transition. The common initializer
  `src/overlays/ovl_15/ovl_15_func_8012E15C.c` sets the alternating index to
  0, selection to 2 and both cached statuses to 4.

Original assembly corroborates dispatch at 0x8012EA18–0x8012EBD8 and the
card selection, cache updates and menu transitions at 0x8012EBDC–0x8012ED00
in `build/ovl_15/asm/nonmatchings/ovl_15_func_8012EA18/ovl_15_func_8012EA18.s`.
The adjacent handler's size and initializer/state-byte writes are visible in
`build/ovl_15/asm/nonmatchings/ovl_15_func_8012ED18/ovl_15_func_8012ED18.s`.

Candidate same-TU membership remains conditional on the existing
shared-prologue/state cluster representing an original TU. Retain medium
confidence: shared absolute globals and call/link adjacency do not prove
static linkage or exact boundaries. Neither the call to
`ovl_15_func_80135E9C` nor the new jump-table rodata assignment establishes
same-TU membership with the separately listed MemCard SDK cluster or merges
the initializer's buffer-pointer cluster into this one.
