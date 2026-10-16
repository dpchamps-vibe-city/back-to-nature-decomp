# ovl_17_func_800B80B0 — grouping corroboration

The documentation handoff reports byte identity and a passed full finalize gate.
Verified source/evidence identity:
`c174f853cf95ec4a00b99faff2adea00e73bbba4b80acf4f6883159ba712b349`.

## Membership evidence

The verified body supplies a concrete connection to the existing D_800BD848
record-area/caller-hub cluster: `D_800BDB10 == D_800BD848 + 0x2C8`.
800BA54C initializes this halfword to -1; existing members 800B8790 and
800B946C store results from func_8001FABC there. 800B80B0 loads the same
signed halfword, tests it against -1 and, when active, passes it to
func_8001FAE8. This connects completion cleanup to the cluster's handle
writers and initializer, rather than merely to overlay-wide storage.

There is independent callback/idiom corroboration. Like existing member
800B7F90, 800B80B0 calls 800BA808, then tests func_80013394() == 1 and
func_8001FE6C() == 0 using a byte-wrapped completion count compared to 2.
Both update the outer state word at D_8006C838+0x448C. D_800BB4E4 lists
800B7F90, 800B8028, 800B8078 and 800B80B0 in consecutive slots 1–4,
matching their gapless link order. Immediate predecessor 800B8078 calls
func_80013328(10) and func_8001FE34(10), then increments that same state
word; 800B80B0 waits for both completion probes and finally writes 0xFF. The renderer and
completion idiom thus have corroborating dispatch, state-flow and
link-order context.

Together these observations support adding only 800B80B0 to the existing
low-confidence cluster. Shared handles, callback tables and repeated idioms
can span source files: no original TU boundary, confidence upgrade or
merger with the separately recorded 800BB394/800BB3F4 pair is inferred.

## Evidence

- Verified body: `src/overlays/ovl_17/ovl_17_func_800B80B0.c`.
- Original handle load, completion checks and state write:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800B80B0/ovl_17_func_800B80B0.s`.
- Handle initializer:
  `build/ovl_17/asm/nonmatchings/ovl_17_func_800BA54C/ovl_17_func_800BA54C.s`
  (store at 0x800BA5F0).
- Handle writers: `src/overlays/ovl_17/ovl_17_func_800B8790.c` and
  `src/overlays/ovl_17/ovl_17_func_800B946C.c`.
- Completion-check sibling and immediate predecessor:
  `src/overlays/ovl_17/ovl_17_func_800B7F90.c` and
  `src/overlays/ovl_17/ovl_17_func_800B8078.c`.
- Callback table and handle address:
  `build/ovl_17/asm/data/3634.data.s`, D_800BB4E4 and D_800BDB10.
- Dispatcher: `src/overlays/ovl_17/ovl_17_func_800B7E38.c`.
- Original extents and call lists: `build/ovl_17/functions.csv`.
