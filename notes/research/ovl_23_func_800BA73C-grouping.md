# ovl_23_func_800BA73C: paired record-handler grouping evidence

Documentation-only handoff: the function is byte-exact and passed the full
finalize gate. Supplied source/evidence identity:
`d771862899e8b2497d39d6a0c600ddc4a7dfd2d337314f8acdc3d7cc9905df2f`
(origin: agent). No build inputs were edited during this documentation pass.

## New evidence

The ledger already identified `800BA73C` as the handler for the 0x50-stride
array driven alongside `800BAA60`'s 0x44-stride array. The verified body now
supports adding it as a member of that existing candidate group:

- **Shared local state and complementary append paths:** both handlers read
  `s32@D_800BF87C+0x04` and `u16@D_800BF87C+0x14`, suppressing their timed
  append when the former is 1. `800BA73C` gates `800B94D0` at period 30;
  `800BAA60` gates `800B9454` at period 10. The named
  `Ovl23_D_800BF87C_draw` view resolves to `D_800BF87C` in
  `include/globals_override.h`; these are not different state objects.
- **Shared rendering idiom:** each updates the embedded sprite with
  `func_80015840`, projects through `800BB040` and `800BAFFC`, sets sprite
  parameters with `func_80015868`, draws with `func_80015BF0`, then calls
  `func_800248E8`. `800BA73C` additionally has an alternate-frame draw path.
  This shared engine-helper sequence corroborates the local-state evidence;
  it is not independently proof of a TU boundary.
- **Call graph and link order agree:** `configs/splat/ovl_23.yaml` places
  `800BA73C` at file offset 0x291C and `800BAA60` at 0x2C40. The former is
  0x324 bytes and ends exactly at the latter. In the caller's original
  assembly, 0x800B933C and 0x800B9344 call the pair consecutively inside one
  six-iteration loop, advancing the records by 0x50 and 0x44 respectively.
  The calls are interleaved, not two separate six-call runs.

Evidence paths:

- `src/overlays/ovl_23/ovl_23_func_800BA73C.c`
- `src/overlays/ovl_23/ovl_23_func_800BAA60.c`
- `src/overlays/ovl_23/ovl_23_func_800B94D0.c`
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800BA73C/ovl_23_func_800BA73C.s`
  (shared state gate at 0x800BA9D4–0x800BAA34; final instruction 0x800BAA5C)
- `build/ovl_23/asm/nonmatchings/ovl_23_func_800B9310/ovl_23_func_800B9310.s`
  (paired loop at 0x800B9328–0x800B9358)
- `include/game_types.h` (`Ovl23BA73CArg`, `Ovl23BAA60Arg`,
  `Ovl23D87CViewBAA60`), `include/globals_override.h`, and
  `configs/splat/ovl_23.yaml`

## Inference and limits

Add `800BA73C` to the existing record-pair driver candidate group, retaining
medium confidence. Shared overlay-local state, complementary operations,
paired calls and gapless handler adjacency support suspected same-TU
membership. They do not prove original static linkage, an original aggregate
declaration, or exact TU boundaries. The recovered struct views and the new
jump-table split are not independently original-declaration evidence.
