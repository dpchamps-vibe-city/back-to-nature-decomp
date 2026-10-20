# ovl_11_func_80118A68 — adjacent state-dispatch grouping

The handoff reports byte-exact source and a passed full finalize gate with
source/evidence identity
`8a23a7a24060645be761d624b1fa828733f3a719f90503cf1cf1dd018279d466`.
Its changed-file list is `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_80118A68.c`. This is notes-only documentation;
no matching experiments or build-input edits were performed.

## New grouping evidence

The verified function supports a low-confidence extension of the existing
`D_8012D520` reader/dispatcher candidate cluster, specifically beside
`ovl_11_func_801189C8`:

- **Zero-gap code order plus shared selector.**
  `configs/splat/ovl_11.yaml` places `801189C8` at file offset `0x60BA8`
  and `80118A68` at `0x60C48`. The original predecessor in
  `build/ovl_11/functions/ovl_11_func_801189C8.s` is `0xA0` bytes, ending
  immediately before `80118A68`. In the verified C sources, `80118A68`
  gates on and writes `D_8012D520` before invoking its initialization
  callback; `801189C8` uses that same selector to dispatch updates through
  `D_800BB248`.
- **Shared callback storage, not merely adjacent table labels.**
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80118A68/ovl_11_func_80118A68.s`
  indexes `D_800BB2BC` with a four-byte stride. Original data in
  `build/ovl_11/asm/data/3408.rodata.s` places a null word at `0x800BB2BC`
  and `ovl_11_func_80118CB0` at `0x800BB2C0`, so initialization entry 1
  is the reset callback. The predecessor assembly loads that identical
  `0x800BB2C0` word when an update reports completion. The reset source,
  `src/overlays/ovl_11/ovl_11_func_80118CB0.c`, clears `D_8012D520` and
  belongs to the already listed nearby reset/quantity-handler candidate run.

## Limits

This is shared state/callback storage corroborated by immediate code adjacency,
not proof of original TU boundaries or private-static ownership. Absolute
addressing does not establish ownership. The callback relation is indirect
through the original table; neither adjacent dispatcher directly calls the
other. The generated split between `D_800BB2BC` and `D_800BB2C0` is not an
original C object boundary. No declaration-order or register-variable quirk
was established, and this does not merge every handler or distant callee into
one TU.
