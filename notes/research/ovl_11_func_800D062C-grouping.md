# ovl_11_func_800D062C grouping evidence

Verified source/evidence identity supplied by the documentation handoff:
`76528ce51c952dde28cc5c8fecc9198cfac1f13d4e491fe5af98d3365bc40313`.
The function passed the full finalize gate before this notes-only update.

## Confirmed nested helper/caller pair

- `src/overlays/ovl_11/ovl_11_func_800D062C.c` defines the three-way
  comparator `ovl_11_func_800D0600` inside the caller. The translation unit
  supplies both functions; the comparator's own source file defines nothing.
- Original assembly under `build/ovl_11/asm/nonmatchings/` places the
  comparator at 0x800D0600 with size 0x2C and its caller immediately afterward
  at 0x800D062C with size 0x6C. The caller invokes the comparator twice,
  establishing `$v0 = $sp + 0x10` in both call delay slots; the comparator
  stores the incoming static chain on entry.
- `configs/splat/ovl_11.yaml` lists consecutive offsets 0x187E0, 0x1880C
  and 0x18878 for 800D0600, 800D062C and 800D0698. The verified build
  preserves this layout with the nested definition supplying the first pair.

The new result promotes the previously scratch-only nested pair to an
integrated, fully verified same-TU reconstruction. Its mechanism is documented
in [the sibling nested-function study](ovl_11_func_800D1CFC-nested-function-cse-not-expected.md):
compiling the nested definition changes expansion state in the parent and
reproduces the table-address formation that a declaration alone missed.
The comparator's entry store is a static-chain spill, not evidence requiring
an explicit register-capture construct.

## Limits

This confirms the 800D0600/800D062C pair, not the original TU's outer
boundaries. The adjacent caller 800D0698 remains a low-confidence candidate
extension on the [existing call/layout evidence](ovl_11_func_800D0698-grouping.md).
The analogous nested pair at 800D1CD0/800D1CFC is a family precedent, not
proof that both pairs belonged to one original TU.
