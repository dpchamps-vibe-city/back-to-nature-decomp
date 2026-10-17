# ovl_11_func_8011D35C grouping evidence

## Verified handoff

- Source/evidence identity: `9eec40e30282e064b4d417e4e436b778af1d02d1bad9d427b5c6fbd52ddfa365` (origin: agent).
- The handoff reports byte identity and a passing full finalize gate; this documentation role does not rerun matching or alter build inputs.
- Supplied changed files: `include/globals_override.h`, `include/overlays/ovl_11.h`, `src/overlays/ovl_11/ovl_11_func_8011D35C.c`.

## Existing draw-group evidence retained

The ledger already grouped `8011D0C8` and `8011D10C` with their shared
callee `8011D150` on zero-gap link order and direct calls. The shared body
selects `D_800517E0` or `D_800517EE` and resolves it through
`D_80054BC0[0] + base`. The previous entry also related this pointer-base
idiom to `8011A9DC`, `8011B210`, `8011DD48`, `8011CEE0` and `8011CF10`;
that is prior context, not a new claim that this turn establishes common
TU membership with those more distant functions.

## New membership evidence

The verified source calls `ovl_11_func_8011D10C` with the current
`D_801282E2` selection, then passes that same selection's address to
`ovl_11_func_8011D264`. This connects the existing sprite-draw variant trio
(`8011D0C8`, `8011D10C`, `8011D150`) to a selection/input pair.

Link order corroborates these calls: the original draw body at `8011D150`
has size `0x114` and ends at `8011D264`; that input helper has size `0xF8`
and ends at `8011D35C`. The verified wrapper is `0xA4` bytes, ending at
`8011D400`. Its calls to the earlier draw forwarder and its immediate
predecessor agree with this contiguous run.

Evidence paths:

- `src/overlays/ovl_11/ovl_11_func_8011D35C.c`: verified draw/input wrapper and selection-state flow.
- `src/overlays/ovl_11/ovl_11_func_8011D10C.c`: variant-1 forwarder to `8011D150`.
- `src/overlays/ovl_11/ovl_11_func_8011D150.c`: shared draw implementation.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8011D264/ovl_11_func_8011D264.s`: original input helper; updates the pointed-to halfword, wraps it to 0/1 and reports completion.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_8011D35C/ovl_11_func_8011D35C.s`: original call edges and function extent.
- `configs/symbols/ovl_11.txt` and `configs/splat/ovl_11.yaml`: symbol addresses and contiguous splits.

## Limits

This supports a **low-confidence extension** of the existing draw trio's
candidate source-file group, not a demonstrated original TU boundary.
`D_801282E2` is directly named only by the wrapper in the inspected source;
passing its address to a generic input helper does not establish shared
static ownership. No new GP-relative ownership, declaration-order effect or
register-variable fingerprint is established. The adjacency of `8011D400`
alone does not merge its separately recorded group into this one.
