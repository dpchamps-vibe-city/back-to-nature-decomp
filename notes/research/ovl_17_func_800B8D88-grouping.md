# ovl_17_func_800B8D88 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`42a9fb78937d3e7a9cee65ab225030dff4c698ec06150373a4271ef529042d8b`.

## Membership evidence

The [800B8C38 note](ovl_17_func_800B8C38-grouping.md) already records
800B8D88's incoming state-9 transition, predecessor adjacency and shared
800B91B8/800BA808 calls. Those observations alone are not new evidence.
The verified body adds two concrete connections:

- On completion, when 800B9940(1) returns nonzero, 800B8D88 stores the
  result of func_8001FABC(0x3E) in s16@D_800BD848+0x2C8
  (800B8EF0–800B8EFC). This is D_800BDB10, the exact signed-halfword
  handle consumed by existing cluster member 800B80B0's gated
  func_8001FAE8 cleanup. Existing members 800B8790 and 800B946C write
  handles to the same field; see the
  [cleanup grouping note](ovl_17_func_800B80B0-grouping.md).
- The callback writes 10 to D_800BD848+4 at 800B8F08. Dispatcher 800B8028
  indexes D_800BB4F8 with that byte; slots 8/9/10 contain
  800B8C38/800B8D88/800B8F28. This state flow agrees with gapless original
  link order: `0x800B8C38 + 0x150 = 0x800B8D88` and
  `0x800B8D88 + 0x1A0 = 0x800B8F28`. The successor repeats the two
  func_80022580 panel geometries, text at D_800BB482 and D_800BB482-0x2E,
  and seven-digit formatting through D_800BD738. Both call 800B9940
  (arguments 1 and 0 respectively) and read D_8006C838+0x5228. This
  shared data/render sequence corroborates the callback/link adjacency,
  rather than relying solely on the overlay-wide state byte.

Together the handle producer/cleanup relationship and corroborated callback
order support adding only 800B8D88 to the existing low-confidence
D_800BD848 record-area/caller-hub group. These relationships can cross
translation units: no TU boundary, confidence upgrade or membership claim
for 800B8F28 or 800B9940 follows. The reconstructed types and new extern
entries are descriptions of the accesses, not independent TU evidence.

## Evidence

- Verified body: `src/overlays/ovl_17/ovl_17_func_800B8D88.c`.
- Original handle/state writes and panel rendering:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8D88/ovl_17_func_800B8D88.s`.
- Handle consumer: `src/overlays/ovl_17/ovl_17_func_800B80B0.c` and
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B80B0/ovl_17_func_800B80B0.s`
  (D_800BDB10 load at 800B80F8).
- Incoming transition: `src/overlays/ovl_17/ovl_17_func_800B8C38.c`.
- Successor rendering:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8F28/ovl_17_func_800B8F28.s`.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Callback table, text and handle labels:
  `build/ovl_17/asm/data/3634.data.s`.
- Original extents and call lists: `build/ovl_17/functions.csv`.
