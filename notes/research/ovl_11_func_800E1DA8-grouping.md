# ovl_11_func_800E1DA8 — adjacent handler/helper pair

Verified source/evidence identity supplied by the finalization handoff:
`443d3d890aeb6aa3c8c5f111519f49b6c98d231e931ee562e110cb4018a2e409`.
The handoff reports byte identity and the full finalize gate, with
`include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800E1DA8.c` as the changed files. This
notes-only review did not rerun matching or change build inputs.

## New evidence for existing candidate membership

The ledger already lists E1DA8 among `D_800B966C`'s callbacks. Its verified
body adds a shared flag protocol with its immediately preceding caller:

- `D_800B966C[13]` at `0x800B96A0` points to E1D48, while entry 18 at
  `0x800B96B4` points to E1DA8. E0B40 dispatches this table using the
  object's handler id at `+0x26`, selected by E0C24.
- E1D48 calls E1DA8 when flag `0x2000` at object `+0x34` is set. The
  verified E1DA8 body specifically tests handler id 13, selects
  `800D12B8(obj, 6, 0x8030)` for that path (otherwise `0x8031`), and
  clears the caller's `0x2000` flag when the three words at
  `+0x48/+0x4C/+0x50` are zero. E1D48 then has a completion path when
  that flag is clear. Both bodies also manipulate flag `0x800`.
- The direct caller and callee are gapless in original code order:
  `800E1D48 + 0x60 = 800E1DA8`. Their splat offsets are `0x29F28` and
  `0x29F88`; E1DA8's `0x118` bytes end at the next function, E1EC0.

The newly identified id-13-specific shared update ties the adjacent pair
more closely than table membership alone. Callback membership, direct call,
shared flag protocol and link order support E1DA8 as a medium-confidence
candidate in the existing E0C24 dispatcher/TU run. They do not establish
static linkage or an original TU boundary. The shared reconstruction types,
engine callee and broadly used `D_8006C838` state are not independent
same-TU evidence; no neighbouring run is merged on that basis.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800E1DA8.c`: verified shared update body.
- `src/overlays/ovl_11/ovl_11_func_800E1D48.c`: adjacent flag-gated caller.
- `src/overlays/ovl_11/ovl_11_func_800E0B40.c` and
  `src/overlays/ovl_11/ovl_11_func_800E0C24.c`: table invocation/selection.
- `build/ovl_11/asm/data/1740.rodata.s`: callback entries 13 and 18.
- `configs/splat/ovl_11.yaml`: consecutive code offsets.
- `build/ovl_11/functions/ovl_11_func_800E1D48.s`: original `0x60` extent
  and direct call at `0x800E1D70`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E1DA8/ovl_11_func_800E1DA8.s`:
  original `0x118` extent, id-13 test and flag update.
