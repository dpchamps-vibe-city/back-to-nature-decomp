# ovl_17_func_800B7F90 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`39dad5096769e138d14951802f1f15535210118de741796488e433a42abb62d2`.

## Membership evidence

The verified body connects existing D_800BD848 record-area/caller-hub
members: 800BA808(0x40000), 800BAFDC(), then six 800BAC24(record, 0x40000)
calls over D_800BD870 at 0x50-byte stride. Original 800B81FC repeats this
ordered rendering sequence, including the constant and record count; this
is more specific corroboration than another reference to overlay-wide data.

The callback tables and link order also agree. D_800BB4E4 begins with
800B7E78, 800B7F90 and 800B8028, dispatched by 800B7E38. Those callbacks
are gaplessly adjacent (`0x800B7E78 + 0x118 = 0x800B7F90`,
`0x800B7F90 + 0x98 = 0x800B8028`). The successor dispatches through
D_800BB4F8, whose first entry is the rendering-sequence twin 800B81FC and
which also contains existing cluster hubs 800B8470 and 800B8898. Thus the
new member's shared record/rendering idiom has corroborating dispatch and
link-order context, rather than relying only on the global address.

Together these observations support adding 800B7F90 to the existing
low-confidence cluster. They do not establish an original translation-unit
boundary; the callback run and broad record-area cluster could span files.
No confidence upgrade or additional member is inferred here.

## Evidence

- Verified source: `src/overlays/ovl_17/ovl_17_func_800B7F90.c`.
- Original calls, record walk and extent:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B7F90/ovl_17_func_800B7F90.s`.
- Rendering-sequence twin:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B81FC/ovl_17_func_800B81FC.s`.
- Callback tables: `build/ovl_17/asm/data/3634.data.s`,
  D_800BB4E4 at 0x800BB4E4 and D_800BB4F8 at 0x800BB4F8.
- Dispatchers: `src/overlays/ovl_17/ovl_17_func_800B7E38.c` and
  `src/overlays/ovl_17/ovl_17_func_800B8028.c`.
- Link extents and call lists: `build/ovl_17/functions.csv`.
