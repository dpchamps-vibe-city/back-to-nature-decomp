# ovl_11_func_80107390: keyed entry point to the guard family

Documentation-only review of the supplied byte-exact, passing-finalize handoff,
identity
`7c8fb6f1a0a9282157b6a9c9447ad51a0b1efa5fa515cb593c758eabef280364`
(origin: agent; changed files: `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_80107390.c`). The reviewed source has SHA-256
`c111e0b775c385385d4c9a4feec9c19fc3e91bbb4971d43769525c746caa7955`.
No solver experiments, build-input edits or new binary verification were
performed in this review.

## New membership evidence

The verified function adds corroborated call/link adjacency and a shared-state
reset to the existing candidate button/state guard family:

- Original assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80107390/ovl_11_func_80107390.s`
  records size 0x88, ending with the return delay slot at 0x80107414.
  Thus 0x80107390 + 0x88 = 0x80107418, the existing candidate run head.
  At 0x801073E4 it directly calls that immediate successor, passing the
  matched list pointer in the delay slot at 0x801073E8. Verified C agrees.
- `configs/splat/ovl_11.yaml` corroborates the order at file offsets
  0x4F570 (`80107390`) and 0x4F5F8 (`80107418`).
  `build/ovl_11/ovl_11.map` places their text at 0x80107390 (size 0x88)
  and 0x80107418 (size 0x68).
- The entry point resets `D_8012D038` to -1 at 0x801073A8 before looking
  up a list. Existing family member `80107480`, reached through `80107418`,
  reads that word at 0x801074AC to gate guard evaluation; `80107888` writes
  its successful selector there at 0x80107B34. This closes an entry-reset /
  evaluator-read / dispatcher-write dependency within the call family,
  rather than merely sharing nearby data addresses. Evidence is the
  verified sources and original assembly under the corresponding
  `src/overlays/ovl_11/` and `build/ovl_11/asm/nonmatchings/` function paths;
  earlier analysis is in [the record-evaluator review](ovl_11_func_80107480-grouping.md).

Assuming corroborated call/link adjacency and this shared state lifecycle
reflect original source organization, these support extending the existing
medium-confidence candidate TU backward to include `ovl_11_func_80107390`.

## Role and limits

The function scans 31 pointers in `D_80127814` for a signed-halfword ID equal
to its argument. On a match it returns 0x1CD if `80107B84` returns 1;
otherwise it passes the matching list to `80107418`. No match returns -1.
The override-header declaration records the pointer-table extent and ID view,
not independent original-TU evidence.

Neither outer TU boundary is established. Absolute access to `D_8012D038`
does not prove ownership, and modern per-function splat splits and map objects
corroborate addresses, not original compilation boundaries. This review does
not merge the preceding `D_8012CF48` rotation-state family, extend the separate
keyed-table group merely because `80107B84` is called, or transfer the nested
`8010780C`/`80107888` pair's stronger confidence to the wider guard run.
