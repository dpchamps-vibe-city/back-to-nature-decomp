# ovl_11_func_80107418: guard-record list evaluator

Documentation-only review of the supplied byte-exact, passing-finalize handoff,
identity
`26284065cb360b390b63d4a457b642d5d4f71598fb01552852db68f306b24580`
(origin: agent; changed file: `src/overlays/ovl_11/ovl_11_func_80107418.c`).
The reviewed source has SHA-256
`fb299d5a39b543f5dd77b91e92914314fb2abd18b75122f9804cd41a0ff85ed8`.
No solver experiments, build-input edits or new binary verification were
performed in this review.

## New membership evidence

The verified function is the immediate predecessor and direct caller of
`80107480`, the existing candidate button/state guard-family head:

- Original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80107418/ovl_11_func_80107418.s`
  records size 0x68, ending with the return delay slot at 0x8010747C.
  Thus 0x80107418 + 0x68 = 0x80107480, with no intervening code.
- The same assembly calls `80107480` at 0x80107444, passing the current
  record. It advances the record pointer by eight bytes at 0x8010745C
  and stops at the first result other than -1. Verified C agrees; its
  `Ovl11GuardRecord` is the same eight-byte record view used by the callee
  in `src/overlays/ovl_11/ovl_11_func_80107480.c`.
- `configs/splat/ovl_11.yaml` corroborates the order at file offsets
  0x4F5F8 (`80107418`) and 0x4F660 (`80107480`).
  `build/ovl_11/ovl_11.map` places their text at 0x80107418 (size 0x68)
  and 0x80107480 (size 0xA8).

Assuming corroborated call/link adjacency reflects original source
organization, this supports extending the existing medium-confidence
candidate TU backward to include `ovl_11_func_80107418`. It is a list driver
immediately before its record evaluator, not proximity alone. Existing
callee-family evidence is in
[the record-evaluator review](ovl_11_func_80107480-grouping.md).

## Role and limits

The function reads a signed count at argument +2 and a record-array pointer
at +4, returning the first non--1 record result or -1 if none qualifies.
It references no data globals and supplies no new shared-static,
declaration-order or register-variable evidence. The shared reconstructed
record type is not independent proof of original TU membership. Per-function
splat splits and map objects corroborate addresses, not original compilation
boundaries. Neither outer boundary is established; this review does not
extend the group to earlier predecessors or raise confidence for the wider
run.
