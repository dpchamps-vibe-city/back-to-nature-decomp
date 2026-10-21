# func_80018B98 — clean-C ceiling and hybrid-asm non-transferability

Status: suspended at the user's request on 2026-10-21, not matched. Latest
preserved C attempt: **283/293 compared words**, staged key **`[0,0,1,2]`**.
The assembly stub is active; the complete improved attempt is preserved under
`#if 0` in `src/func_80018B98.c`. No new policy exception was requested or
adopted. Do not read the historical "ceiling" below as an impossibility proof.

## 2026-10-21: loop-note and temporary-web premises reopened

The former best `[0,2,3,9]` was reproduced against the current toolchain before
experimentation. The decisive changes were structural, not pins or flags:

1. The old goto-only source's compiler trace reported **zero source loops**.
   Retaining the same branches but expressing one back-edge with an explicit
   `do ... while` produces `[0,0,1,7]`. Both inner-only and outer-only loop-note
   forms compiled identically. Two explicit loops instead measured
   `[0,0,6,17]`; a nested top-tested form regressed CFG/population. One-loop
   weighting gives the flag `$s5` and scale `$s6` without pinning either.
2. Separating the y product from the masked-token variable improves that
   one-loop seed to `[0,0,1,4]`. This directly overturns the old blanket claim
   that splitting this web is intrinsically destructive. Its earlier failure
   was conditional on the old loop shape and/or pinned variants.
3. An explicit raw-token load, masked-token assignment, then token copy,
   with the raw temporary reused for the x product, reaches **`[0,0,1,2]`**.
   The mask, lasting token copy and remask now use the target's `$a0`, `$a1`
   and `$v1`. The source's generic `work0` is the raw load/x-product web;
   `work1` is the token copy, `work2` the initial mask, `work3` the y product,
   `work4` the nested-text pointer, and `work5` the outer index.

Fresh best measurement: `psx_residual_objective` on source hash
`6315afc200716cbfc2d5f0cb7f85ba606e23d43ed74d84ff9f19891babba33c0`.
The disabled body preserves those bytes as source; its umbrella include is
inside the disabled region as well. Scratch duplicate:
`build/manual/func_80018B98/best-283-of-293.c`.

### What actually remains

- Block 0: one scheduling difference, the incoming arg8 load appears earlier
  than the target. Saved-register roles otherwise agree.
- Block 17 (`0x80018CEC`): two allocation differences. Target loads the raw
  token into `$v0` and births `0xFFFF` in `$v1`; this candidate uses `$v1`
  and `$v0`, respectively. The surviving token copy therefore reads `$v1`
  instead of `$v0`. Other blocks are clear under the staged residual.
- This is **not a match**. The staged zero population is not an independent
  whole-function semantic proof, and the reversal's mach round-trip had a
  reported failure on the earlier seed; do not inflate its attribution.

### Bounded experiments already measured

All **877/877** set partitions of seven sequential temporary roles were
compiled: raw load, lasting token, initial mask, x product, y product,
lookup pointer and outer index. One-loop source shape, existing `-fno-gcse`,
fixed u16 remask and fixed expression/branch sites were held constant.
None was exact. Simplest best partition: `0120345`, i.e. raw/x-product share
one variable, all other roles distinct. Richer partition `0120332` emits the
same words. This closes **only this finite partition grammar**, not other
source types, expression boundaries, declaration births or loop constructions.

Artifacts:
- `build/manual/func_80018B98/make-web-search.ts` and `web-search.json`;
- `build/sourceShapeSearch/func_80018B98/413e6478114b31c7/summary.json`;
- simple best source: `variants/v000004__sequential-web-partitions-partition-0120345/source.c`
  under that search directory;
- `psx_record_closed` has the scope and results, and the experiment ledger
  contains the hand-measured variants.

Other bounded findings:
- Scalar **volatile** arg8 and the former 4-byte BLKmode `TextFlag` produce
  identical words on the one-loop pre-partition seed. Thus prior regression
  of a plain scalar did **not** prove an original struct parameter. The best
  preservation uses volatile scalar for a compatible recursive-call ABI;
  volatility itself is not independently proven. Re-audit that contract.
- A single-set token with fresh x/y products loses three target range masks.
  Reusing token for the lookup pointer restores them but damages call-result
  allocation; using it for the return result emits the old `[0,0,1,4]` program.
  The compiler's `combine.c` `nonzero_bits` / global nonzero-bit union is the
  relevant rule: a multi-set source variable can prevent range-mask removal.
- One read/write empty token barrier was measured under `build/` and regressed
  to `[0,0,2,4]`; no barrier was adopted. No new pin experiments were run.
- `psx_synthesize_source_shapes` generated **zero alternatives**, covering
  11/181 requirements: its contiguous-prologue MVP stops at the first compound
  statement. This is a capability limit, not an exhausted C domain.
- `psx_jump_trace` failed to parse an RTL function section for the result-web
  candidate; it supplied no branch-fold proof.
