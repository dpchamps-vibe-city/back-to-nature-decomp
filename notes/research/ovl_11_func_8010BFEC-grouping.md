# ovl_11_func_8010BFEC: dispatcher-family handler extension

## Verified handoff

The static-origin handoff reports byte-exactness and a passed full finalize
gate for `ovl_11_func_8010BFEC`, with source/evidence identity
`92facf33b9465be0561f7677c7b3b04684f6c96a45da1d31089ffa28324ecdd0`.
Its supplied changed-file list names
`src/overlays/ovl_11/ovl_11_func_8010BFEC.c`.
This documentation pass read that source and did not change build inputs or
rerun matching.

## New grouping evidence

- **Named dispatch-table membership.**
  `build/ovl_11/asm/data/2BB0.rodata.s` places the target at
  `D_800BAA34[18]` (0x800BAA7C) and its caller `ovl_11_func_8010BF8C` at
  index 13 (0x800BAA68). The existing family member
  `src/overlays/ovl_11/ovl_11_func_8010B4D8.c` calls through this table,
  indexed by the object's halfword at +0x26. The verified target reads that
  same selection, distinguishing 13 from other values before calling
  `ovl_11_func_800D12B8`.
- **Call graph agrees with gapless link order.**
  `src/overlays/ovl_11/ovl_11_func_8010BF8C.c` calls the target with the same
  object. `build/ovl_11/functions.csv` independently records that call and
  a caller size of 0x60: `0x8010BF8C + 0x60 = 0x8010BFEC`.
  `configs/splat/ovl_11.yaml` lists consecutive offsets 0x5416C and 0x541CC.
  Thus this is a directly coupled caller/helper pair, not adjacency alone.
- **Complementary flag handling.** The caller sets bit 0x800 at object
  +0x34, then either clears it itself or invokes the target. The verified
  target unconditionally clears that same bit before returning. The
  dispatcher `8010B4D8` tests it to gate selection refresh. This corroborates
  the shared handler lifecycle across the table and local call edge.

## Conclusion and limits

Record the target as a **low-confidence candidate handler extension** of the
existing `D_800BA9E4` / `D_800BAA34` dispatcher family. The local caller/helper
pair has call/link-order corroboration; associating it with the dispatchers
additionally relies on named table membership and complementary state access.
These observations do not prove an original translation-unit boundary or
raise the existing family's confidence.

Do not infer static linkage, a declaration-order effect, or original shared
type ownership from the reconstructed source declarations. The common callee
`ovl_11_func_800D12B8` alone is not grouping evidence. This entry does not add
other table handlers or merge the separately recorded neighboring groups.
