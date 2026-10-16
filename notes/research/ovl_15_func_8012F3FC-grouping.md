# ovl_15_func_8012F3FC — grouping evidence

Verified handoff identity:
`2af6ee2f38ca41a7d51fa66bda5ae7e77ebf9c4c0f232ed0acdab35efc76257c`.
The handoff reports byte identity and a passed full finalize gate. This review
changes notes only and does not rerun matching.

## New membership in the existing shared-prologue cluster

The verified body supports adding this function to the medium-confidence
`D_80053350/func_8001AC10` cluster at 0x8012ED18–0x80132934:

- `src/overlays/ovl_15/ovl_15_func_8012F3FC.c` opens with the cluster's
  `func_8001AC10` idiom: one `D_8005E3C0->field_D8` load, offsets +0x18/+0x14,
  and `D_80054BBC[0]` added to the updater address, here `D_80051FEA`.
  The original assembly corroborates this at 0x8012F424–0x8012F444 in
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012F3FC/ovl_15_func_8012F3FC.s`.
- Beyond that common draw idiom, it reads and resets `D_80137587` and writes
  `D_80137584`. Its reset-to-zero/state-8 pair is also the successful tail
  of `src/overlays/ovl_15/ovl_15_func_8012F158.c`, already named in the
  cluster's evidence. It also shares the `ovl_15_func_801370B4` switch and
  those two state bytes with the listed `ovl_15_func_80130048` handler
  (`src/overlays/ovl_15/ovl_15_func_80130048.c`), although the branch outcomes
  differ. In the target assembly, the state accesses are at 0x8012F468,
  0x8012F4D0 and 0x8012F4D8, and the input-helper call is at 0x8012F490.
- Link placement is consistent with that membership: the target's 0xF8-byte
  body ends exactly at the already-named `ovl_15_func_8012F4F4`, which has
  the same draw prologue and writes `D_80137584`. See
  `configs/splat/ovl_15.yaml` (offsets 0x1614/0x170C) and
  `build/ovl_15/asm/nonmatchings/ovl_15_func_8012F4F4/ovl_15_func_8012F4F4.s`.

The shared state and draw idiom support candidate same-TU membership;
link adjacency is corroboration, not a caller/callee boundary proof.
Neither the new updater extern nor these absolute global references establish
original static linkage or exact TU boundaries. Retain medium confidence and
do not merge the separate renderer or buffer-pointer clusters merely because
the target calls their helpers or touches their state.
