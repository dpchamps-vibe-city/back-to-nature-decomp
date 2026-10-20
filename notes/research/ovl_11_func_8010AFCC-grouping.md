# ovl_11_func_8010AFCC: adjacent motion-refresh caller

## Verified handoff

The handoff reports byte-exactness and a passed full finalize gate for
`ovl_11_func_8010AFCC`, source/evidence identity
`9feaf7bd1519c069fd28aed9f665f28c37e3bc2084eda9fb0e2a3e8bceb9c7fc`.
The supplied changed-file list names
`src/overlays/ovl_11/ovl_11_func_8010AFCC.c`, reviewed in this pass.
This documentation pass did not rerun matching or change build inputs.

## New grouping evidence

- **Call graph and gapless link order agree.** The original target calls
  `ovl_11_func_8010AE64` at 0x8010B028 after handing the resolved position to
  `ovl_11_func_800D05D0`. Its callee is its immediate predecessor:
  `0x8010AE64 + 0x168 = 0x8010AFCC`. Original evidence:
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8010AFCC/ovl_11_func_8010AFCC.s`
  and `build/ovl_11/asm/nonmatchings/ovl_11_func_8010AE64/ovl_11_func_8010AE64.s`.
  `configs/splat/ovl_11.yaml` corroborates consecutive offsets
  0x53044 / 0x531AC / 0x5323C for AE64 / AFCC / B05C.
- **Complementary object-state handling.** Both functions test flag 0x2000
  at object +0x34 and use `ovl_11_func_800D0408(4, object+0x48, 0)` on a
  flag-clear path (original calls at 0x8010AFAC and 0x8010B040).
  The target's flag-set path resolves request 0x65 through 800F44B4,
  returning 2 on failure; success installs the position, calls AE64 to
  refresh motion state, and returns 0. This connects the caller to the
  existing D_8012D0xx family's documented D_8012D084 reader, rather than
  relying on proximity alone. Reviewed callee source:
  `src/overlays/ovl_11/ovl_11_func_8010AE64.c`.

## Conclusion and limits

Add AFCC as a **low-confidence candidate** extension of the existing
D_8012D0xx family through AE64; do not raise confidence or merge other groups.
The target accesses no globals directly. This is caller/link-adjacency and
shared state-operation evidence, not proof of shared static storage or an
original TU boundary. Modern shared types and prototypes are not ownership
evidence. No declaration-order or register-variable quirk is claimed, and
adjacency alone does not justify adding successor 8010B05C.
