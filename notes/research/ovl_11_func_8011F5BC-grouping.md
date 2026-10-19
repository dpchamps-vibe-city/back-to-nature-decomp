# ovl_11_func_8011F5BC — selection-driver wrapper grouping evidence

## Provenance

Notes-only review of the supplied byte-exact/full-finalize handoff. Supplied
verified source/evidence identity:
`74c9ebc77af40d010a84d866d68f925178353d836ce34e6e5dd7fdfa063bda59`.
Origin: static. The supplied changed-file list is
`src/overlays/ovl_11/ovl_11_func_8011F5BC.c`.
Verification is supplied, not rerun here; this documentation edits no build inputs.

## New membership evidence

Add the wrapper as a candidate member of the existing D_801285xx–D_801287xx
selection run, without changing its extent or confidence:

- **Shared selection state.** The verified source gates its driver call on
  `D_80128540 != 0`. In
  `src/overlays/ovl_11/ovl_11_func_8011F608.c`, that same halfword selects
  and advances the driver's states; the existing initializer
  `src/overlays/ovl_11/ovl_11_func_8011F52C.c` clears it. Thus the wrapper
  consumes the existing initializer/driver state, not merely nearby data.
- **Call graph and gapless link order agree.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8011F5BC/ovl_11_func_8011F5BC.s`
  loads the halfword at `8011F5C0` and directly calls `8011F608` at
  `8011F5E8`. Its size is `0x4C`, so `8011F5BC + 0x4C = 8011F608`.
  `configs/splat/ovl_11.yaml` corroborates the consecutive starts at file
  offsets `0x6779C` and `0x677E8`.

Role: when selection state is active, save the current context, select context
3, run the selection driver, and restore the saved context.

## Limits

The conditional premise is that this shared-state wrapper/driver pair and its
corroborating link order reflect one original module. Calls can cross TUs, and
absolute addressing does not establish ownership or static linkage. No
register-variable quirk, declaration-order effect or SDK-specific same-file
constraint was established. The general context getter/setter calls do not
justify merging their other callers into this group.

This fills an unlisted position inside the existing candidate range; it does
not establish a TU boundary or infer membership for `8011F574` or the separate
preceding run. The wider group's existing
[driver/reset premises and limits](ovl_11_func_8011F608-grouping.md) remain in force.
