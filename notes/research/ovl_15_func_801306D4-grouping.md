# ovl_15_func_801306D4 — grouping evidence

Verified handoff identity:
`8f114839b70f768ea05b3fcb0ee17284cdcf5b40bc80dab83b2edf48a3a4bf19`.
The handoff reports byte identity and a passed full finalize gate; this review
changes notes only and does not rerun matching.

## Additional support for existing membership

The `D_80053350/func_8001AC10` shared-prologue section already names this
function among the `D_80052FC4` users. Its verified body adds a shared-state
and status-helper tie to the already-named `ovl_15_func_8012EF84` and
`ovl_15_func_801316B0`, beyond the recorded draw setup:

- The three sources in `src/overlays/ovl_15/` guard on `D_8013759A >= 2`,
  pass the selected card `D_8013758C` to `ovl_15_func_80135C20`, and map its
  status to the same menu-state byte `D_80137584`.
- All three additionally test `D_80137594[D_8013758C]` for status 4, choosing
  state 5 when nonzero and a handler-specific state when zero. All map
  status 8 to state 6 and unhandled statuses to state 7. The target is a
  related handler, not a duplicate: it also maps statuses 6/7 to state 8.
- Original assembly corroborates the shared guard, helper and objects in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_801306D4/ovl_15_func_801306D4.s`
  at 0x8013070C–0x801307C4,
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012EF84/ovl_15_func_8012EF84.s`
  at 0x8012EFBC–0x8012F064, and
  `build/ovl_15/asm/nonmatchings/ovl_15_func_801316B0/ovl_15_func_801316B0.s`
  at 0x801316E8–0x80131764.

The parallel status-handling sequence over the same private-state cluster
strengthens the existing candidate same-TU grouping. It does not establish
static linkage or exact TU boundaries; retain medium confidence. Calling
`ovl_15_func_80135C20` does not establish that the separately listed MemCard
SDK cluster belongs to this TU.
