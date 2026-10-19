# ovl_11_func_800CF044: adjacent lookup-helper extension

## Verified premise

Documentation-only review of the supplied byte-exact, finalize-gated source.
Source/evidence identity:
`259c8f8778aa7981ef5d99d97d3bdcfb3f6a6d0b93ecf40ba6d70ae37838a3e9`.
The handoff lists `configs/splat/ovl_11.yaml`, `include/sdk_types.h` and
`src/overlays/ovl_11/ovl_11_func_800CF044.c` as changed files. This review
makes no build-input changes and does not rerun matching.

## New membership evidence

CF044 supplies a **low-confidence candidate same-TU link to its immediate
successor, ovl_11_func_800CF258**:

- Call graph and original link order agree: CF044 occupies 0x214 bytes,
  so `0x800CF044 + 0x214 == 0x800CF258`. The splat text offsets are
  0x17224 and 0x17438. Its `jal` at 0x800CF20C calls that successor.
- This is an operative helper dependency: CF044 passes the selected
  record's u16 ID at +0x00, then branches on the lookup result at
  0x800CF214. A zero result calls the 0xB0 record clearer 800D3200;
  a nonzero result preserves the updated record. CF258 implements the
  corresponding 17-entry item-ID/mask test over D_80123758.

The preceding CEFCC/CF044 helper/caller pair and CF044's pool/flag selection
were [already recorded](ovl_11_func_800CEFCC-grouping.md). The new link
extends that narrow candidate run through CF258; it is not new evidence
for merging all pool consumers.

Evidence paths:

- `src/overlays/ovl_11/ovl_11_func_800CF044.c`
- `src/overlays/ovl_11/ovl_11_func_800CF258.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800CF044/ovl_11_func_800CF044.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800CF258/ovl_11_func_800CF258.s`
- `configs/splat/ovl_11.yaml` (text offsets 0x171AC / 0x17224 / 0x17438)

## Limits

The inference is conditional on the adjacent caller and lookup helper
having originated together; an adjacent cross-TU call remains possible.
No private static, data ownership, declaration-order effect or shared
register-variable quirk is established. CF258 also has distant callers,
so adjacency plus a call is not proof of exclusive helper ownership.
Its existing D_80123758 lookup/spawn-cluster membership is separate evidence,
not grounds to merge that entire cluster with CEFCC/CF044. The reset/update
pair, other pool consumers and distant service callees remain separate;
no original TU boundary is identified.
