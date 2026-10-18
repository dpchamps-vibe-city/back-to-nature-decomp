# ovl_17_func_800B8F28 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`e932aaac5a9f25200b5c99ecfd164670713f50f6b184ddfbaf4c85e06a1ad7e1`.

## Membership evidence

The [800B8D88 note](ovl_17_func_800B8D88-grouping.md) already records the
state-10 handoff, gapless predecessor adjacency and shared panel/text rendering.
Those observations are corroboration, not new evidence from this function.
The verified body adds a specific producer/consumer connection to existing
cluster member 800B80B0:

- At 800B9060–800B9090, 800B8F28 writes the word at
  `D_8006C838 + 0x8000 + 0x6614` (absolute offset +0xE614). It writes 1
  exactly when s16@+0xE514 is 3 and D_800BDB04 is 2, otherwise 0.
  800B80B0 reads that exact word at 800B8140 and, when it equals 1,
  executes its 800BB394/800BB3F4 call sequence. This connects the summary
  callback's output to completion cleanup, rather than merely sharing the
  overlay-wide D_8006C838 base.
- The other input, `D_800BDB04 == D_800BD848 + 0x2BC`, is the first
  signed-halfword record index used by existing members 800B9CE4 and
  800B986C. Thus the completion gate depends on a specific field of the
  established record-area cluster. This alias corroborates the connection;
  the new extern and reconstructed struct views are not independent TU evidence.

Together with the already recorded state-table/link-order and render-sequence
agreement with 800B8D88, the gate producer/consumer relationship supports adding
only 800B8F28 to the existing low-confidence D_800BD848 record-area/caller-hub
group. These relationships can cross translation units: no original TU boundary,
confidence upgrade or merger with the separate 800BB394/800BB3F4 pair is inferred.

## Evidence

- Verified body: `src/overlays/ovl_17/ovl_17_func_800B8F28.c`.
- Original gate writes and outer-state increment:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8F28/ovl_17_func_800B8F28.s`.
- Gate consumer: `src/overlays/ovl_17/ovl_17_func_800B80B0.c` and
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B80B0/ovl_17_func_800B80B0.s`
  (base construction at 800B8130–800B813C, load at 800B8140).
- Record-index users: `src/overlays/ovl_17/ovl_17_func_800B9CE4.c` and
  `src/overlays/ovl_17/ovl_17_func_800B986C.c`.
- Callback table and record-index label:
  `build/ovl_17/asm/data/3634.data.s`, D_800BB4F8 and D_800BDB04.
- Predecessor transition: `src/overlays/ovl_17/ovl_17_func_800B8D88.c`.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Original extents and call lists: `build/ovl_17/functions.csv`.
