# ovl_31_func_800B7FCC — shared directory-result storage

## Verified handoff

The documentation handoff reports byte-exact source and a passed full finalize
gate for `src/overlays/ovl_31/ovl_31_func_800B7FCC.c`, evidence identity
`f532d3bbf73a60dae273ddb1266516674079a731e64d6d8e28c903402a478382`.
This notes-only pass does not repeat the solver or verification work.

## New membership evidence

The existing `ovl_31` memory-card service group already records the driver's
five helper calls, SDK family and link adjacency. The verified driver adds a
shared-global producer/consumer relationship with `ovl_31_func_800B83B8`:

- The helper passes `D_800B88B0` and `D_800B8868` to `MemCardGetDirentry` as
  the directory buffer and count output, then formats the summary into
  `D_800B8B10` and returns 2.
- After dispatching that helper and retaining its result in `D_800B886C`, the
  driver handles status 2 by reading the count at `D_800B8868`, printing names
  from `D_800B88B0` at stride 0x28, and printing `D_800B8B10`.

This shared result-storage cluster independently corroborates the driver's
membership with the directory helper in the existing candidate TU. It does
not establish static linkage, prove the whole overlay was one original TU,
or identify a new TU boundary. No membership outside the existing group is
inferred from it.

## Evidence paths

- `src/overlays/ovl_31/ovl_31_func_800B7FCC.c`: verified dispatch and display.
- `build/ovl_31/asm/nonmatchings/ovl_31_func_800B7FCC/ovl_31_func_800B7FCC.s`:
  original count/name/summary display at 0x800B8080–0x800B80E0.
- `build/ovl_31/asm/nonmatchings/ovl_31_func_800B83B8/ovl_31_func_800B83B8.s`:
  original directory outputs at 0x800B83E4–0x800B8404, summary destination at
  0x800B8460–0x800B8474, and return 2 at 0x800B8484. This is the evidence for
  the helper, not its disabled, nonmatching C attempt.
- `include/globals_override.h`: directory-buffer and summary declarations;
  the symbols' same-address uses in other overlays are not grouping evidence.
