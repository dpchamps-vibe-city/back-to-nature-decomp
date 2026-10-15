# ovl_11_func_8011B840 — candidate local handler pair

## Provenance

Notes-only review of the verified source
`src/overlays/ovl_11/ovl_11_func_8011B840.c` and its immediate successor
`src/overlays/ovl_11/ovl_11_func_8011BAFC.c`. The supplied finalized evidence
identity is `90889b03b260139fc256736d414553e885407a13a31eb1937410bf2505dbe6d9`
(origin: agent); its changed-file list names `configs/splat/ovl_11.yaml`,
`include/sdk_types.h` and the B840 source. No new matching or build-gate claim
is made here.

## Grouping evidence

A **low-confidence candidate same-TU pair** is supported locally:

- Original assembly sizes give `8011B840` (0x2BC) → `8011BAFC` (0x270),
  ending at `8011BD6C`, with no gap between the two handlers.
  `configs/splat/ovl_11.yaml` records the corresponding text offsets
  0x63A20 and 0x63CDC.
- Their six-entry jump tables are consecutive in the same order:
  `jtbl_800BB4E8` (0x18 bytes) → `jtbl_800BB500` (0x18 bytes), at file
  offsets 0x36C8 and 0x36E0. The original handler assemblies identify the
  table references; the config and `build/ovl_11/ovl_11.map` corroborate
  placement. The reconstruction's per-function rodata splits do not establish
  original TU ownership.
- Both sources implement the same six-state interaction template over
  `*arg0 - arg1`: engine-status waits for 2/5, paired engine resets,
  state advancement through `8011D098`/`8011D0B4`, a record selection loop
  using by-value `StructD548` with `8011D9B4`, and completion/cancel returns.
  Their setup and selection operations differ, making them sibling variants
  rather than identical functions.
- The call graph corroborates a paired API: original callers `80114604`,
  `80115308` and `8011C6CC` each call both handlers. Their B840/BAFC call
  addresses are respectively 0x8011470C/0x80114B24,
  0x80115414/0x801157DC and 0x8011C7B8/0x8011C8E0.

Original assembly evidence is under
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for the two handlers
and those three callers. Sizes and call edges also appear in
`build/ovl_11/functions.csv`.

## Limits

The hypothesis is conditional on the combined local text/rodata ordering,
shared template and shared callers reflecting source organization rather
than neighbouring translation units. No shared static ownership or TU-level
compiler quirk is established. `D_8012D52C` is already documented as
container-wide and is not an independent TU fingerprint.

`80117BB4` passes the `D_8012D7B8` descriptor initialized by `80117A40` to
B840 by value (original call at 0x80117CAC). That establishes a client/data
flow, not ownership by B840; it does not justify extending the distant
`D_8012D7A8` group. Likewise the related `80118744` handler remains in its
existing group. Neither outer boundary of the local pair is proven.
