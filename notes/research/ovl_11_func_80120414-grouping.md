# ovl_11_func_80120414 — table-selection cluster extension

## Provenance

Notes-only follow-up to the supplied byte-exact/full-finalize handoff for
`src/overlays/ovl_11/ovl_11_func_80120414.c` and
`include/overlays/ovl_11.h`. Supplied verified source/evidence identity:
`3f7d41fd7e35df9d7032911de69905bc90d4df10ba7dae80891924ed536e9019`.
No matching experiments or build-input edits were performed for this note.

## New grouping evidence

Extend the existing D_801285xx table-lookup candidate through `80120414`,
explicitly recording its common driver `8011FFA8`. Confidence remains medium;
this is a candidate original TU, not a recovered boundary.

- **Call graph and gapless link order agree.** In
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8011FFA8/ovl_11_func_8011FFA8.s`,
  the driver loads `$s2 = &D_8012DB48` and passes it to `80120358` at
  `801200EC`, `801201DC` and `80120310`, and to `80120414` at `80120298`.
  Both helpers index their tables with the same object's word at +8.
  `configs/splat/ovl_11.yaml` orders the three at file offsets `0x68188`,
  `0x68538` and `0x685F4`. The target assembly sizes corroborate zero gaps:
  `8011FFA8 + 0x3B0 = 80120358`, `80120358 + 0xBC = 80120414`.
- **Their data order agrees too.**
  `build/ovl_11/asm/data/69960.data.s` places `80120358`'s four range records
  at `D_801285F4`, followed by its six-halfword `D_8012862C` table ending
  at `80128638`. At that exact address begins the first default weighted
  table for `80120414`; `D_80128748` points to `D_80128638`, `D_80128664`,
  `D_80128678` and `D_801286A0`. Four override tables occupy
  `D_801286D0` through `D_80128747`, followed by that pointer array and
  six fallback halfwords at `D_80128758`. Thus the newly identified tables
  continue the predecessor's data cluster, rather than merely lying in
  the same overlay. The helper roles and +8 indexing are visible in
  `src/overlays/ovl_11/ovl_11_func_80120358.c` and the verified target source.

## Roles

- `8011FFA8`: state-machine driver using the common selection state and both helpers.
- `80120358`: min/max range selector that seeds a random delay and returns a table value.
- `80120414`: weighted value selector with eligibility/one-shot gates and periodic fallback.

## Limits

The call/shared-state/link-order tie supports the local three-function group;
its attachment to the wider D_801285xx family inherits that family's existing
candidate premise. Neither code nor data adjacency alone proves a TU boundary.
The common `func_80012A34` call and broad `D_8006C838` state object are not
independent same-file evidence.

`D_8012DB8C` is not private to the target: the assembly for
`ovl_11_func_800BCD28` clears it at `800BCF08`. That distant reset does not
justify adding the overlay initializer to this candidate. Likewise, the driver
calls the next helper `80120850`, but this note does not infer a further TU
extension from that call alone. No static linkage, declaration-order effect or
shared register-variable quirk was established by this handoff.
