# ovl_11_func_800E0280 — adjacent handler/helper pair

Verified source/evidence identity supplied by the finalization handoff:
`c7f3157d78ca0474ee9f75a73aef2a379ac478a56342cdd4f657287c1e1fafc6`.
The handoff reports byte identity and the full finalize gate, with
`src/overlays/ovl_11/ovl_11_func_800E0280.c` as the changed file. This
notes-only review did not rerun matching or change build inputs.

## New grouping evidence

The verified body adds a shared flag protocol with its immediately preceding
caller, beyond callback-table membership alone:

- `D_800B95CC[13]` at `0x800B9600` names 800E0220, and entry 18 at
  `0x800B9614` names 800E0280. The per-tick driver 800DEF30 invokes this
  table using the object's handler id at `+0x26`, selected by 800DF010.
- 800E0220 calls 800E0280 when flag `0x2000` at object `+0x34` is set.
  The verified callee specifically tests handler id 13, selects
  `800D12B8`'s final argument `0x8030` for that path (otherwise `0x8031`),
  and clears that same flag when the three words at `+0x48/+0x4C/+0x50`
  are zero. On a later invocation, the caller's flag-clear path sets the
  `+0x2C` completion timer to `0x12C`. Both bodies also manipulate flag
  `0x800` at `+0x34`.
- Original call order agrees with gapless link order: 800E0220's direct
  call is at `0x800E0248`, and `0x800E0220 + 0x60 = 0x800E0280`.
  Splat offsets are `0x28400` and `0x28460`; 800E0280 occupies `0x120`
  bytes, ending at the next function, 800E03A0.

The id-13-specific update, direct call, table membership and link adjacency
support medium-confidence candidate membership in the existing
`D_800B957C`/`D_800B95CC` dispatcher group, with the strongest association to
800E0220. They do not establish static linkage or an original TU boundary,
nor place every callback in one TU. Shared reconstruction types, the engine
callee and the broadly used `D_8006C838` state are not independent same-TU
evidence. The similar distant 800E1DA8 body does not justify merging the two
dispatcher groups.

## Evidence paths

- `src/overlays/ovl_11/ovl_11_func_800E0280.c`: verified id-13-specific update.
- `src/overlays/ovl_11/ovl_11_func_800E0220.c`: adjacent flag-gated caller.
- `src/overlays/ovl_11/ovl_11_func_800DEF30.c` and
  `src/overlays/ovl_11/ovl_11_func_800DF010.c`: table invocation/selection.
- `build/ovl_11/asm/data/1740.rodata.s`: callback entries 13 and 18.
- `configs/splat/ovl_11.yaml`: consecutive code offsets.
- `build/ovl_11/functions/ovl_11_func_800E0220.s`: original `0x60` extent,
  flag test, direct call and timer store.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800E0280/ovl_11_func_800E0280.s`:
  original `0x120` extent, id-13 test and flag update.
