# ovl_11_func_800FAB18 grouping evidence

Verified-source/evidence identity supplied by the finalization handoff:
`1ea7c1bf3b4def431b9ec009d067f9a883cec2174c14b2c4af041e5de5b6a376`.
The handoff reports byte-exact source and a passed full finalize gate; this
notes-only review did not rerun matching.

## Evidence

- `src/overlays/ovl_11/ovl_11_func_800FAB18.c` directly calls
  `ovl_11_func_800FAC0C`, reads `D_80126FE0`, and clears that word when state
  3 completes. The existing ledger recorded the read and adjacency, but not
  this direct call or the closing-state write.
- Original assembly:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800FAB18/ovl_11_func_800FAB18.s`.
  The call is at `0x800FABE0`, the state-clear store at `0x800FABA0`, and
  the function occupies `0xF4` bytes: `0x800FAB18 + 0xF4 = 0x800FAC0C`.
  The shared-state caller and callee are therefore zero-gap neighbours.
- `src/overlays/ovl_11/ovl_11_func_800FAC0C.c` reads the same state word
  and sets it to 3 on close requests, corroborating the caller's closing
  branch. Its direct callee `ovl_11_func_800FB0C4.c` also gates a write to
  `D_80126FE0` on `func_800226A4() == 2`.

## Interpretation and limits

The direct call plus zero-gap link adjacency and shared-state lifecycle
corroborate candidate membership of `ovl_11_func_800FAB18` in the existing
`D_80126FE0/E4/E8/EC` menu-state cluster. Its role is menu update/draw
dispatch and close completion. Original TU boundaries and ownership of the
state data remain unproven; neither the broadly used globals nor the shared
import call alone establish same-TU membership. No extension to distant
callees is inferred.
