# ovl_11_func_800CEFCC: candidate record-update helper/caller pair

## Verified premise

The supplied handoff records byte-exact matching and a passed full finalize
gate for source/evidence identity
`3934b61d5e4779107eab2e690ea845a770cab6817d4c82bdefb66d1f42fb9122`,
with changes to `include/sdk_types.h` and
`src/overlays/ovl_11/ovl_11_func_800CEFCC.c`. This documentation review
makes no build-input changes and does not rerun matching.

## Membership evidence

`ovl_11_func_800CEFCC` and `ovl_11_func_800CF044` form a low-confidence
candidate same-TU helper/caller pair:

- **Call graph agrees with gapless original code order.** The helper is
  0x78 bytes long, so `0x800CEFCC + 0x78 == 0x800CF044`.
  `configs/splat/ovl_11.yaml` independently places the pair at offsets
  0x171AC and 0x17224. The immediate successor calls the helper at
  0x800CF148 and 0x800CF200.
- **The caller's record gate complements the verified helper.** CF044
  selects records with flag 0x20000 at +0x34 from `D_800742EC`,
  `D_800749F4`, `D_800759E4` or `D_80075BC4`, then passes the selected
  record and its incoming tag/position pointers to CEFCC. The helper
  clears that exact flag, copies the halfword-aligned tag to +0x2E and
  four position words to +0x38, then calls `800BDB88(position, 0)`.
  The same helper serves both the special pool branches and the general
  six-record loop.

Evidence paths:

- `src/overlays/ovl_11/ovl_11_func_800CEFCC.c`
- `include/sdk_types.h` (`Ovl11CEFCCObject`)
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800CEFCC/ovl_11_func_800CEFCC.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800CF044/ovl_11_func_800CF044.s`
- `configs/splat/ovl_11.yaml` (offsets 0x171AC / 0x17224 / 0x17438)

## Limits

The inference is conditional on the adjacent helper and its caller having
originated together; a cross-object call between adjacent original TUs
remains possible. CEFCC accesses only its parameters and establishes no
private static, GP ownership, declaration-order effect or register-variable
quirk. Its tag/vector copies alone do not establish a TU-wide SDK idiom.
Neither the distant `800BDB88` service nor the neighboring `800CEEF8` is
added to the pair.

The existing D_80075BC4 ledger section contains this narrower pair because
CF044 consumes that pool. This does not merge the pair with the CE96C/CEBC8
reset/update pair or the other distant pool consumers, establish pool
ownership, or identify original TU boundaries.
