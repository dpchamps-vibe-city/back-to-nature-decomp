# ovl_11_func_800CFE40: candidate object-render variant run

## Verified premise

The supplied handoff records byte-exact matching and a passed full finalize
gate for identity
`c6098d3521beefe59eb49940fc6debea1113dadaa548266d8eb16214f7d509c7`,
with changes to `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800CFE40.c`. This note documents that
verified source and original assembly; it does not rerun matching.

## Membership evidence

Candidate members: `ovl_11_func_800CFE40`, `ovl_11_func_800D001C`,
`ovl_11_func_800D0198`, and `ovl_11_func_800D037C`.

- **Gapless code order agrees with the call graph.**
  `configs/splat/ovl_11.yaml` places the four at offsets `0x18020`,
  `0x181FC`, `0x18378`, and `0x1855C`. Original lengths `0x1DC`, `0x17C`,
  and `0x1E4` make the three draw variants consecutive, followed immediately
  by their common helper. They call `800D037C` at `0x800CFFD0`,
  `0x800D0154`, and `0x800D032C`, respectively.
- **Adjacent, analogous scratch storage.** The verified target uses
  `D_80128D38` for three position halfwords and `D_80128D40` for two screen
  words. `800D0198` uses `D_80128D50` and `D_80128D48` for the corresponding
  roles. `build/ovl_11/asm/data/69960.data.s` places these four eight-byte
  slots consecutively at `0x80128D38..0x80128D57`. These are distinct
  buffers, not a demonstrated shared static.
- **A specific render pipeline ties the variants together.** All three
  gate drawing on `D_8006C838+0x0C` bit `0x800000`, project object
  `+0x38/+0x3C/+0x40` coordinates after the same `+150/-100/-150` offsets,
  submit a companion from object `+0xA8`, call the following `800D037C`,
  and gate the final `func_800158E4(object+0x78)` on bit `0x08000000`.
  The target and `800D0198` additionally share the object `+0x34` bit
  `0x01000000` path that reprojects with fixed Y = -100 through
  `func_8001DFD4`; otherwise they copy the first projected screen pair.
  `800D001C` uses stack coordinate storage and fixed companion offsets.

Original draw evidence is in
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for each of the
three variants. Helper evidence is
`build/ovl_11/functions/ovl_11_func_800D037C.s` and
`src/overlays/ovl_11/ovl_11_func_800D037C.c`: it suppresses the
`800C9E90` marker draw when flag `0x1000` is set and forwards the high flag
bit as the final argument otherwise.

## Limits

Low-confidence same-TU candidate, conditional on the adjacent render variants
and their neighboring scratch pairs having originated together. Similar
render pipelines, contiguous code and contiguous data can also span separate
original objects. No original static linkage, declaration-order effect,
GP-ownership boundary or register-variable quirk was established. The common
projection/render services are not added to this TU, nor are the functions
outside this four-function run; original file boundaries remain unproven.
