# ovl_15_func_801331C4 — grouping evidence

Verified handoff identity:
`11097277e05c75c6de628e033ce06e7bbaa572bd817ca9f4f47acc2f69624b78`.
The handoff reports byte identity and a passed full finalize gate for
`src/overlays/ovl_15/ovl_15_func_801331C4.c` and
`include/overlays/ovl_15.h`. This review changes notes only; it does not
rerun matching.

## Candidate extension of the existing dual-base slot group

- The verified body walks row 1 of `Ovl15Slots`, skips type -1, and copies
  selected records into the runtime pools rooted at `D_800742EC` and
  `D_800749F4` (the latter is the former plus 0x708). The alias in
  `include/globals_override.h` identifies `Ovl15Slots` as `D_80140F90`.
  Existing members `src/overlays/ovl_15/ovl_15_func_801320A8.c` and
  `src/overlays/ovl_15/ovl_15_func_801335F8.c` traverse this same two-row
  selection table and use the same runtime record layout (0xB4/0xB8).
- Original assembly at
  `build/ovl_15/asm/nonmatchings/ovl_15_func_801331C4/ovl_15_func_801331C4.s`
  corroborates the table base at 0x801331D0/0x801331D8, row-1 type access
  at 0x80133208, and calls to `ovl_15_func_8013345C` at 0x8013323C and
  0x80133330. Thus there is a direct shared-table tie, not just reuse of
  a generic copy routine.
- `configs/splat/ovl_15.yaml` puts the target at 0x53DC (size 0x298),
  ending exactly at its callee `ovl_15_func_8013345C`, 0x5674 (size 0xDC).
  That helper ends at 0x5750, the existing member `ovl_15_func_80133538`,
  followed by existing member `ovl_15_func_801335F8` at 0x5810.
  The helper's original assembly at
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8013345C/ovl_15_func_8013345C.s`
  scans the same two record pools and writes the chosen index through its
  third argument. This is corroborating caller/callee link adjacency near
  the established pair, not proof that every function in the run is one TU.

Add only the target as a low-confidence candidate member of the existing
`D_800742EC/D_801376D0` dual-base slot group. It does not directly reference
`D_801376D0`; membership rests on the shared selection table/runtime layout
and the adjacent helper call. These facts establish neither static linkage
nor an exact original TU boundary. Do not merge the separate adjacent-u16
state group or infer a shared compiler-flag requirement from this evidence.
