# ovl_17_func_800B8C38 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`bdf665fef89b02d424497c12ddbe3071de60faee0f28a046f50132691df31074`.

## Membership evidence

The existing low-confidence D_800BD848 record-area/caller-hub group
already connects predecessor 800B8898 to its record helpers and renderers.
Another reference to this overlay-wide work area would not be new evidence.
The verified callback adds a concrete handle-consumer and state-transition
link to that predecessor:

- At 800B8B74–800B8B80, 800B8898 calls func_8001FABC(0x43), stores
  its result in s16@D_800BD848+0x2CA and clears the +0x2CC sentinel.
  It subsequently passes that handle to func_800207E4. On completion,
  800B8C38 loads exactly that signed halfword at 800B8D4C and passes it
  to func_8001FAE8. This is a producer/consumer relationship over a
  specific field, not just shared storage.
- The predecessor selects state 8 at 800B8BF8–800B8C00 after cluster
  member 800B986C returns 1. Dispatcher 800B8028 indexes D_800BB4F8
  with D_800BD84C, the D_800BD848+4 byte. Consecutive zero-based table
  slots 7/8/9 contain 800B8898, 800B8C38 and 800B8D88. The verified
  callback writes 9 to that byte after the handle-consumer call.
- State order agrees with gapless original link order:
  `0x800B8898 + 0x3A0 = 0x800B8C38` and
  `0x800B8C38 + 0x150 = 0x800B8D88`. All three callbacks call
  800B91B8 and existing cluster renderer 800BA808; the verified callback
  uses the predecessor's terminal backdrop value, 0x800000.

The verified body also reads the six s16 indices at work-area +0x2BC,
looks up each indexed 0x50-stride record's u16@+0x40 text offset and
renders the row whose record index is 2 last. The +0x2BC lookup is also
used by 800B8898 and existing cluster members, corroborating the shared
record view without independently proving TU membership.

Together the handle handoff, callback transition and matching link order
support adding only 800B8C38 to the existing low-confidence cluster.
Callbacks and shared storage can span translation units: no original TU
boundary, confidence upgrade or additional member is inferred.

## Evidence

- Verified body: `src/overlays/ovl_17/ovl_17_func_800B8C38.c`.
- Original text and handle consumer:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8C38/ovl_17_func_800B8C38.s`.
- Handle producer, terminal backdrop value and state-8 transition:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8898/ovl_17_func_800B8898.s`.
- Successor's shared render calls:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8D88/ovl_17_func_800B8D88.s`.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Callback table and state-byte label:
  `build/ovl_17/asm/data/3634.data.s`, D_800BB4F8 / D_800BD84C.
- Original extents and call lists: `build/ovl_17/functions.csv`.
- Existing predecessor connections:
  [800B8790 grouping note](ovl_17_func_800B8790-grouping.md) and
  [800B92B0 grouping note](ovl_17_func_800B92B0-grouping.md).
