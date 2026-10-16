# ovl_17_func_800B83A4 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`794016b3a7a56290fe68a8f0817b5b7483ffaf4c3f6ba36033c34577b26709c3`.

## Membership evidence

The verified body connects existing D_800BD848 record-area/caller-hub
members: 800BAFDC(), six 800BAC24(record, 0x40000) calls over D_800BD870
at 0x50-byte stride, then 800BA808(0x40000). Its gapless successor,
800B8470 (`0x800B83A4 + 0xCC = 0x800B8470`), repeats that ordered
sequence, followed by the same 8002261C / 800226A4 completion check.
Both advance the state byte at D_800BD848+4 when 800226A4 returns 2;
800B8470 addresses that byte through the D_800BD84C alias.

The call graph and link order agree: D_800BB4F8 contains 800B83A4 and
800B8470 in consecutive slots 1 and 2, and 800B8028 dispatches that table
using D_800BD84C. Thus the connection to the existing 800B8470 member is
not merely another reference to overlay-wide data: the pair shares its
render/transition idiom, dispatcher and gapless link adjacency.

This supports adding 800B83A4 to the existing low-confidence cluster.
It does not establish an original translation-unit boundary or justify a
confidence upgrade; callback tables and shared storage can span files.
No additional member is inferred here.

## Evidence

- Verified source: `src/overlays/ovl_17/ovl_17_func_800B83A4.c`.
- Original calls, record walk, state writes and extent:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B83A4/ovl_17_func_800B83A4.s`.
- Gapless successor and repeated render/transition sequence:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B8470/ovl_17_func_800B8470.s`.
- Callback table: `build/ovl_17/asm/data/3634.data.s`, D_800BB4F8.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Link extents and call lists: `build/ovl_17/functions.csv`.
