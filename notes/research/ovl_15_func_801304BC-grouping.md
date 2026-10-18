# ovl_15_func_801304BC — grouping evidence

Verified handoff identity:
`a802009bc0ab75016d023f574569f059c0380f4deb4622df628865ba4a542060`.
The handoff reports byte identity and a passed full finalize gate; this review
changes notes only and does not rerun matching.

## Additional support for existing membership

The `D_80053350/func_8001AC10` shared-prologue section already names this
function as a `D_80052FC4` user. That draw setup is not new evidence.
The verified body adds a shared-state writer/consumer tie:

- `src/overlays/ovl_15/ovl_15_func_801304BC.c` has the same two-card
  poll/normalize/cache sequence as the already-grouped
  `src/overlays/ovl_15/ovl_15_func_8012ED6C.c`: after the
  `D_8013759A >= 2` guard, both call `ovl_15_func_80135C20` for each card,
  normalize statuses 1 to 0 and 7 to 6, and update `D_80137594[0..1]`.
  Both then select `D_80137584` state 2 when both cached statuses are 4,
  otherwise state 3. This is a shared operation, not a claim of identical
  machine code.
- The already-grouped `src/overlays/ovl_15/ovl_15_func_801306D4.c` consumes
  `D_80137594[D_8013758C]` in its status-4 path and writes the same
  `D_80137584` menu-state byte. The target therefore maintains state used
  by another handler in the existing cluster, beyond sharing its draw setup.
- Original assembly corroborates the target's guard, polling loop, halfword
  cache stores and menu-state selection at 0x80130508–0x801305AC in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_801304BC/ovl_15_func_801304BC.s`.

These ties strengthen the existing candidate same-TU membership, conditional
on the shared-prologue/state cluster representing an original TU. They do not
prove static linkage or exact boundaries; retain medium confidence. The helper
call does not merge the separately listed MemCard SDK cluster into this group.
