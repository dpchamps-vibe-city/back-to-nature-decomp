# ovl_11_func_800DEF30 — handler-dispatcher grouping

Verified source/evidence identity supplied by the finalization handoff:
`e8ffab31a1b9909c90dbd8c115626623fa5aca165283717859fc61dd6e15bc0d`.
The handoff reports byte identity and the full finalize gate; this notes-only
review did not rerun matching or change build inputs.

## New membership evidence

`ovl_11_func_800DEF30` was already listed in the `800DF4F0` predicate-caller
run. Its verified body adds a specific tie to the existing two-table dispatcher
group: it invokes `D_800B95CC[u16@+0x26]` with the object pointer, while
`ovl_11_func_800DF010` tests that same table before writing the selected id to
`s16@+0x26` and clearing the object's `+0x28/+0x2A/+0x2C` state. Both functions
also use flag `0x800` at `s32@+0x34`; DEF30 advances the `+0x2C` timer. This is
a shared handler-selection/update relationship, not merely a common engine call.

The relationship agrees with gapless link adjacency: DEF30 occupies 0xE0 bytes
and ends at `0x800DEF30 + 0xE0 = 0x800DF010`. The splat entries likewise advance
from file offset `0x27110` to `0x271F0`. `D_800B95CC` is the 20-word table
immediately after `D_800B957C`; its entries name the documented `800DFC44+`
handler run. Thus DEF30 is a medium-confidence same-TU candidate alongside
DF010 in the existing two-table dispatcher group, strengthening its already
recorded predicate-run membership.

**Limits:** shared table use and paired object-state access do not establish
static ownership or an original TU boundary. Do not infer that every table
handler or every function in the surrounding address span belongs to that TU.
The widely used `D_8007AFF0` far-state access is not new grouping evidence.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800DEF30.c`: verified table invocation,
  predicate guard, flag and timer accesses.
- `src/overlays/ovl_11/ovl_11_func_800DF010.c`: table check and handler-state
  selection/reset.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800DEF30/ovl_11_func_800DEF30.s`:
  original table relocation, indirect call and 0xE0 extent.
- `configs/splat/ovl_11.yaml`: consecutive DEF30/DF010 link-order entries.
- `build/ovl_11/asm/data/168C.rodata.s`: adjacent `D_800B957C` and
  `D_800B95CC` tables and their handler entries.
