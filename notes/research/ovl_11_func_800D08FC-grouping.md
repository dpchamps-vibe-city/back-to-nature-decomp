# ovl_11_func_800D08FC: pair-selector association and matching evidence

## Verified result

`src/overlays/ovl_11/ovl_11_func_800D08FC.c` passed
`psx_finalize_function` after integration: exact diff, full configured build,
scope and clean-source gates. The scratch winner is
`build/ovl_11_func_800D08FC/matched-pair.c` (57/57 original words).
Documentation handoff identity:
`4659ea01479cee55acc3addde6e8c5c7458955b74c6ab7bf3ddbaeb5a3f76861`.
No compiler override or source-policy exception was added.

The function returns an `Ovl11DF72CPair *`, the existing two-halfword type
used by matched callers 800DF72C and 800E12D8. Mode 1 searches slots 0–1;
other modes search slots 2–3. A slot is free when its leading signed
halfword is -1. The second slot of each range is disabled by zero at
D_8006C838+0x44D0 / +0x44D2. Failure returns null.

## Grouping evidence

The original 800D079C is 0x160 bytes and ends exactly at 800D08FC.
Its final scan tests the leading halfword of the first two four-byte pairs
through D_80074838+0x19C8, then reads each pair's second halfword through
D_8006C838+0x99C8. Both origins identify D_80076200, the pair storage this
selector searches and returns. Thus the association is a shared specific
record cluster plus zero-gap adjacency, not merely a common state global.

This supports a **low-confidence extension** of the existing tag-setter /
scanner group. It does not make 800D08FC part of that group's closed direct
call cluster and does not prove an original translation-unit boundary.

Evidence paths:
- `src/overlays/ovl_11/ovl_11_func_800D079C.c`
- `src/overlays/ovl_11/ovl_11_func_800D08FC.c`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800D079C/ovl_11_func_800D079C.s`
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800D08FC/ovl_11_func_800D08FC.s`
- `include/game_types.h`: `Ovl11DF72CPair`

## Matching mechanism, separate from grouping

The prior 3,888-candidate exhausted search was conditional on its frozen
for-loop grammar and statement set. It did not establish a clean-C
impossibility for this function.

Measured changes under `build/ovl_11_func_800D08FC/`:
- `compound-header.c`: explicit walked cursors, residual [0,8,1,9].
- `early-offset.c`: materialize `offset = i * 4` before the free-slot
  test, residual [0,0,1,4]. This lets the shift, rather than the cursor
  increment, occupy the branch delay slot.
- `first-last-index.c`: explicitly initialize the first last-slot index
  before capturing the flag base, residual [0,0,0,2].
- `offset-first.c`: offset-first integer address addition removes the
  commutative-operand difference. Only the initial base/cursor copy
  direction still differs (55/57 words; no staged residual).
- `first-cursor-web.c`: separate first-range and second-range cursor
  variables; exact 57/57. `matched-pair.c` replaces the temporary local
  record typedef with the already shared caller type and remains exact.

The copy-direction change is consistent with the measured expansion and
`gcc/cse.c:make_regs_eqv`: CSE prefers an equivalent pseudo with a later
last-use endpoint beyond the current block. Reusing one cursor in both
arms extends that cursor's source-level web; separating the first cursor
allows the base initialization and cursor copy to retain the target's
direction. This is source-web evidence, not a claim that declaration order
alone controls the result, nor evidence for a shared register-variable TU.

The ledger at `build/experimentLedger/ovl_11_func_800D08FC.jsonl` retains the
measurements. These compiler-source and experiment artifacts are diagnostic;
the integrated source and full byte gate establish the result.
