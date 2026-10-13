# ovl_11_func_800EF488: setter/resetter-family grouping evidence

The supplied handoff reports byte-exact source and a passed full finalize gate
(origin: agent; source/evidence identity
`2402fe46298ed2569c864dab94269671a39b6d2faebbd6389fe985136fee920b`).
This is documentation of grouping evidence, not a new matching experiment.

## Evidence

- **Shared record aliases and producer/consumer fields.** Verified
  `src/overlays/ovl_11/ovl_11_func_800EF488.c` copies four unsigned halfwords
  from `D_80070CF0` into `D_80071CC0` for index -1 or one of five 0xC-byte
  `D_8007AD10` records. It consumes the latter record's signed +8/+A indices
  to set bit 3 at work-area +0x9A06 in two 0x1D4-stride records.
  `src/overlays/ovl_11/ovl_11_func_800EF6E4.c` writes those same +8/+A fields
  through the same destination aliases and sets the same flag bit using an
  index from the singleton record. The alias views and field offsets are in
  `include/globals_override.h` (`Ovl11EF488Record`, selection, records and
  flag views). `D_80071CC0` is work-area +0x5488; `D_8007AD10` is +0xE4D8.
  The latter region is also reset by
  `src/overlays/ovl_11/ovl_11_func_800EF870.c`.
- **Gapless link order.** `configs/splat/ovl_11.yaml` places EF488 at
  file offset 0x37668 and EF6E4 at 0x378C4. The target listing
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800EF488/ovl_11_func_800EF488.s`
  gives size 0x25C: 0x800EF488 + 0x25C = 0x800EF6E4. EF6E4's 0x18C bytes
  then end at EF870, which EF6E4 calls. This extends the existing contiguous
  setter/resetter run backwards by one function.
- **Call graph corroboration.** The target listing
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E9D28/ovl_11_func_800E9D28.s`
  calls EF488 at 0x800E9E10 and EF6E4 at 0x800E9EC0 from separate dispatcher
  arms. Thus the shared record operations are also paired by one caller,
  independently of their link adjacency. The caller is evidence of a related
  interface, not a proposed additional member of this family.

## Conclusion and limits

EF488 is a **candidate same-TU member** of the EF6E4/EF870 record
setter/resetter family: shared aliases and complementary record fields now
have both link-order and call-graph corroboration. Its role is record snapshot
and paired-flag updater.

These are globally shared main-RAM records, not demonstrated TU-local statics;
a common dispatcher can call across TU boundaries, and adjacent functions can
straddle one. The original TU boundary therefore remains unproven. In
particular, this does not establish EF488's membership in the D_80127428
shared-state TU: the verified function touches neither D_80127428 nor
D_8012742C. Keep the existing ledger section's broader data-region association
separate from that stronger claim.
