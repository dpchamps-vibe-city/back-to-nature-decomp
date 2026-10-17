# ovl_11_func_800E93CC grouping evidence

The verified function supports a candidate local same-TU pair with its
zero-gap successor `ovl_11_func_800E946C`, within the existing `D_80129560`
accessor family. The new evidence is a shared overlay-private table and
signed-halfword slot view, corroborated by link order. This corrects the
ledger's earlier description of `800E93CC` as a non-table-accessing stub;
it does not establish an original TU boundary.

Supplied source/evidence identity:
`9e22325105271dd3e130932d328c0fdb40c178d5827bb3b4601927bd91871341`.
The handoff reports byte identity and a passed full finalize gate, with
changes to `include/overlays/ovl_11.h` and
`src/overlays/ovl_11/ovl_11_func_800E93CC.c`. Neither file is modified by
this documentation.

## Evidence

- `src/overlays/ovl_11/ovl_11_func_800E93CC.c` conditionally resolves
  `arg0` through the signed low halfword of `D_80129560[arg0]` when
  `arg1 != 0`, otherwise using the raw item ID. Its role is an item
  eligibility predicate: ID `0xA4` succeeds directly; other IDs require
  `(flags & 0x1C00) != 0` and a low-halfword result below 3 from
  `ovl_11_func_800D60F0(id, 3)`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E93CC/ovl_11_func_800E93CC.s`
  confirms the absolute `D_80129560` base at `800E93E4`/`800E93E8`,
  four-byte slot stride at `800E93EC`, and signed `lh` at `800E93F4`.
  `include/globals_override.h` identifies the shared table as a 0x50-byte
  ovl_11 s32 array.
- `src/overlays/ovl_11/ovl_11_func_800E946C.c` resolves four slots guarded
  against `-1`, using the same signed-halfword view for its first two
  arguments and full s32 loads for the other two, then forwards them to
  `ovl_11_func_800E7504`. Its corresponding original assembly confirms
  the same absolute base, stride and `lh` sequence at
  `800E948C`–`800E949C`. This member supplies the already-recorded call
  edge from the slot-accessor family to the `D_80076220` record-reset
  family; it is not a call between the candidate pair.
- `configs/splat/ovl_11.yaml` places the pair consecutively at offsets
  `0x315AC` and `0x3164C`. The target assembly's 0xA0-byte extent ends
  exactly at `800E946C`, making the shared table/view a local adjacency
  tie rather than only a scattered global dependency.

## Limits

The pair does not call each other, and no shared static, declaration-order
or register-variable quirk is established. The immediate predecessor
`800E9384` does not use this table; adjacency alone does not add it to the
pair. The target's `D_8006C858` item-table read was already recorded as a
scattered dependency, so it does not newly join the distant
`800D5750`–`800D6090` accessor TU hypothesis. Membership remains conditional:
the shared private-table view plus local adjacency supports the candidate
pair, not a merger of the whole accessor band or its callees into one TU.
