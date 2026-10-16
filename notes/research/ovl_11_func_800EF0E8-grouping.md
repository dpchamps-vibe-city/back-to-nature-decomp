# ovl_11_func_800EF0E8: record-query grouping bridge

The handoff reports byte-exact source and a passed full finalize gate
(origin: agent; source/evidence identity
`7d63da0026a30c625f7a47ef1dba1b513a955005a8a9ab71a4e5b75d1a15168e`).
This is notes-only documentation, not a new matching experiment.

## New grouping evidence

The existing button-state run already records EF0E8's shared state trio and
calls to EF8BC/EFA1C. The additional evidence is its complementary use of the
record cluster in the immediately following snapshot/set/reset family:

- **Same record destinations, queried versus updated.** Verified
  `src/overlays/ovl_11/ovl_11_func_800EF0E8.c` selects a 0xC-byte
  `D_8007AD10` record by index, or the singleton at
  `D_80070CF0 + 0xFD0 = D_80071CC0` for index -1. It compares the record's
  +0/+2/+4 fields with the current state and consumes its signed +8/+A
  indices for 0x1D4-stride status checks. Its immediate successor,
  `src/overlays/ovl_11/ovl_11_func_800EF488.c`, snapshots the current state
  into those same destinations and uses the same two indices for paired
  flag updates. Both also read the work-area +0x5492 selection and use
  the same per-index flag pairs 0x21/0x26 through 0x24/0x29, queried by
  EF0E8 and set by EF488. The record aliases and layouts are declared in
  `include/globals_override.h`; reuse of the modern typedef names alone
  is not grouping evidence.
- **Gapless link bridge.** The original listing
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800EF0E8/ovl_11_func_800EF0E8.s`
  gives size 0x3A0, so EF0E8 ends exactly at 0x800EF488.
  `configs/splat/ovl_11.yaml` places them at file offsets 0x372C8 and
  0x37668, followed by EF6E4 at 0x378C4 and EF870 at 0x37A50.
  [The EF488 note](ovl_11_func_800EF488-grouping.md) already establishes
  the latter snapshot/set/reset family. EF0E8 now supplies a shared-record
  bridge from the preceding button-state run into that family.
- **Local call graph agrees.** EF0E8 calls EF8BC, also called by existing
  run member EEFB8 (`800EEFB8`).
  `src/overlays/ovl_11/ovl_11_func_800EF6E4.c` calls that same EEFB8
  producer with the selected singleton or indexed record, then calls
  EF870 on its singleton path. Thus the existing run and the following
  record family share a record producer as well as consecutive link order.

## Conclusion and limits

EF0E8 is the record-state eligibility classifier and a candidate same-TU
member of the adjacent EF488/EF6E4/EF870 snapshot/set/reset family, connecting
it to the already recorded EEECC/EEFB8/EF0E8 run. This corroborates candidate
membership; it does not establish an original TU boundary or justify merging
the ledger's broader clusters.

These are globally shared main-RAM records, not demonstrated TU-local
statics. Cross-TU calls can use the same layouts and flag interface. No new
GP-relative ownership, declaration-order effect or shared register-variable
quirk is established. In particular, this does not place EF0E8 in the distant
D_80127428 TU merely because the record family is discussed in that ledger
section.
