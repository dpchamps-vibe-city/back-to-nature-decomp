# ovl_11_func_800FF324: translation-unit grouping evidence

Verified handoff identity: `8c58698ae312434417d76daa1eb06d1ae68217a606463b5bb2ddb2347b1f6f1b`.
Source SHA-256: `bd88fbb7096886605746f4d45331e155c2f72830c8dd0fae1f4e7174fa0fcf28`.
The supplied handoff reports byte identity and a passed full finalize gate;
this documentation pass did not modify build inputs or repeat solver work.

## New membership evidence

- **Call graph agrees with gapless link order.**
  `src/overlays/ovl_11/ovl_11_func_800FF324.c` calls already-listed
  `ovl_11_func_800FF8C8(D_801273BA, D_801273BC)`.
  `configs/splat/ovl_11.yaml` places the caller at offset `0x47504` and
  the callee immediately next at `0x47AA8`, a `0x5A4`-byte span.
  The extracted target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800FF324/ovl_11_func_800FF324.s`
  confirms the call at `0x800FF894` and the final instruction at `0x800FF8C4`,
  immediately before the callee's `0x800FF8C8` entry.
- **Complementary access to the same menu-state cluster.** The caller handles
  options input in `D_801273B0 == 0x20`, updates selectors `D_801273BA/BC`,
  and sets state `0x21` for the helper. The helper in
  `src/overlays/ovl_11/ovl_11_func_800FF8C8.c` consumes those selectors and
  returns the state to `0x20` after event completion.
  `src/overlays/ovl_11/ovl_11_func_800FEBF8.c` resets the state word and both
  selectors together. The existing dispatcher connection also corroborates
  this membership: `src/overlays/ovl_11/ovl_11_func_800FED0C.c` dispatches
  through `D_801273C0[(state >> 4) - 1]`, and
  `build/ovl_11/asm/data/69960.data.s` identifies its second entry at
  `0x801273C4` as 800FF324, beside first-entry 800FEFEC.

These call/link and shared-state ties support adding 800FF324 beside the
already-listed menu handlers in the existing `D_801273D8/D_801273DA`
init/reset-pair section (medium-confidence suspected translation unit).
800FF324 does not itself access that flag pair; its membership is through
800FF8C8 and the menu-state cluster. This does not establish original file
boundaries or strengthen the section's other members.
