# ovl_19_func_800B86DC — outer-callback membership evidence

## Provenance

The agent handoff reports byte-exact source and a passed full finalize gate,
with verified source/evidence identity
`3a668f777fa287630088b8f4f8ba74e939263bc0dc637453ca37e02fa87f8fbd`.
Supplied changed files: `include/overlays/ovl_19.h` and
`src/overlays/ovl_19/ovl_19_func_800B86DC.c`. This notes-only review did not
rerun matching or change build inputs.

## New grouping evidence

The verified callback uses the existing cluster's `D_800BF4C0` state array
and calls its render driver `800BA770`. It is entry 1 of `D_800BCEA8`,
dispatched by recorded candidate member `800B80E8` using `D_800BF4C4`
(`D_800BF4C0[2]`). When index 4 is zero, or when the completion probe returns
2 and `func_800225B8()` returns 2, the target sets index 2 to 2 and clears
index 3. This explicitly selects `800B87C4`, the nested dispatcher that
consumes index 3 and is the target's immediate table and text successor:
`800B86DC + 0xE8 = 800B87C4`.

The preceding table entry is also its immediate text predecessor:
`800B868C + 0x50 = 800B86DC`. That predecessor calls `800BA770` and writes
1 to `D_800BF4C4` on its input arm, selecting the target. Thus the shared
selector connects consecutive callbacks in both execution and link order.

There is also a complementary exit pattern with recorded cluster member
`800B93B0`: both call `func_8002261C(4, ...)`, gate on
`func_800226A4() == 2`, then use `func_800225B8()` to choose outer state 3
(clearing `D_800BF4C0[0x100]`) or state 2. They differ in when they clear the
inner selector; no identical-body or register-variable claim is made.

## Membership conclusion and limits

Add only `ovl_19_func_800B86DC` as a candidate member of the existing
`D_800BF4D0`/`D_800BF4C0` s16-state consumer cluster: a selection-dependent
outer callback handing off to nested dispatch or the timed outer callback.

This same-TU inference assumes the shared state and ordered callback family
belonged to one original module. Globals, handler tables and calls can cross
TU boundaries; static linkage and an original file boundary are not proven.
No other membership or cluster boundary changes.

## Evidence paths

- `src/overlays/ovl_19/ovl_19_func_800B86DC.c`: verified shared-state accesses,
  render call, selection-dependent commands and state handoffs.
- `src/overlays/ovl_19/ovl_19_func_800B80E8.c` and
  `include/globals_override.h`: outer-table dispatch, signed selector and
  shared s16-array declarations.
- `src/overlays/ovl_19/ovl_19_func_800B87C4.c`: successor consumes the cleared
  inner selector; `src/overlays/ovl_19/ovl_19_func_800B93B0.c`: complementary
  exit pattern on the same fields.
- `build/ovl_19/asm/data/3F54.data.s`: consecutive outer entries at
  0x800BCEA8/0x800BCEAC/0x800BCEB0 for predecessor, target and successor.
- `build/ovl_19/functions/ovl_19_func_800B868C.s`: predecessor size 0x50,
  render call and selector write at 0x800B86C8.
- `build/ovl_19/functions/ovl_19_func_800B86DC.s`: target size 0xE8 and
  selector stores at 0x800B8738/0x800B8740 and 0x800B87A8/0x800B87AC.
- `configs/splat/ovl_19.yaml`: consecutive text offsets 0x86C/0x8BC/0x9A4.
