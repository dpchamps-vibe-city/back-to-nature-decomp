# ovl_17_func_800B8790 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`63de7c5320c4d4b0b281d9528779a1ad1b1af961f2e3401827bdc196d82aa91a`.

## Membership evidence

The predecessor/counter relationship with 800B86C4 and the shared
800BAFDC / six 800BAC24 calls / 800BA808 display sequence were already
recorded in [800B86C4's grouping note](ovl_17_func_800B86C4-grouping.md).
Those facts are not new evidence.

New corroboration connects the verified body to successor hub 800B8898.
The original loops at 800B8834–800B884C and 800B8974–800B898C have
identical instructions: for six D_800BD848 records at 0x50 stride, pass
s16@+0x34 to cluster getter 800B9C48 and store its result to both
s16@+0x2C and s16@+0x2E. The successor also calls cluster member
800B9594, which adjusts this same field pair. This is a shared record
initialization idiom, not merely another reference to overlay-wide storage.

There is also a concrete sentinel handoff: 800B8790 writes -1 to
s16@D_800BD848+0x2CC, and 800B8898 tests that field for -1 at
800B8B64–800B8B6C before starting sound 0x43 and clearing it. The verified
body selects state 7 via D_800BD848+4; dispatcher 800B8028 uses that byte
to index D_800BB4F8, whose consecutive slots 6 and 7 hold 800B8790 and
800B8898. Their original extents are gapless
(`0x800B8790 + 0x108 = 0x800B8898`), so the state transition, shared
idiom and link order agree.

Together these observations support adding only 800B8790 to the existing
low-confidence D_800BD848 record-area/caller-hub cluster. This remains
conditional TU evidence: callback order, duplicated loops and shared
storage can cross source-file boundaries. No original TU boundary or
confidence upgrade is inferred.

## Evidence

- Verified body: `src/overlays/ovl_17/ovl_17_func_800B8790.c`.
- Original loop, sentinel write and state transition:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8790/ovl_17_func_800B8790.s`.
- Successor's identical loop, sentinel test and cluster calls:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8898/ovl_17_func_800B8898.s`.
- Getter and field-pair adjustment:
  `src/overlays/ovl_17/ovl_17_func_800B9C48.c` and
  `src/overlays/ovl_17/ovl_17_func_800B9594.c`.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Callback table: `build/ovl_17/asm/data/3634.data.s`, D_800BB4F8.
- Original extents and call lists: `build/ovl_17/functions.csv`.
