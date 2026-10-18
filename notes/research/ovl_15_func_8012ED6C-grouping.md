# ovl_15_func_8012ED6C — grouping evidence

Verified handoff identity:
`1d6fcb0a634af770b92b0c020ee01df2a995e6d6597d9c84624ea3c3288175db`.
The handoff reports byte identity and a passed full finalize gate; this review
changes notes only and does not rerun matching.

## Additional support for existing membership

The `D_80053350/func_8001AC10` shared-prologue section already names this
function as a `D_80052FC4` user and the immediate successor of
`ovl_15_func_8012ED18`. Neither fact is new grouping evidence.

The verified body adds a writer/consumer tie within that candidate group:

- `src/overlays/ovl_15/ovl_15_func_8012ED6C.c` polls
  `ovl_15_func_80135C20` for both cards when `D_8013759A >= 2`, normalizes
  statuses 1 to 0 and 7 to 6, and updates `D_80137594[0..1]`. It then sets
  `D_80137584` to 2 if both cached statuses are 4, otherwise 3.
- The already-grouped `ovl_15_func_8012EF84`, `ovl_15_func_801306D4` and
  `ovl_15_func_801316B0` sources in the same directory use the same guard,
  status helper and menu-state byte. Their status-4 paths read
  `D_80137594[D_8013758C]` to choose the next state. The target therefore
  maintains the shared cached state those handlers consult, beyond merely
  sharing their text-draw setup.
- Original assembly corroborates the target's guard, polling loop, halfword
  stores and menu-state selection at 0x8012EDB8–0x8012EE5C in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012ED6C/ovl_15_func_8012ED6C.s`.

This strengthens membership in the existing shared-prologue/state cluster;
shared objects do not prove static linkage or exact TU boundaries. Retain
medium confidence. The helper call does not establish same-TU membership
with the separately listed MemCard SDK cluster.
