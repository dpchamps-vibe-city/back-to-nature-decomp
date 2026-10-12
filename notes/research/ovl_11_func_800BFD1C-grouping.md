# ovl_11_func_800BFD1C — local interpolation-state grouping

Verified handoff source/evidence identity:
`745911e47536bae101ea66b799fbebaa3dbbc73174ab254dc68694a3d2b60cd8`.
Source SHA-256: `3d219d70064b1b4177ad3d163bc8502db894650f45032250ad72daa14bdf95da`.
Matching and the full finalize gate were supplied as passed; this is notes-only documentation.

## New membership evidence

- `src/overlays/ovl_11/ovl_11_func_800BFD1C.c` and adjacent
  `ovl_11_func_800BFE3C.c` both select an index with
  `ovl_11_func_800C0834` from the s16 at `D_8006C838+0x44C0`, pass
  `&D_80128A80[index * 3]` to `ovl_11_func_800C0010`, and store that index
  at `D_8006C838+0x8000+0x6768` (`+0xE768`). The new function compares
  the cached index first and requests a 0x100-tick transition; its neighbour
  initializes the same state with a count of 1.
- `configs/splat/ovl_11.yaml` and `build/ovl_11/functions.csv` establish
  zero-gap link adjacency: 0x800BFD1C + 0x120 = 0x800BFE3C. The census
  call lists confirm the same selector and interpolation initializer.
  `build/ovl_11/functions/ovl_11_func_800BFD1C.s` independently shows the
  `lw`/`sw` at work-base +0x6768 and both calls.
- `src/overlays/ovl_11/ovl_11_func_800BFF00.c` copies the selected table's
  first triplet into the nearby +0xE748..+0xE750 vector and clears
  +0xE754..+0xE768, corroborating the existing local state family. The new
  function also calls its nearby per-tick stepper, `ovl_11_func_800BFF74`.

## Scope of inference

Shared specific state, the identical table/selector/initializer chain, and
zero-gap adjacency support suspected same-TU membership with 800BFE3C,
not merely another user of the widely shared D_8006C838 buffer. This does
not prove an original TU boundary or merge the entire scattered buffer
cluster into one TU. The ledger adds only membership and a one-line role
in its existing D_8006C838 flags/state-buffer section.
