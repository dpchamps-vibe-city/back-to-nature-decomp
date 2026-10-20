# ovl_11_func_800CECB0: candidate pool reset/update run extension

## Verified premise

The supplied handoff records byte-exact matching and a passed full finalize
gate for source/evidence identity
`19f1af102bcdc3f3d2995221103225777aacc692fa04fc3ce026698f08d47ae8`,
origin agent, with changes to `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800CECB0.c`. This documentation review makes
no build-input changes and does not rerun matching.

## New membership evidence

The verified function extends the existing **low-confidence candidate
same-TU reset/update pair** CE96C / CEBC8 to include CECB0:

- **Specific shared pool-pass sequence:** CEBC8 and CECB0 both begin their
  gated sweeps with ten 0xB4 records at `D_8006C838 + 0x7AB4`, then make
  two separate passes over the same twenty 0xB8 records at `D_800749F4`.
  CECB0 calls E21E4, DEB00 and E06E0 where CEBC8 calls E20F0, DE9C8 and
  E05A8. CE96C initializes these same pools, including the duplicated
  twenty-record pass. This repeated sequence is more specific than merely
  sharing the main-RAM state buffer. The sweeps are not identical: CECB0
  also accepts first-pool records with flag 0x02000000, then handles the
  two 0xF0 records rather than CEBC8's six-record D_80075BC4 sweep.
- **Shared gate and reset idiom:** CECB0 tests the same mask 0x8 at
  `D_8006C838 + 0x4450` as CEBC8, then clears it after the gated work;
  CE96C also clears that mask. CECB0's conditional tail resets three
  0xB4 records at work-area offset +0x97AC to type 0 / ID 0xFFFF,
  matching CE96C's reset of those exact slots. Both functions also call
  `ovl_11_func_800CFD9C`, though this common service alone is not a TU tie.
- **Gapless original link order:** splat places CE96C, CEBC8, CECB0 and
  CEEF8 at offsets 0x16B4C, 0x16DA8, 0x16E90 and 0x170D8. Thus CEBC8 is
  0xE8 bytes and ends exactly at CECB0; CECB0 occupies 0x248 bytes and
  ends at CEEF8. The existing reset/update pair and the new member form
  a contiguous run, corroborating the shared pool/gate fingerprint.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800CECB0.c` (verified target)
- `src/overlays/ovl_11/ovl_11_func_800CE96C.c` (pool and sentinel resets)
- `src/overlays/ovl_11/ovl_11_func_800CEBC8.c` (preceding gated sweep)
- `include/globals_override.h` (`Ovl11PoolResetView`)
- `configs/splat/ovl_11.yaml` (offsets 0x16B4C–0x170D8)
- [Existing reset/update-pair evidence](ovl_11_func_800CE96C-grouping.md)

## Limits

The inference is conditional on the complementary, adjacent pool passes
having originated together. There is no direct caller/callee edge among
these three functions, proven private static, GP ownership, declaration-order
effect or register-variable quirk. The shared C struct view is a modern
reconstruction, not itself evidence of an original declaration. Adjacent
original TUs remain possible; no boundary or pool ownership is established.

CECB0 clears `D_80128D66`, which CF428 reads/sets, and calls CFA48 after
clearing spawn-related flags. Those dependencies provide subsystem context,
not enough evidence to merge this run with the lookup/spawn cluster. Neither
the merely adjacent CEEF8 nor the separate CEFCC / CF044 / CF258 candidate
run nor the distant pool handlers/consumers is added to this run.
