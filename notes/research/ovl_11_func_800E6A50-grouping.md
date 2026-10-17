# ovl_11_func_800E6A50 grouping evidence

The supplied byte-exact, full-finalize-gate-passed function adds a candidate
member to the existing `D_80129560` accessor family, with a local same-TU tie
to its immediate predecessor `ovl_11_func_800E69F8`. It does not establish an
original TU boundary.

Handoff source/evidence identity:
`75b2de8af36b4667d84a1106e018856a89c5702719d00891d27b5f44fdf6cbea`.
The supplied changed files are `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800E6A50.c`; this documentation does not
modify either file.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800E6A50.c` forwards
  `&D_80129560[arg2]`, the result of
  `ovl_11_func_800EFC68(arg0, arg1, arg2, arg3)`, and `arg3` to
  `ovl_11_func_800EFE34`, then returns 1.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E6A50/ovl_11_func_800E6A50.s`
  confirms the absolute table base, four-byte slot scaling, and calls at
  0x800E6A74 and 0x800E6A90. Its 0x60-byte extent ends at 0x800E6AB0.
- `src/overlays/ovl_11/ovl_11_func_800E69F8.c`, already recorded in the
  family, likewise passes a pointer into `D_80129560`, a selected value,
  and `arg3` to `ovl_11_func_800EFE34`, then returns 1. Its corresponding
  assembly under `build/ovl_11/asm/nonmatchings/ovl_11_func_800E69F8/`
  confirms that table-pointer use and the call at 0x800E6A38.
- `build/ovl_11/functions.csv` gives the predecessor a 0x58-byte extent:
  0x800E69F8 + 0x58 = 0x800E6A50. `configs/splat/ovl_11.yaml` independently
  places the pair consecutively at offsets 0x2EBD8 and 0x2EC30. Shared
  table-pointer use and the shared callee corroborate this gapless adjacency;
  adjacency alone is not the tie.

Limits: the target's slot index is full-width `arg2`, without a halfword cast
or guard. No shared register-variable or declaration-order quirk is inferred.
The two functions do not call each other, and an overlay-private table need
not be TU-private. This supports candidate family membership and a local pairing,
not one proven TU across the scattered accessor band or inclusion of either
distant callee. No membership is inferred for the following music helper
`ovl_11_func_800E6AB0` merely from its adjacency.
