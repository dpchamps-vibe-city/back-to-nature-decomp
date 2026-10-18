# ovl_11_func_800BF4B4 — local grid-query run

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate for evidence identity
`e127a479ae1771f57d7471131f619e18ab4c7067a339f82d4a2fc17b84a9b6fb`.
Its changed-file list is `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800BF4B4.c`. No matching work was repeated.

## New membership evidence

- The verified source calls `ovl_11_func_800BF630` to classify the near and
  far grid cells derived from a directional offset.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800BF4B4/ovl_11_func_800BF4B4.s`
  independently records those calls at 0x800BF5D8 and 0x800BF5FC, and gives
  the probe's size as 0x17C: 0x800BF4B4 + 0x17C = 0x800BF630.
- `configs/splat/ovl_11.yaml` places the probe and classifier consecutively
  at offsets 0x7694 and 0x7810, followed without gaps by `800BF720` at
  0x7900 and `800BF844` at 0x7A24. Thus the call graph corroborates the
  immediate link-order join to the existing 800BF630–800BF844 candidate run.
- The existing run's table evidence remains local: the classifier assembly
  (`build/ovl_11/asm/nonmatchings/ovl_11_func_800BF630/ovl_11_func_800BF630.s`)
  and `src/overlays/ovl_11/ovl_11_func_800BF720.c` select the 7-wide
  `D_80074124` or 45-wide `D_80071DFC` grid from the same nonzero/zero
  selector convention; `ovl_11_func_800BF844.c` walks the latter grid.

This supports adding `800BF4B4` to that local same-TU candidate, with the
strongest new edge being the adjacent caller/classifier pair. It does not
establish an original object boundary or prove that all four functions
belonged to one TU.

## Limits

The probe's four-byte-stride signed halfword reads from `D_801281F0` confirm
its already recorded data-family tie to `800CE034` and `80113310`; they do
not establish same-TU membership with those distant consumers. The probe
also calls the distant coordinate helpers `800D7EF8` and `8011E090`, but
those calls do not extend the local TU candidate to the helpers. No new
shared-static, declaration-order, or register-variable evidence is claimed.
