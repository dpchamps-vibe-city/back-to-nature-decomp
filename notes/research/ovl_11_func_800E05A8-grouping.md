# ovl_11_func_800E05A8 — adjacent counter/flag handlers

Verified source/evidence identity supplied by the finalization handoff:
`08d28c02d87d7ccbba8aad101198e59d1bc4e3a1db8f7415c77b73c78cd2ef74`.
The handoff reports byte identity and the full finalize gate, naming
`src/overlays/ovl_11/ovl_11_func_800E05A8.c` as its changed file. This
notes-only review did not rerun matching or change build inputs.

## New corroboration for existing candidate membership

The ledger already places E05A8 and E06E0 in the medium-confidence
E109C predicate-caller run. E05A8's body adds a specific counter/flag
relationship with its immediate successor, beyond their known common
predicate call:

- E05A8 conditionally increments the object's `u16@+0xAC`; once it is
  at least 5, with flag `0x1` of `s32@+0x34` clear and `800D9108`
  returning 1, it sets that flag.
- E06E0 tests the same counter against 5 to increment `u16@+0x16`,
  uses the same flag bit to select other updates, and clears both the
  counter and that flag near its end.
- Both reject objects when `800E109C` returns zero or flag `0x400`
  at +0x34 is set.

These complementary update/consume/reset operations agree with gapless
original link order: `0x800E05A8 + 0x138 = 0x800E06E0`. The configured file
offsets are likewise consecutive, `0x28788` and `0x288C0`. Together they
reinforce E05A8's membership in the existing handler-run candidate group;
its one-line role is a per-object counter/flag update handler.

**Limits:** the relationship corroborates the existing medium-confidence
group, not a recovered TU boundary or shared-static ownership. It does
not establish runtime call order between these handlers. Common engine
state and reconstruction types alone are not same-file evidence.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800E05A8.c`: verified counter/flag update.
- `src/overlays/ovl_11/ovl_11_func_800E06E0.c`: neighbour's counter/flag use
  and reset, independently corroborated by its original assembly below.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E05A8/ovl_11_func_800E05A8.s`:
  original +0xAC increment/threshold and flag set at 0x800E0618–0x800E0664,
  shared guards, and `0x138` extent.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E06E0/ovl_11_func_800E06E0.s`:
  flag-selected updates at 0x800E0808–0x800E0844, counter threshold/use
  at 0x800E0848–0x800E0868, and resets at 0x800E0AA8–0x800E0ADC.
- `configs/splat/ovl_11.yaml`: consecutive E05A8/E06E0 link-order entries.
