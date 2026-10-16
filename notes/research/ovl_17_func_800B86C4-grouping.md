# ovl_17_func_800B86C4 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`c498a822cc0b31a01345437a3caf11a985286b56fbd733ec4795e82f10e6b0bd`.

## Membership evidence

The verified body is a twin of existing cluster member 800B83A4: both
call func_80017A08 for six records, passing D_800556A8 plus each record's
u16@+0x40 in D_800BD848, then call 800BAFDC(), six
800BAC24(record, 0x40000) steps over D_800BD870 at 0x50 stride, and
800BA808(0x40000). Their original 0xCC-byte bodies differ in
only the second argument to func_8002261C (4 here, 3 in 800B83A4).
Both clear s16@D_800BD848+0x2CE and select state 6 when 800226A4 returns 2.

Dispatch and link order corroborate that shared idiom. D_800BB4F8 holds
800B86C4 and 800B8790 in consecutive zero-based slots 5 and 6; 800B8028
indexes this table with D_800BD84C, the D_800BD848+4 state byte written
here. The selected callback is also the gapless successor
(`0x800B86C4 + 0xCC = 0x800B8790`). It repeats the record/render sequence,
increments the counter reset here, and calls existing cluster members
800B9C48 and 800BB050. This is a concrete producer/consumer transition,
not just another reference to overlay-wide storage.

Together these observations support adding 800B86C4 to the existing
low-confidence D_800BD848 record-area/caller-hub cluster. The grouping
remains provisional: shared callbacks and storage can span translation
units, so no original TU boundary, confidence upgrade or additional
member is inferred.

## Evidence

- Verified source: `src/overlays/ovl_17/ovl_17_func_800B86C4.c`.
- Original body: `build/ovl_17/asm/nonmatchings/ovl_17_func_800B86C4/ovl_17_func_800B86C4.s`.
- Existing twin: `src/overlays/ovl_17/ovl_17_func_800B83A4.c` and `build/ovl_17/asm/nonmatchings/ovl_17_func_800B83A4/ovl_17_func_800B83A4.s`.
- Successor and counter consumer: `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8790/ovl_17_func_800B8790.s`.
- Callback table: `build/ovl_17/asm/data/3634.data.s`, D_800BB4F8.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Link extents and call lists: `build/ovl_17/functions.csv`.
