# ovl_15_func_80132C1C — grouping evidence

Verified handoff identity:
`45539ac71c1b703a14066d535db181310fef6252459f59b10f196b092eed3967`.
The handoff reports byte identity and a passed full finalize gate for
`src/overlays/ovl_15/ovl_15_func_80132C1C.c`,
`include/globals_override.h` and `include/overlays/ovl_15.h`.
This review changes notes only and does not rerun matching.

## Candidate extension of the existing dual-base slot group

- The verified body stages selected row-1 save records from `D_801376D0`
  into `D_80142550` and writes their addresses to `Ovl15Slots[1][i].unk4`.
  Its immediate link successor,
  `src/overlays/ovl_15/ovl_15_func_801331C4.c`, consumes those same pointers
  to copy the staged records into runtime slots. The alias in
  `include/globals_override.h` identifies `Ovl15Slots` as `D_80140F90`,
  already shared by the dual-base group's members. This is a concrete
  producer/consumer tie through the same table, not merely common copy calls.
- Both functions dispatch types 0 versus 1/2 over the 0xB4/0xB8 record pools
  and call `ovl_15_func_8013345C`, `ovl_15_func_80132A8C` and
  `ovl_15_func_80132B44`. Original target assembly at
  `build/ovl_15/asm/nonmatchings/ovl_15_func_80132C1C/ovl_15_func_80132C1C.s`
  witnesses the table base at 0x80132C38–0x80132C40, row-1 pointer stores at
  0x80132D84/0x80132EB4, setup calls at 0x80132CB8/0x80132DD8, and
  free-slot helper calls at 0x80132F4C/0x80133058.
- `configs/splat/ovl_15.yaml` places `ovl_15_func_80132B44` at 0x4D5C
  (size 0xD8), the target at 0x4E34 (size 0x5A8), and
  `ovl_15_func_801331C4` at 0x53DC. Thus the target immediately follows
  a setup callee and immediately precedes its staged-record consumer.
  Call/data-flow evidence agrees with the gapless link order.
- The target also uses both `D_80140F90` and `D_80142550`, the table/buffer
  pair reset by `src/overlays/ovl_15/ovl_15_func_8013468C.c` in the
  separately recorded adjacent-u16 state group. This corroborates shared
  slot machinery, but does not establish that the two groups are one TU.

Add only the target as a low-confidence candidate member of the existing
`D_800742EC/D_801376D0` dual-base slot group. Membership depends on the
combined shared table, staged-record data flow, helper calls and link
adjacency. These facts do not prove static linkage or exact original TU
boundaries; do not merge other groups or infer shared compiler flags.
