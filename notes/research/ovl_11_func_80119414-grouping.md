# ovl_11_func_80119414 — adjacent callback-wrapper run

The agent handoff reports byte-exact source and a passed full finalize gate for
`src/overlays/ovl_11/ovl_11_func_80119414.c`, the sole changed file in that
handoff, with source/evidence identity
`4f2e514948c7782952f6b30bf14955650959430a4c198f06e1082f625bdecd4b`.
This review is documentation only; no matching experiments or build-input edits
were performed.

## New grouping evidence

The verified function supports a low-confidence same-TU candidate run comprising
`ovl_11_func_80119338`, `80119364`, `80119390`, `801193BC`, `801193E8`,
`80119414` and `80119470` (all with the `ovl_11_func_` prefix):

- **Shared helper-call idiom in adjacent code.** The five preceding C sources
  each call `ovl_11_func_8011C298` with constant arguments and second argument
  2. The verified target is a flag-selected version of that wrapper: it tests
  `D_800719F8` bits 0x40/0x80, calls the same helper with second argument 2,
  and otherwise returns 1. The immediately following handler also has that
  call shape. Its original listing at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80119470/ovl_11_func_80119470.s`
  tests bit 0x20 at `D_8006C838 + 0x51C0`, the same flag-word address
  `0x800719F8`, before selecting its action.
- **Callback order agrees with link order.**
  `build/ovl_11/asm/data/3408.rodata.s` lists these seven functions at
  consecutive words `0x800BB258`–`0x800BB270` within `D_800BB248`, in exactly
  their code-address order. `src/overlays/ovl_11/ovl_11_func_801189C8.c`
  dispatches through this table using `D_8012D520`.
  `configs/splat/ovl_11.yaml` places the seven functions consecutively at
  offsets `0x61518`, `0x61544`, `0x61570`, `0x6159C`, `0x615C8`, `0x615F4`
  and `0x61650`. The target's original listing at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80119414/ovl_11_func_80119414.s`
  records size 0x5C, ending exactly at `0x80119470`.

Thus the newly verified body links the preceding constant-wrapper run to the
following flag-gated wrapper by both behaviour and callback/link order, rather
than by proximity alone.

## Limits

This is candidate membership, not proof of original TU boundaries. Callback
tables can span translation units, the helper is callable across files, and
`D_800719F8` is shared overlay state, not an established private static. The
following handler also uses a different address-base spelling; no common
register-variable or declaration-order quirk was established. Reconstructed
helper prototypes are not evidence of an original shared declaration.

Do not infer membership of the distant helper, dispatcher, all table entries,
or the separately recorded D_800719F8 flag-update families. The seven-function
range is the reviewed local run, not a proposed original file boundary.
