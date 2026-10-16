# ovl_11_func_800E7D4C grouping evidence

## Candidate membership

Add `ovl_11_func_800E7D4C` to the existing `D_80129560` accessor family
and `Ovl11D124Entry` table cluster. Retain their existing medium and low
confidence respectively: the new evidence is shared-global/helper-cluster
membership, not proof that either family is one original translation unit.
Do not merge the families on this function's evidence.

Verified source: `src/overlays/ovl_11/ovl_11_func_800E7D4C.c`.
Supplied verified source/evidence identity:
`c4cdec5fdd10780cb443b05ba107e17f63a6258b237a09ff91607990344c2db8`.

## Evidence

Original instructions below are in
`build/ovl_11/asm/nonmatchings/ovl_11_func_800E7D4C/ovl_11_func_800E7D4C.s`.

- **Shared state-slot table.** At 0x800E81A4 the function calls the recorded
  grid-item updater `800D85F8`; 0x800E81AC–0x800E81BC forms the absolute
  `D_80129560` base and stores the returned item ID into its four-byte-stride
  slot. Unlike many recorded accessors, this slot parameter is an unguarded
  `s32`, not an `s16` index with a `-1` sentinel. Membership rests on the
  shared table, not an identical argument-narrowing idiom.
- **Shared grid/helper cluster.** The target selects `D_80071DFC` or
  `D_80074124` at 0x800E7FFC–0x800E8050 and again at
  0x800E814C–0x800E81A0, with the existing 0x168/0x38 row strides and
  eight-byte entries. It restores the halfword at +0 from +2 and clears
  +4/+5/+6. Calls to recorded member `800DADB0` at 0x800E7FE4 and
  0x800E8134, followed in mode 2 by `800D85F8`, corroborate the table/type
  tie. The availability halfword read at 0x800E7E50 is
  `D_8006C838+0x44D4`, also used to gate the seven-wide search in
  `src/overlays/ovl_11/ovl_11_func_800DADB0.c`.
- **Link context, not an independent boundary claim.**
  `configs/splat/ovl_11.yaml` and `build/ovl_11/functions.csv` place recorded
  state-slot accessor `800E7CCC` (0x80 bytes) immediately before the target
  (0x504 bytes), which ends at `800E8250`. The predecessor's verified source
  also accesses `D_80129560`. Neither neighbour appears among the target's
  direct callees; adjacency alone does not establish a new same-file run.

## Limits

The membership remains conditional on the ledger's existing shared-cluster
premises. Shared storage and calls do not establish original internal linkage.
No new static ownership, declaration-order effect, register-variable quirk,
SDK idiom, or original TU boundary is established. The distant grid helpers
are not link-adjacent to this function.
