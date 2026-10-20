# ovl_15_func_801326E8 — grouping evidence

Verified handoff identity:
`4416672a07851688f70dfc37f65233db26146a01aa1865975aa0010392ffbe7e`.
The handoff reports byte identity and a passed full finalize gate for
`src/overlays/ovl_15/ovl_15_func_801326E8.c` and
`include/overlays/ovl_15.h`. This is a notes-only review, not a new matching
attempt.

## Supported membership

Add the target to the existing medium-confidence
`D_80053350/func_8001AC10` shared-prologue cluster; retain its confidence
and leave exact original translation-unit boundaries unresolved.

- The verified source shares with existing member
  `src/overlays/ovl_15/ovl_15_func_8012F3FC.c` the same text base
  `D_80051FEA` and draw setup:
  `func_8001AC10(field_D8 + 0x18, field_D8 + 0x14,
  D_80051FEA + D_80054BBC[0])`. Both then render slots 0–2 through
  `ovl_15_func_80136C70`, draw the cursor at
  `(0x39, D_80137587 * 0x31 + 0x18)`, and switch on
  `ovl_15_func_801370B4()` to update `D_80137584`. The renderer mode differs
  (target 0, sibling 1), as do the branch actions. This is a shared
  text/global cluster and corresponding handler sequence, not merely a
  common drawing callee.
- Original assembly at
  `build/ovl_15/asm/nonmatchings/ovl_15_func_801326E8/ovl_15_func_801326E8.s`
  corroborates the text setup at 0x80132700–0x80132728, the slot loop,
  `D_80137587` read, cursor draw and selection-helper call at
  0x8013272C–0x8013277C, and the `D_80137588`/`D_80137584` stores at
  0x801327B8–0x801327F4.
- Link adjacency agrees with the call graph: `configs/splat/ovl_15.yaml`
  places the target between existing members `ovl_15_func_80132638` and
  `ovl_15_func_8013280C` (0x4850 + 0xB0 = 0x4900;
  0x4900 + 0x124 = 0x4A24). Their corresponding `.c` files share the draw
  idiom and state byte `D_80137584`; the predecessor also shares the
  selection helper, reset callee `ovl_15_func_80134724` and
  `D_80137588`. The common state dispatcher calls all three in this order
  at 0x80131288, 0x80131298 and 0x801312A8 in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_80131094/ovl_15_func_80131094.s`.

The main-executable text table alone does not establish same-TU membership;
this inference depends on its combination with the shared overlay-local
state, corresponding handler sequence and corroborated call/link adjacency.
These observations neither prove static linkage nor justify merging the
separately recorded renderer, slot-transfer or scratch-buffer clusters
merely because the target calls their helpers.
