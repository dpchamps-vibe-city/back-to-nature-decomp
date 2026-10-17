# ovl_17_func_800B861C — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`5679a2567dbaf53240eba379eff273f1a31eff90877781289c1a8643be7a4294`.

## Membership evidence

The verified body walks six D_800BD870 records at 0x50-byte stride,
deferring index 2 until after the other five. D_800BD910 is that deferred
record (`D_800BD870 + 2 * 0x50`). Each record goes through existing
cluster member 800BAC24 with argument 0x40000, followed by existing
member 800BA808(0x40000) and the 8002261C / 800226A4 completion check
also used by adjacent cluster member 800B86C4.

The completion store provides a concrete transition link, beyond another
reference to overlay-wide storage: `D_800BD910 - 0xC4 == D_800BD84C`,
the state byte used by 800B8028 to index D_800BB4F8. That table holds
800B861C and 800B86C4 in consecutive zero-based slots 4 and 5. When
800226A4 returns 2, 800B861C calls 800BA704 and writes 5 to that byte,
selecting 800B86C4. The callbacks are also gapless in the original text:
`0x800B861C + 0xA8 = 0x800B86C4`.

The preceding callback 800B8534 independently corroborates the local
idiom: it likewise defers record 2, calls 800BAC24 and 800BA808 with
0x40000, checks completion and writes the state byte through the same
D_800BD910-0xC4 alias. Its state-4 branch selects 800B861C; table slots
3/4 and gapless text placement (`0x800B8534 + 0xE8 = 0x800B861C`)
agree with that transition.

Together the shared record/render idiom, state handoff and corroborated
callback/text order support adding 800B861C to the existing low-confidence
D_800BD848 record-area/caller-hub cluster. This remains provisional:
callbacks and shared storage can span translation units. No original TU
boundary, confidence upgrade or additional member is inferred.

## Evidence

- Verified source: `src/overlays/ovl_17/ovl_17_func_800B861C.c`.
- Original body and byte-store base: `build/ovl_17/functions/ovl_17_func_800B861C.s`.
- Preceding callback and state-4 branch: `build/ovl_17/functions/ovl_17_func_800B8534.s`.
- Existing successor: `src/overlays/ovl_17/ovl_17_func_800B86C4.c`.
- Callback table: `build/ovl_17/asm/data/3634.data.s`, D_800BB4F8.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Original link extents and call lists: `build/ovl_17/functions.csv`.
