# ovl_19_func_800B8190 — completion/exit callback membership evidence

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`88c7dfcdf52150f3de0ce8592c1d5a7ad8239c55bbadd3892c1b075be43adba1`.
Supplied changed files: `include/globals_override.h`,
`include/overlays/ovl_19.h` and
`src/overlays/ovl_19/ovl_19_func_800B8190.c`. This notes-only review did not
rerun matching or change build inputs.

## New grouping evidence

The verified target loads the signed halfword `D_800BF6C2`, tests it against
-1 and passes a valid value to `func_8001FAE8`. This address is exactly
`D_800BF4C0 + 0x202`, or `D_800BF4C0[0x101]`, not merely a nearby global.
Existing cluster member `800B843C` initializes that slot to -1; `800B8F38`
and `800B9AB0` store `func_8001FABC` returns there. Their extracted assembly
explicitly names `D_800BF6C2` in the stores at 0x800B8F94 and 0x800B9CB8.
The target thus consumes the same overlay-local handle that cluster members
initialize and produce. This direct shared-state connection is additional to
the callback adjacency and `800BA770` call already recorded in earlier notes.

The existing call/link evidence agrees: the target calls cluster render driver
`800BA770` and occupies the final entry (index 4) of outer-callback table
`D_800BCE94`, immediately after candidate member `800B8150`. Text order is
gapless: `800B8150 + 0x40 = 800B8190`, and
`800B8190 + 0x128 = 800B82B8`, the recorded cluster initializer. On completion,
the target writes 0xFF to `D_8006C838 + 0x448C`, the same outer selector that
`800B7ED8` uses to dispatch the table, and sets the 0x80000 flag at base +0xC.

## Membership conclusion and limits

Add only `ovl_19_func_800B8190` as a candidate member of the existing
`D_800BF4D0`/`D_800BF4C0` state-consumer cluster: the final outer callback,
running the render driver before completion-gated cleanup and exit marking.

The same-TU inference assumes the shared overlay-local state and ordered
callback family belonged to one original module. Address identity establishes
a shared slot, not static linkage or the original array declaration; it could
have been accessed through externs across TUs. No TU boundary is recovered,
and the distant `800BBD14` callee is not added or merged by this review.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B8190.c` and
  `include/globals_override.h`: verified handle load, declaration and exit sequence.
- `src/overlays/ovl_19/ovl_19_func_800B843C.c`,
  `src/overlays/ovl_19/ovl_19_func_800B8F38.c` and
  `src/overlays/ovl_19/ovl_19_func_800B9AB0.c`: shared-slot initialization and writes.
- `build/ovl_19/functions/ovl_19_func_800B8190.s`: size 0x128,
  signed handle load at 0x800B81D4 and outer-selector store at 0x800B8220.
- `build/ovl_19/functions/ovl_19_func_800B843C.s`,
  `build/ovl_19/functions/ovl_19_func_800B8F38.s` and
  `build/ovl_19/functions/ovl_19_func_800B9AB0.s`: original shared-slot stores.
- `build/ovl_19/asm/data/3F54.data.s`: callback table and state-label addresses.
- `build/ovl_19/asm/nonmatchings/ovl_19_func_800B7ED8/ovl_19_func_800B7ED8.s`:
  outer-selector load and indirect dispatch.
- `configs/splat/ovl_19.yaml`: consecutive text offsets 0x330, 0x370 and 0x498.
- [Earlier transition-callback evidence](ovl_19_func_800B8150-grouping.md):
  previously recorded adjacency and shared render-driver connection.
