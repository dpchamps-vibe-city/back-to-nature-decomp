# ovl_11_func_800FEFEC: translation-unit grouping evidence

Verified handoff identity: `c3eac02c231de6e8cda343daabe0b6f9e06ca835b4a6da50993ea4ed3808c8f0`.
Source SHA-256: `cb78f6ddab61b57d5f8044cd6ff528f9be3fc2bad526accd3e5a5ed516db7d2c`.
The supplied handoff reports byte identity and a passed full finalize gate;
this documentation pass did not modify build inputs or repeat solver work.

## New membership evidence

- **Call graph agrees with gapless link order.**
  `src/overlays/ovl_11/ovl_11_func_800FEFEC.c` calls
  `ovl_11_func_800FF27C(D_801273B6, D_801273B8_selection[0])`.
  `configs/splat/ovl_11.yaml` places the caller at offset `0x471CC` and
  the callee immediately next at `0x4745C`: a `0x290`-byte span.
  The extracted target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800FEFEC/ovl_11_func_800FEFEC.s`
  confirms the call at `0x800FF250` and the last instruction at `0x800FF278`,
  immediately before the callee's `0x800FF27C` entry.
- **Complementary access to the same state cluster.** The caller handles
  menu input in `D_801273B0 == 0x10`, sets the state to `0x11` on confirmation,
  and passes the column/row selection to the callee.
  `src/overlays/ovl_11/ovl_11_func_800FF27C.c` consumes those arguments in
  state `0x11` and returns the same state word to `0x10` after event completion.
  `src/overlays/ovl_11/ovl_11_func_800FEBF8.c` resets `D_801273B0`,
  `D_801273B6` and `D_801273B8` together, corroborating the existing state-block
  relationship already cited by the ledger.

These independent call/link and shared-state ties support adding 800FEFEC
beside the already-listed 800FF27C in the existing `D_801273D8/D_801273DA`
init/reset-pair section (medium-confidence suspected translation unit).
800FEFEC does not itself access that flag pair; its membership is through
800FF27C, not direct flag-pair access. This does not establish original
translation-unit boundaries or strengthen the section's other members.
