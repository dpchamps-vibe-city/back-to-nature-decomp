# ovl_11_func_800F6A24: texture-ID group loader extension

## Verified premise

The supplied finalize identity is
`82d10411461939d85091392d7c2ced5c37015d368d2bb66f02021effb73469dd`.
`run_output/autoloop/state.json`, entry
`completions.ovl_11_func_800F6A24`, records agent origin, verification passed,
and changed files `include/game_types.h` and
`src/overlays/ovl_11/ovl_11_func_800F6A24.c`. Their recorded SHA-256 values,
respectively `ef3b38b6ce66f72d07dab38b2a451545d10cf9815025ad25146fd34320c2c688`
and `94d15c8b34c9f690695cbd5cdd644c7a5d3132f28525d0e747c9f2645b972bc4`,
match the files read for this documentation. No solver or new build was run.

## New membership evidence

Extend the existing [texture-ID lookup pair](ovl_11_func_800F6C78-grouping.md)
with its immediate predecessor `ovl_11_func_800F6A24`. Candidate members are
`800F6A24`, `800F6C78`, and `800F6D14` (medium confidence; original TU
boundaries remain unproven).

- **Shared record lifecycle.** The verified loader searches 72 records at
  `D_80129648`, stride `0x22`, for a zero unsigned ID at `+0x20`, then stores
  the requested ID there and initializes the texture dimensions, texture
  coordinates, and CLUT coordinates before uploading. The existing pair
  searches the same ID field with the same bound and stride. On a miss,
  `800F6D14` finds an inactive record using byte `+0x1E`, clears its ID, calls
  this loader, and retries through `800F6C78`. Thus the new source confirms
  the first-free-slot producer that completes the neighbour's reclaim/load/
  lookup path, not just another generic table reference. Original evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F6A24/ovl_11_func_800F6A24.s`
  (`0x800F6A38–0x800F6A8C`, `0x800F6B18–0x800F6C54`),
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F6C78/ovl_11_func_800F6C78.s`
  (`0x800F6CD0–0x800F6D00`), and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F6D14/ovl_11_func_800F6D14.s`
  (`0x800F6D8C–0x800F6DAC`, `0x800F6DB8–0x800F6E18`).
- **Call graph and gapless link order agree.** The loader's `0x254` bytes
  end exactly at `0x800F6C78`; that helper's `0x9C` bytes end exactly at
  `0x800F6D14`. `configs/splat/ovl_11.yaml` places the three consecutively
  at offsets `0x3EC04`, `0x3EE58`, and `0x3EEF4`.
  `build/ovl_11/functions.csv` corroborates both sizes and the caller's
  edges to the loader and lookup helper; the original `jal` instructions
  are at `0x800F6D94` and `0x800F6DA8`.

## Limits

The inference depends on the shared record lifecycle plus the consecutive
loader/helper/caller link run. Neither the modern shared record view nor
matching source-file separation proves the original C types or TU boundaries.
No static/GP-relative ownership, declaration-order effect, register-variable
quirk, or distinctive shared SDK idiom was established. No membership outside
these three is claimed here. In particular, the distant
[texture-draw pair](ovl_11_func_800F8F3C-grouping.md) remains separate; a shared
table and drawing dependencies do not establish one table-wide TU.
