# ovl_11_func_800E9724 grouping evidence

The byte-exact, finalized function adds a candidate member to the existing
`D_80129560` accessor family; it does not establish an original TU boundary.
Handoff source/evidence identity:
`ca919f64c43d6be0f991a628fc0515ac5979dfacde5c117f06067af3bae86106`.

Evidence:
- `src/overlays/ovl_11/ovl_11_func_800E9724.c` reads three slots of the
  overlay-private `D_80129560` table and forwards them to the coordinate
  snap/interpolation function `ovl_11_func_800E60A0`. The first slot is read
  as a signed halfword, the other two as words.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E9724/ovl_11_func_800E9724.s`
  confirms the absolute table base, the family's signed-halfword-scaled
  index for arg1, the `lh`/`lw`/`lw` loads, and the call at 0x800E9760.
  Its 0x54-byte extent ends exactly at 0x800E9778.
- The immediate successor, `src/overlays/ovl_11/ovl_11_func_800E9778.c`, is
  already a family member and writes three coordinate slots in the same
  table. Shared table use corroborates the gapless link adjacency.

Limits: neither neighbour calls the other, and an overlay-private global
need not be TU-private. The distant call to 800E60A0 alone does not merge
this accessor into the `D_801295C6` setter/getter group. This is membership
support for the existing candidate family, not proof of a single TU across
its full address span.
