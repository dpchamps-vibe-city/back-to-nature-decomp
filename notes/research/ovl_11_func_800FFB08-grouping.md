# ovl_11_func_800FFB08: translation-unit grouping evidence

Verified handoff identity: `0f5376ccaf48704004ec264bb186426b9c94618c85bbde29518f12e551205813`.
Source SHA-256: `dab9fc4f72db7e459fbecddf44c6ad399a2b30f83f990b1d0455931bad6e537b`.
The supplied handoff reports byte identity and a passed full finalize gate;
this documentation pass did not modify build inputs or repeat solver work.

## New membership evidence

- **Complementary access to the existing selection/flag pair.**
  `src/overlays/ovl_11/ovl_11_func_800FFA28.c` initializes
  `D_801273D8 = 0` and `D_801273DA = 0xFF`;
  `src/overlays/ovl_11/ovl_11_func_800FFA40.c` calls that initializer and
  clears the flag to activate the menu. The verified source in
  `src/overlays/ovl_11/ovl_11_func_800FFB08.c` reads and updates the same
  `D_801273D8` halfword as a four-choice selection. Its extracted target in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800FFB08/ovl_11_func_800FFB08.s`
  confirms the selection loads/stores at `0x800FFBF8–0x800FFC34`.
  `build/ovl_11/asm/data/69960.data.s` places the two halfwords consecutively
  at `0x801273D8` and `0x801273DA`. This supersedes the ledger's earlier
  claim that only the initializer/reset pair references them.
- **Call graph agrees with gapless link order.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800FFA64/ovl_11_func_800FFA64.s`
  reads `D_801273DA`, calls 800FFB08 at `0x800FFA80` when the flag is zero,
  latches its result in `D_8012CEF8`, and advances the same flag on a
  nonzero result. `configs/splat/ovl_11.yaml` places the reset, wrapper and
  controller consecutively at offsets `0x47C20`, `0x47C44` and `0x47CE8`.
  Their extracted sizes are `0x24`, `0xA4` and `0x2C4`, respectively
  (`build/ovl_11/functions.csv`); the wrapper's final instruction is at
  `0x800FFB04`, immediately before 800FFB08's entry. The wrapper therefore
  bridges the pair's flag to the controller's selection through both a
  direct call and contiguous code.

These shared-state and call/link ties support adding 800FFA64 and 800FFB08
as same-TU candidates in the existing `D_801273D8/D_801273DA` section.
The grouping remains medium-confidence: shared overlay state and adjacency
support membership but do not establish original file boundaries or a
compiler-local ownership/declaration-order effect. In particular, the next
function, 800FFDCC, belongs to the separately recorded `D_801273DC` state
cluster; this evidence does not merge the two groups or strengthen the
section's earlier menu-handler extensions.
