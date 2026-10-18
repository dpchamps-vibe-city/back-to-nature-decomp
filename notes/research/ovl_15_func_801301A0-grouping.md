# ovl_15_func_801301A0 — grouping evidence

Verified handoff identity:
`92feb76385376bab8f4aaf253294ada5c50bba58ea95ce3618fb381ecffc1648`.
The handoff reports byte identity and a passed full finalize gate; this review
changes notes only and does not rerun matching.

## New membership support

The verified `src/overlays/ovl_15/ovl_15_func_801301A0.c` supports adding the
dispatcher to the existing `D_80053350/func_8001AC10` shared-prologue/state
cluster, although the dispatcher itself has no text-draw prologue:

- Its switch on `D_80137584` dispatches states 0–19 to the handler run
  `ovl_15_func_80130468` through `ovl_15_func_80130FF0`, in link order.
  `configs/splat/ovl_15.yaml` places the target at offset 0x23B8, with size
  0x2C8 ending exactly at its state-0 callee `ovl_15_func_80130468`
  (0x2680). That callee's 0x54 bytes end exactly at the state-1 callee
  `ovl_15_func_801304BC` (0x26D4). Both callees are already grouped;
  adjacency is therefore corroborated by direct calls, not merely proximity.
- Beyond dispatch, the target reads and updates `D_80137594`, selects a card
  using `D_8013758C`, and writes menu transitions to `D_80137584`.
  `src/overlays/ovl_15/ovl_15_func_801304BC.c` refreshes both
  entries of that cache and writes the same menu-state byte;
  `src/overlays/ovl_15/ovl_15_func_801306D4.c` consumes
  `D_80137594[D_8013758C]` to select further menu states. The dispatcher
  maintains the same state its handlers use, not just a common helper call.
- The target uses `D_8013758A` as the alternating card index when
  `D_8013758C == 2`. `src/overlays/ovl_15/ovl_15_func_8012E15C.c`
  initializes that index to 0, the selection to 2 and both cached statuses
  to 4, corroborating the shared state block.

Original assembly corroborates dispatch at 0x801301A0–0x80130320 and the
card selection, cache updates and menu transitions at 0x80130324–0x80130450
in `build/ovl_15/asm/nonmatchings/ovl_15_func_801301A0/ovl_15_func_801301A0.s`.

These ties support candidate same-TU membership conditional on the existing
shared-prologue/state cluster representing an original TU. Retain medium
confidence: they do not establish static linkage or exact TU boundaries.
Calling `ovl_15_func_80135E9C` does not merge the separately listed MemCard
SDK cluster into this group.
