# ovl_17_func_800B8294 — record initializer membership evidence

The agent handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`458ba36c247e592c8991528e91b0f07a4385f8e89c0ac70618152fa1b6ff042f`.
This notes-only review does not rerun matching or alter build inputs.

## Membership evidence

The verified initializer writes six 0x50-byte D_800BD848 rows: pointers at
+0x3C, cleared flags at +0x28 and name offsets at +0x40, selected from
D_800BB534. The gapless successor, existing member 800B83A4, consumes those
exact six +0x40 halfwords to draw the names through func_80017A08. This is
a producer/consumer connection, not merely another reference to the
widely shared overlay work area. The initializer also calls existing
cluster member 800B90F8 to initialize the separate queue in that area.

The original caller and link order corroborate the connection:

- 800B81FC calls 800B8294 at 0x800B8244, then selects state 1 or 2 in
  D_800BD848+4 according to the same mode field used by the initializer.
- D_800BB4F8 slots 0, 1 and 2 are 800B81FC, 800B83A4 and 800B8470;
  800B8028 dispatches this table using D_800BD84C (D_800BD848+4).
  Thus the caller selects the subsequent display callbacks after setup.
- The text run is gapless on both sides of the initializer:
  `0x800B81FC + 0x98 = 0x800B8294` and
  `0x800B8294 + 0x110 = 0x800B83A4`.

Together these observations support retaining 800B8294 as a candidate
member of the existing low-confidence D_800BD848 record-area/caller-hub
cluster. If the callback/setup run belongs to one original translation
unit, this initializer plausibly belongs with it. Shared storage, calls
and adjacency do not establish that premise or a TU boundary; no
confidence upgrade or additional member is inferred here.

## Evidence

- Verified source: `src/overlays/ovl_17/ovl_17_func_800B8294.c`.
- Original row stores, callees and extent:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8294/ovl_17_func_800B8294.s`.
- Adjacent caller, direct call and state selection:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B81FC/ovl_17_func_800B81FC.s`.
- Name-field consumer: `src/overlays/ovl_17/ovl_17_func_800B83A4.c` and
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B83A4/ovl_17_func_800B83A4.s`.
- Queue initializer: `src/overlays/ovl_17/ovl_17_func_800B90F8.c`.
- Callback table: `build/ovl_17/asm/data/3634.data.s`, D_800BB4F8.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Original extents/call lists: `build/ovl_17/functions.csv`; configured
  text offsets 0x3DC, 0x474 and 0x584 agree in `configs/splat/ovl_17.yaml`
  (the modern per-function splits are not evidence of original TU splits).
