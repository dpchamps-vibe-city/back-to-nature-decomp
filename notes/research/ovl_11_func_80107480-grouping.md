# ovl_11_func_80107480: record evaluator preceding the guard dispatcher

Documentation-only review of the supplied byte-exact, passing-finalize handoff,
identity
`61cd13d51d6461f8e7074b8853ed8ad2fc0ab74d839583f60d16e147b00a04be`
(origin: agent; changed files: `include/sdk_types.h` and
`src/overlays/ovl_11/ovl_11_func_80107480.c`). The reviewed source has SHA-256
`3bab085280fcc0867ff36816a5da89e31a34479db0c29ddd4448a244344d85ba`.
No solver experiments, build-input edits or new binary verification were
performed in this review.

## New membership evidence

The existing candidate button/state guard family began at `80107528`.
The verified record evaluator adds corroborated call/link adjacency and a
shared state dependency:

- `build/ovl_11/asm/nonmatchings/ovl_11_func_80107480/ovl_11_func_80107480.s`
  records size 0xA8, with its final return delay slot at 0x80107524.
  Thus 0x80107480 + 0xA8 = 0x80107528, the existing candidate run head.
  The evaluator calls that immediate successor twice, at 0x801074C4 and
  0x801074D4. Verified C agrees on both calls.
- `configs/splat/ovl_11.yaml` corroborates the order with file offsets
  0x4F660 (`80107480`) and 0x4F708 (`80107528`).
  `build/ovl_11/ovl_11.map` likewise places their text at 0x80107480
  (size 0xA8) and 0x80107528 (size 0x98).
- The evaluator reads `D_8012D038` at 0x801074AC and permits evaluation
  only when it is -1 or equals the record's first u16 payload. A member
  already reached through `80107528`, `80107888`, writes its successful
  selector to that same word at 0x80107B34. This is a functional state
  dependency within the call family, not merely proximity to other
  `D_8012D0xx` addresses. Evidence for the writer is
  `src/overlays/ovl_11/ovl_11_func_80107888.c` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80107888/ovl_11_func_80107888.s`.

Together these support extending the existing candidate run backward to
include `ovl_11_func_80107480`, retaining medium confidence. Earlier evidence
for the dispatcher and its callees is preserved in
[the dispatcher review](ovl_11_func_80107528-grouping.md); stronger evidence
specific to `8010780C`/`80107888` is in
[the nested-pair review](ovl_11_func_80107888-grouping.md).

## Role and inference limits

The evaluator takes an eight-byte record: two u8 selectors, two u16 payloads
and a u16 result. After the shared-state gate, it evaluates both selector/
payload pairs through `80107528`. It returns the record result when the first
guard returns 1 and either the second selector is 0xFF or the second guard
also returns 1; otherwise it returns -1. The second call still occurs for
selector 0xFF.

This is candidate original-TU membership, not proof of either outer boundary.
The modern per-function splat splits and map objects are reconstruction
artifacts, not original compilation units. Shared global access does not by
itself prove common ownership; the absolute load supplies no small-data
ownership evidence. The added `Ovl11GuardRecord` declaration supplies a record
layout, not independent grouping or SDK-idiom evidence. This review does not
extend membership to other predecessors or transfer the nested pair's stronger
confidence to the whole run.
