# ovl_21_func_800B806C — shared-state table dispatcher

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with source/evidence identity
`1ef1cceb0910d4bd3d7d1edb483cad865ddf09f6f46a3a90e8c37d7b4640c153`.
The supplied changed files are `include/globals_override.h`,
`include/overlays/ovl_21.h`, and
`src/overlays/ovl_21/ovl_21_func_800B806C.c`. This note does not rerun matching
or change build inputs.

## New grouping evidence

- The verified source indexes `D_800BCB6C` with the signed leading halfword
  of `D_800C0448`. The extracted target assembly at
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B806C/ovl_21_func_800B806C.s`
  confirms the `lh` at 0x800B80A4 and indirect call at 0x800B80BC.
  This is the same state halfword written by existing cluster members
  `src/overlays/ovl_21/ovl_21_func_800B9538.c` and
  `src/overlays/ovl_21/ovl_21_func_800B95EC.c`.
- `build/ovl_21/asm/data/3C64.data.s`, at `D_800BCB6C`
  (0x800BCB6C–0x800BCBB8, end exclusive), supplies 19 handler pointers in
  ascending link order, from `ovl_21_func_800B859C` through
  `ovl_21_func_800B96B0`. `configs/splat/ovl_21.yaml` records exactly those
  consecutive code functions at offsets 0x77C–0x1890. The table includes the
  existing state-writer run 800B9538/800B9590/800B95EC/800B9644; the verified
  dispatcher therefore connects that shared-state cluster to a table whose
  call graph and handler link order agree.
- The verified source also repeats the four-call update sequence
  `800BB138` → `800BB250` → `func_8001B5DC` → `800BB2B4` found in
  `src/overlays/ovl_21/ovl_21_func_800B80D4.c`. Its 0x68-byte target body
  ends exactly at that wrapper's entry (0x800B806C + 0x68 = 0x800B80D4),
  corroborating a local wrapper family without adding the neighbour as a
  separately established ledger member.

## Membership conclusion and limits

Add only `ovl_21_func_800B806C` to the existing `ovl_21` D_800C0448
state-pair cluster, as its state-table dispatcher. Under that cluster's
same-TU hypothesis, shared-state access plus the ordered handler table support
this membership. Confidence remains medium: globals, tables and calls can
cross translation units, and adjacency alone does not establish a boundary.
No new TU boundary, data ownership, or merger with the far-buffer cluster is
established.
