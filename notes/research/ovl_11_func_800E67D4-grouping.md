# ovl_11_func_800E67D4 grouping evidence

The supplied byte-exact, full-finalize-gate-passed function adds a candidate
member to the existing `D_80129560` accessor family, with a local same-TU tie
to its immediate successor `ovl_11_func_800E6834`. It does not establish an
original TU boundary.

Handoff source/evidence identity:
`1977700d15a8f7444d5a987323cafbe65e7c41ea5a1427caa8f13b76615f49b9`.
The supplied changed files are `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800E67D4.c`; this documentation does not
modify either file.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800E67D4.c` forwards the result of
  `ovl_11_func_800EFC68(arg0, arg1, arg2, arg3)` and
  `D_80129560[arg3]` to `ovl_11_func_800EFDA0`, retaining `arg2` as the
  comparison helper's third argument.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E67D4/ovl_11_func_800E67D4.s`
  confirms the absolute `D_80129560` base, word load, and calls at
  0x800E67F8 and 0x800E6818. Its 0x60-byte extent ends at 0x800E6834.
- `src/overlays/ovl_11/ovl_11_func_800E6834.c`, already recorded in the
  family, likewise passes a `D_80129560` value to
  `ovl_11_func_800EFDA0` with `arg2` as its third argument. Its corresponding
  assembly under `build/ovl_11/asm/nonmatchings/ovl_11_func_800E6834/`
  confirms that table access and call.
- `configs/splat/ovl_11.yaml` places these functions consecutively at
  offsets 0x2E9B4 and 0x2EA14. Shared table use and the shared callee therefore
  corroborate the gapless link adjacency; adjacency alone is not the tie.

Limits: the table slot here is indexed by full-width `arg3`, without a
signed-halfword cast or guard; no shared register-variable or declaration-order
quirk is inferred. The neighbours do not call each other, and an overlay-private
global need not be TU-private. These observations support candidate membership
and a local pairing, not a single proven TU across the whole accessor band or
membership of either distant callee in that TU.
