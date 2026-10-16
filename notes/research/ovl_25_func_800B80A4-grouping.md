# ovl_25_func_800B80A4 grouping evidence

Verified source/evidence identity: `3fe6265986734b42c786462aba5c89fe14c5bf4f22024d22b7b84b8cdf1b052a`.
The supplied handoff reports byte identity and a passed full finalize gate;
this notes-only pass does not repeat that verification.

## Additional support for the existing D_800BCBD8 candidate group

- `src/overlays/ovl_25/ovl_25_func_800B80A4.c` conditionally increments the
  dispatch word at `D_8006C838 + 0x448C` when `func_80013394() == 1` and
  `func_8001FE6C() == 0`. The original listing
  `build/ovl_25/functions/ovl_25_func_800B80A4.s` corroborates the load,
  increment and store at `0x800B8110`–`0x800B811C`. This adds a callback
  writer alongside the recorded `800B7F3C` and `800B81B4`; the dispatcher
  `src/overlays/ovl_25/ovl_25_func_800B7EB4.c` reads that word as its index.
- The target shares the ordered two-probe gate, byte-truncated increment
  and comparison with 2 with the group's terminal callback `800B81F4`.
  Compare `0x800B80CC`–`0x800B8100` in the target's original listing with
  `0x800B8204`–`0x800B822C` in
  `build/ovl_25/functions/ovl_25_func_800B81F4.s`. Both callbacks also call
  `ovl_25_func_800B93E4` and `ovl_25_func_800BBA30`.
- `build/ovl_25/asm/data/3D20.data.s`, at `D_800BCBD8`, places the target
  between `800B7F3C` and `800B813C`. The original function listings agree:
  `800B7F3C + 0x168 = 800B80A4` and `800B80A4 + 0x98 = 800B813C`.
  Thus the shared transition state and probe sequence reinforce the existing
  table/link-order cluster rather than relying on adjacency alone.

This supports retaining the target in the existing medium-confidence
candidate translation unit, not extending or merging groups. The state word
is shared engine storage, not a private static or proof of TU ownership;
the matching C pointer/local spellings do not establish original declarations.