- The refreshed m2c handoff did not compile (SDK include-order errors) and
  contained inferred signatures, including a two-argument `func_8001A284`
  contradicted by that callee's matched one-argument definition. It was not
  used as the primary source. Current authored declarations were audited;
  `func_8001FABC` has a value/void dispute but its return is unused here.

The next investigation should address the two named remaining sites against
this new seed. Do not rerun the 877 partitions unchanged or seek approval for
full assembly on the strength of the superseded 2026-08-21 "ceiling".

## Historical classification (2026-08-21; superseded where noted above)

Semantics are correct: 294 = 294 instructions against the target, every
premise audited (callee_truth 0 contradicted; matched wrappers; frame map +
4-byte BLKmode `TextFlag` arg8; `-fno-gcse` dominant). The residual is a
register topology, in three coupled pieces:

1. Inner-loop rotation: `tok` should live in `$a1` (via a surviving
   `move a1, tok` copy the target emits at 0x80018CFC), `masked` in `$a0`,
   `remasked` in `$v1`. The candidate emits `$v1` / `$a2` / `$a0`. `.greg`
   shows `tok` already *prefers* `$a1` (preferences 3 7) — local allocation
   state just blocks it.
2. s5/s6 swap: the `flag` web (`D_8005E446-7 < 3`) should be `$s5`; the arg5
   scale web should be `$s6`. Allocator counterfactual gives the exact bound:
   flag (refs 3, live 190, priority 157) must outrank arg5 (refs 4, live 460,
   priority 173) → flag needs refs≥4 or live≤172 — both structurally pinned
   (2 uses at loop depths 1+2; live pinned by emission).
3. Blocks 20/21 (0xFFFF→done): target keeps a shared return-set block
   (`.L80018FF4` `addu v0,s1`) that the flag-branch and the loop-exit both
   enter; the candidate's flag-branch skips it (and on that path even computes
   v0=2 instead of arg1 — harmless, all six callers discard the return).

## Why the spelling family is closed

- Two exhaustive source-space searches (`exhausted-no-exact`) from weaker
  seeds (201/271 and 207/271 class states).
- Most clean-C variations compile to identical words (CSE/scheduler fold
  guard inversions, web splits, store reorders back together) or strictly
  worse (volatile, type changes, inline-flag → pop 376).
- The current-source closure is derived at 2,727,936 candidates / 148 web
  partitions but has NOT been exhaustively evaluated (~1.8h at 23 jobs) —
  this is the one honest remaining clean-C channel.

## The hybrid-window exception was granted, then measured non-transferable

The func_80019070-style exception (`register-asm` / `embedded-asm`) was
granted (2026-08-21). Nine distinct pin/window shapes were compiled and
scored; every one regresses the baseline:

| # | shape | score |
|---|---|---|
| h1 | pin all 5 webs (flag/masked/tok/remasked + scl→s6 via copy) | 229/286 pop 375 |
| h2 | flag→$21 (s5) only | 246/290 pop 375 cfg 1 |
| h3 | masked→$4 (a0) only | 259/291: b17 alloc 4→2, b54 →6 |
| h4 | tok→$5 (a1) only | 252/292 alloc →15 |
| h5 | scale→$22 (s6) via pinned copy | 248/288 |
| h6 | masked→a0 + tok→a1 | 261/291: clears b3/11/17/42, breaks b52/54/56 |
| h7 | all 8 parameter homes | 206/268 pop 16 |
| h8 | mask+tok + split ydelta→$2 | 237/283 pop 95 cfg 2 |
| h9 | masked→a0 + split ydelta (unpinned) | 239/285 pop 95 |

Mechanism why this fails (unlike func_80019070): GCC 2.95 resolves every
fixed-register constraint (`register T v __asm__("$N")`) by global
spill/shuffle. Each pin fixes a local node and cascades into distant blocks
(a pin that clears block 17 moves the damage to the mult tail 52/54/56).
The `masked`→y-delta reuse (one variable for the inner mask AND the y
product) is load-bearing: splitting it destroys the whole func_80019070
call-arg tail (blocks 54–61), consistent with the earlier parked separate
xdelta/ydelta attempt scoring worse. func_80019070's hybrid succeeded
because its divergence was a single isolated prologue window that could be
hand-written; func_80018B98's divergence is the function's whole
register topology inside a nested loop with many calls.

## Next steps (see parking note for the decision menu)

1. Exhaust the current-source closure (148 web partitions — strictly more
   than the 26-partition closure the ancestor seeds exhausted). If it finds
   an exact candidate, the clean model beats every asm option.
2. If empty, full-asm INCLUDE_ASM body is the only guaranteed byte-exact
   route and needs an assembly-stub allowlist entry (a stronger category
   than the window hybrid granted).
3. Otherwise hold clean 257/292.

Do not re-attempt bare register-asm pins without new evidence; the nine
counterexamples are in the experiment ledger (keys v1:h1..v3:h9 under
build/experimentLedger/func_80018B98.jsonl).
