# ovl_21_func_800BAA38 — shared sprite flag toggle

## Provenance

The handoff reports byte-exact source and a passed full finalize gate, with
source/evidence identity
`633ebf4536edbefbcc8b6062d882cf0549f87c59a1f5615e76119d22ca80b273`.
Its changed files are `include/overlays/ovl_21.h` and
`src/overlays/ovl_21/ovl_21_func_800BAA38.c`. This documentation does not rerun
matching or change build inputs.

## New grouping evidence

- The verified source toggles flag 4 on the exact sprite subrecords initialized
  by existing member `src/overlays/ovl_21/ovl_21_func_800BB648.c`: six records
  based at `D_800C0458`, outer stride 0x108, three sprites per record at
  +0x40 with stride 0x4C. This is an initializer/flag-updater relationship over
  identical subobjects, not just use of an overlay-wide global. Existing member
  `src/overlays/ovl_21/ovl_21_func_800BA510.c` also walks this six-record base,
  linking the toggle to the documented `D_800C0448` cluster.
- Its other targets are sixteen 0x30-byte sprites starting at `D_800C0ACC`.
  Existing member `src/overlays/ovl_21/ovl_21_func_800BB328.c` initializes the
  first; 800BB648 initializes the remaining fifteen via `D_800C0B2C - 1`,
  `D_800C0B2C`, and the thirteen successors. Thus the toggle independently
  connects both display-setup initializers to the same shared object cluster.
- `src/overlays/ovl_21/ovl_21_func_800BA9F8.c` and
  `src/overlays/ovl_21/ovl_21_func_800BAA18.c` call the toggle with 1 and 0.
  `configs/splat/ovl_21.yaml` places these two wrappers immediately before it
  at offsets 0x2BD8, 0x2BF8, and 0x2C18. This corroborates a local
  wrapper/helper run; the shared-object relationships above are the tie to
  the existing cluster.

## Membership conclusion and limits

Add only `ovl_21_func_800BAA38` to the existing `ovl_21` D_800C0448 cluster as
its bulk sprite flag-4 toggle. Under that cluster's medium-confidence same-TU
hypothesis, the identical initialized/updated subobjects support membership.
Shared storage and calls can cross TUs; this establishes neither TU boundaries
nor data ownership, and does not add the wrappers as members.
