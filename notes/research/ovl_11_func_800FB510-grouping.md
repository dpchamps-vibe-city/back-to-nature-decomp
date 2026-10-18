# ovl_11_func_800FB510 grouping evidence

Verified handoff source/evidence identity:
`767b40fae4d32131b1e5d885180c4eeb4c62649fc304f5ea49f57fcf208cfa59`.

The verified body supports adding this function as a low-confidence candidate
member of the existing `D_80126FE0/E4/E8/EC` menu-state cluster. The ledger
already mentioned its call edge from `ovl_11_func_800FAC0C`; the new evidence
is their shared coordinate table and its relationship to the state cluster.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800FB510.c` reads signed halfwords at
  offsets 0/2 of `D_80126FF0` rows with stride 0x1C. It dispatches the entry
  through `ovl_11_func_800F7010` or `ovl_11_func_800D6F78`, with a conditional
  `ovl_11_func_800F7094` draw. The declaration in
  `include/globals_override.h` records this coordinate view.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800FAC0C/ovl_11_func_800FAC0C.s`
  calls this helper at 0x800FAD10/20/48/5C and directly reads the same
  table's coordinate pair with the same stride at 0x800FAD94–0x800FADC4,
  indexed by `D_80126FEC`. These two functions are the only symbolic users
  of `D_80126FF0` in the overlay source and extracted function assembly.
- `build/ovl_11/asm/data/69960.data.s` places `D_80126FF0` immediately after
  the four state words `D_80126FE0/E4/E8/EC`, without intervening data.
  This corroborates a shared state/table cluster rather than a call edge
  alone.
- `configs/splat/ovl_11.yaml` places this function immediately after the
  existing candidate `ovl_11_func_800FB4B0` (0x800FB4B0 + 0x60 = 0x800FB510).
  That neighbour is also called by `800FAC0C` (0x800FADE8/0x800FAE60) and
  selects the six-byte records that caller supplies to `800FB510`.

Limits: shared data and corroborated call/link order support candidate
membership, not private linkage or defining-TU ownership. The caller and
this helper are not consecutive functions; adjacency to `800FB4B0` and to
state data does not rule out an original object boundary. Retain low
confidence for this extension and do not merge the entire intervening span.
