# ovl_11_func_801035C0: selection-record tie within the handler family

## Evidence identity

The handoff reports byte-exact output and a passed full finalize gate for
source/evidence identity
`97f1b681b593285463c1419209875b596f5527d2f062b98ca3d352a901e9cdeb`
(origin: agent), also recorded for the target in
`run_output/autoloop/state.json`. The supplied changed files were reviewed
without modification:

- `include/globals_override.h`
- `include/overlays/ovl_11.h`
- `src/overlays/ovl_11/ovl_11_func_801035C0.c`

Matching/build checks were not repeated for this documentation-only role.

## New grouping evidence

The ledger already places this helper after the case-0x3C0–0x3C4 handlers
in the `D_801273DC` family, based on calls and gapless link order. Its
verified body adds a concrete shared-record responsibility:

- **Selection-record writer/reader tie:** the target writes the selected
  ID and value at `D_8006C838 + 0x4AE0/+0x4AE2 + index*0x16`, then the
  flags at +0x4AE4 and eight IDs starting at +0x4AE6. Existing family
  member `src/overlays/ovl_11/ovl_11_func_8010021C.c` tests the ID in that
  same 80-record, stride-0x16 array using its page/slot selectors. The
  family's dispatch witness `8010289C` reads the same ID/value pair and
  compares the candidate value against the stored +0x4AE2 field before
  installing the case handlers that call this writer. Thus the helper
  commits the exact records the selection/confirmation handlers inspect;
  this is more specific than sharing the broad `D_8006C838` engine base.
- **Shared selection-state snapshot:** the target calls `80102BCC` for
  the ID, value and existing-record index, then stores `D_801273EE` and
  all eight `D_8012CF00` IDs into that record. The callee preserves those
  same globals while testing candidates and searches this same record
  array for the resulting ID. This supplies a data-flow tie to the nearby
  ID-map run beyond the previously recorded adjacency. See the earlier
  [80102BCC evidence](ovl_11_func_80102BCC-grouping.md). The `_flags` and
  `_ids` declarations alias the original assembler symbols; the new C
  record typedef is not itself evidence of an original TU or shared header.

## Corroboration and evidence paths

The already-known call/link tie remains intact: `80103548 + 0x78 =
801035C0`; that immediate predecessor calls the target at 0x80103598.
`build/ovl_11/functions.csv` also records calls from `80103424` and
`801034D0`. `configs/splat/ovl_11.yaml` places the predecessor and target
at offsets 0x4B728 and 0x4B7A0.

Original instruction evidence:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_801035C0/ovl_11_func_801035C0.s`
  (selector call at 0x801035CC; record-header stores at
  0x8010367C/0x8010368C/0x80103690; ID copy at 0x80103694–0x801036A8).
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8010289C/ovl_11_func_8010289C.s`
  (record ID/value reads at 0x801028F4/0x80102900; candidate selection at
  0x80102940; stored-value comparison at 0x801029B8–0x801029C4 and
  resulting handler installation through 0x801029F8).
- `build/ovl_11/asm/nonmatchings/ovl_11_func_80103548/ovl_11_func_80103548.s`.

`8010289C` and `80103548` remain assembly-backed source stubs; their
observations above come from the original instructions.

## Limits

Assuming specialized shared-record ownership plus the established handler
call/link locality reflects source organization, this corroborates the
helper's existing candidate same-TU membership. It does not prove private
linkage, a declaration-order effect, or an original TU boundary. Keep the
existing groups and confidence levels: the newly observed connection to
`80102BCC` does not by itself merge the ID-map run, handler family or
state-block handler/display cluster.
