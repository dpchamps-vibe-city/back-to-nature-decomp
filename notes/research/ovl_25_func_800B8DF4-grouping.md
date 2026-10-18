# ovl_25_func_800B8DF4 grouping evidence

Verified source/evidence identity: `8d9355cf1623e5f95d4013d0863f6c816e45491b9e81868ddd3bcd13731c5703`.
The agent handoff reports byte identity and a passed full finalize gate for
`src/overlays/ovl_25/ovl_25_func_800B8DF4.c`,
`include/globals_override.h`, `include/overlays/ovl_25.h` and
`configs/splat/ovl_25.yaml`. This notes-only pass does not repeat verification.

## Candidate backward extension of the record-array driver run

The verified updater supports adding `800B8DF4` and its immediate caller
`800B8D70` to the existing `D_800BFE54`/`D_800C0124` record-array driver
candidate:

- `build/ovl_25/functions/ovl_25_func_800B8D70.s` decrements the signed
  halfword timer at each record's +4, clamps it to zero, and calls
  `ovl_25_func_800B8DF4` at `0x800B8DC0`. Its six arguments are
  `D_800BFE44 + 0x10 + i*0x78`, exactly the `D_800BFE54` records scanned
  by `src/overlays/ovl_25/ovl_25_func_800B9208.c`. The verified target
  consumes that timer in its per-record state transitions.
- The original listings give a gapless chain:
  `800B8D70 + 0x84 = 800B8DF4` and
  `800B8DF4 + 0x414 = 800B9208`. Thus the new caller directly dispatches
  its immediate link successor, which ends at the existing run head.
  The corresponding listings are under `build/ovl_25/functions/`.
- Beyond sharing the large state base, the updater and its immediate
  successor share a concrete completion-state handoff. `800B9208` sets
  the word at `D_800BFE44 + 8` to 1 on the first qualifying record;
  the updater tests that word for 1 and compares the first
  record's position halfword at `D_800BFE44 + 0x20` against `D_800BCC40`
  before writing -1 to the status halfword at +0x4CC. The original
  `ovl_25_func_800B9208.s` shows the flag store at `0x800B929C`, and
  `ovl_25_func_800B8DF4.s` shows the reads and status store at
  `0x800B91C4`–`0x800B91E8`. Both functions also use `800B9B00` to set
  record state and timer.

This supports candidate membership through call/link-order agreement and a
shared record/completion-state cluster, not a proven original TU boundary.
The state is absolute-addressed shared storage; neither the reconstructed
view nor its new field declarations establish private-static ownership or
original declaration order. The assigned switch-table range in the splat
configuration is build bookkeeping, not independent TU evidence. Keep the
neighbouring dispatch and decision/predicate groups separate: the updater's
calls to `800B9480`/`800B97E4`, or their common use of `D_800BFE44`, do not
by themselves establish a wider merge.
