# ovl_11_func_800E880C grouping evidence

Verified handoff source/evidence identity:
`8e059749178be611580f78ed5d9881922ab73cb320735e311d664264d05b0a60`.

The verified body strengthens this function's existing candidate membership
in the `D_801295C6` setter/getter family; it does not establish TU boundaries.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800E880C.c` reads the selector table
  `D_80129560` as well as reading/writing the cached selector `D_801295C6`.
  The ledger previously recorded only its use of the latter. This joint
  access ties it to the same table/flag cluster used by
  `src/overlays/ovl_11/ovl_11_func_800EE604.c`.
- It calls `ovl_11_func_800E60A0` with selected coordinates and sets the same
  `D_8006C838`+0xC bit 0x400000 as that existing family member
  (`src/overlays/ovl_11/ovl_11_func_800E60A0.c`). The callee resets
  `D_801295C6` to -1; this caller writes the selected index on success unless
  arg2 is 2. This corroborates the shared-state relationship.

Limits: these are shared-global and call-graph ties, not evidence of private
linkage or defining-TU ownership. The earlier 800E60A0, this function and
800EE604 are not consecutive functions in `configs/splat/ovl_11.yaml`.
Retain the existing tentative family without merging intervening functions
or claiming one original TU across the full span.
