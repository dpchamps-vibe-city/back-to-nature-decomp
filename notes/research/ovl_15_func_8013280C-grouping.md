# ovl_15_func_8013280C — grouping evidence

Verified handoff identity:
`f0ef53acaa2b912e1c89ed6c2617db02a8b18f115ec7d5d84030c15efb478506`.
The handoff reports byte identity and a passed full finalize gate for
`src/overlays/ovl_15/ovl_15_func_8013280C.c`,
`include/overlays/ovl_15.h` and `configs/splat/ovl_15.yaml`.
This is a notes-only review, not a new matching attempt.

## Supported membership

Add the target to the existing medium-confidence
`D_80053350/func_8001AC10` shared-prologue cluster; retain its confidence
and leave exact original translation-unit boundaries unresolved.

- The verified source shares with existing member
  `src/overlays/ovl_15/ovl_15_func_8012FF28.c` the
  `func_8001AC10(field_D8 + 0x18, field_D8 + 0x14,
  D_8005346A + D_80054BBC[0])` draw idiom, followed by
  `ovl_15_func_80136660(D_8013758C, D_8013758E, D_80137830)` and
  result-based writes to `D_80137584`. These are a shared text/global
  cluster and a closely corresponding handler body, not just a common
  drawing callee. The target differs in destination states and its
  success-path call to `ovl_15_func_801331C4`.
- Original assembly at
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8013280C/ovl_15_func_8013280C.s`
  corroborates the draw setup at 0x80132810–0x8013283C, the card/slot/buffer
  arguments and write-helper call at 0x80132840–0x80132858, and the
  `D_80137584` result stores at 0x80132888–0x801328B0.
- `configs/splat/ovl_15.yaml` places the target immediately before existing
  member `ovl_15_func_801328C4`: 0x4A24 + 0xB8 = 0x4ADC.
  The target selects state 0x1C for write results 2/5; the successor's
  source, `src/overlays/ovl_15/ovl_15_func_801328C4.c`, uses the same draw
  idiom and passes state 0x1C to `ovl_15_func_80137228` before updating
  `D_80137584`. Thus link adjacency agrees with shared-state handling.

The main-executable text table and drawing helper alone do not establish
same-TU membership; the inference depends on their combination with the
shared overlay-local card/slot/state/buffer accesses and corresponding
handler structure. Calling the write helper or slot-transfer helper does
not justify merging their separately recorded clusters, prove static
linkage, or establish an exact TU boundary.
