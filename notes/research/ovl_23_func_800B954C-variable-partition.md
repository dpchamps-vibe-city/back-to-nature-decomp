# ovl_23_func_800B954C: the variable partition and global-alloc ordering

## Resolution (2026-10-23)

**Matched byte-exact in plain C (165/165).** There are no pins and no asm,
and the full gate and finalization pass. The source has three parts:
- the separate `position`/`cam`/`delta` partition described below;
- the `frame` recompute fix (`test = roundedCamera * 8;
  test = frame + roundedCamera * 8;`);
- one combine-eliminated term in each loop's if-body:
  `distance = (position | cam) < 0; x = HWD0 / 2 - delta + (distance >> 1);`.

The term is always 0. Flow (life analysis) still sees its uses of `position`
and `cam`, so both are live through the delta and `HWD0 / 2` block. Combine
then proves the term is zero (`nonzero_bits` of a comparison) and deletes it.
global-alloc, though, uses flow's block live-in sets, and nobody recomputes
them after combine.

The stale liveness gives exactly the conflict picture the allocator model
below required: `pos` in t0, cam in a2, delta in a3.

Ablations, each measured:
- without the term: 141/165;
- with a term on `position` only: no match;
- with a fresh `pf` variable as the carrier instead of an existing local:
  149/165, with population differences;
- with `kind` as the carrier: the trailing `position = 0;` is needed again;
- with `distance`, `test` or `roundedCamera` as the carrier: no trailing
  binding is needed.

Two further constructs are needed:
- without the `frame` recompute: 164/165;
- unpinned `register` declarations: no effect.

General lesson: global-alloc's block live-in sets come from flow, which runs
before combine. Any use combine deletes still extends liveness for the
allocator. That is another eliminated-operation lever, alongside the dead
bindings in
`harness-improvement-case-studies/ovl_11_func_80110944-dead-bindings-and-compiler-history.md`.

The rest of this note is the investigation that found the requirement.

**Earlier status (2026-10-23).** Unmatched; the best ledger key was `[0,0,0,4]`
(148/165). This note recorded a structural correction that scores lower but
matches the target's value partition, and the global-allocation question it
left open.

## What the target's registers say about the source

Per-value registers in the first loop (the second loop is the same):

| Value | Target | Best ledger source |
|---|---|---|
| `i * 8`, then the `pos` rounding temporary | a0, a0 | a0, a0 (`kind`) |
| `pos` (`unk5CC[i].unk0`) | **t0** | a2 |
| camera `*(s32 *)(base + 0x10)` | **a2** | a2 |
| `pos/4096 - cam/4096` (delta) | **a3** | a2 |
| shifted `pos`, `abs(delta)`, comparison | a1, a1, a1 | a1 (`distance`) |

The best source reuses `position` for `pos`, the camera and the delta, so all
three share one pseudo and a2. The target gives three registers, so these are
three separate pseudos.

The `addu a0,t0,zero; bgez t0; addiu a0,t0,0xFFF` sequence is GCC's own
signed `/ 4096` expansion: it adds to the original `pos`, not to the copy.
`pos` stays separate from the temporary only if cse keeps `pos` as the
canonical register of their quantity. `make_regs_eqv` (`cse.c`) makes the newer
register canonical when its `REGNO_LAST_UID` comes later in insn order.
Reusing `position` late, as the best source does, satisfies that, but only by
merging the three values.

## Measured (scratch, 2026-10-23)

`w1.c`: separate `camera` and `delta` variables shared by both loops, explicit
`kind = position; if (position < 0) kind = position + 4095;`, and a separate
rounding variable in loop 2 (so `kind`'s last use stays in loop 1). Key
`[0,0,1,8]`, 137/165.
- Every value is in its own register, and `pos` is distinct from its
  temporary.
- The remaining differences are one consistent register shift:
  `pos` a1 → t0, camera v1 → a2, delta a2 → a3. There is also the `frame`
  operand order (`addu v1,s1,v1` against `addu v1,v1,s1`).

Native `/ 4096` spellings get the partition right too, but they shift the
registers further (`div4`/`e2`, about 123/165).

## Later session findings (2026-10-23)

- **The `frame` add operand order is fixed.** The target's
  `addu v1,s1,v1` (`frame + test` into `test`) cannot come from
  `test = frame + test`: `expand_binop` swaps commutative operands whenever
  the target is the second operand (`optabs.c:812`). Writing
  `test = roundedCamera * 8; test = frame + roundedCamera * 8;` expands
  without the swap, and cse then substitutes `test` for the recomputed
  product. Applied to the best ledger source this gives 149/165, key
  `[0,0,0,4]`: only the `pos`, camera and delta registers remain. Declaration
  order has no effect on the operand order.
- **What m2c says.** Its raw output (`build/preparation/.../3e19b393.../0-m2c.stdout`)
  reads the function as `frame = unk6 + ((i % 12) >= 6) * 8`,
  `delta = pos / 4096 - unk10 / 4096`, `if (HWD0 / 2 >= abs(delta))`.
  Written literally that is worse (165 vs 169-173 instructions):
  - the target truncates the `i / 12` quotient, not the remainder, so the
    source is `i - (s16)(i / 12) * 12` or a short quotient;
  - the target does not strength-reduce the `pos` address, which needs the
    multi-set `kind = i * 8` offset before the `if`.
  The best source's skeleton is therefore right.
- **Partition.** In the target each source variable keeps one register in both
  loops: `kind` a0, `distance` a1, camera a2, delta a3, `pos` t0,
  `roundedCamera` v0, `test` v1. With separate camera and delta variables,
  `pos` stays distinct from its rounding temporary only while `pos` is cse's
  canonical register. That needs its last mention in insn order after
  `kind`'s, which a trailing `position = 0;` after loop 2 provides (`k2_tp0`).
- **No clean source reaches t0.** A scan of all 245 ledger sources found `pos`
  in a0 (119), a1 (82), a2 (24), a3 (5), v1 (3) and never t0. Partition,
  division-order, condition and inline-helper sweeps (about 130 more
  variants) also never reach it. In the target, nothing in a1 to a3 is live
  across `pos`'s blocks, and the only lower-priority non-call-crossing
  allocno there is `kind`. So the a1-a3 exclusion must come from preferences
  (`set_preference` through locally allocated operands, `expand_preferences`
  along REG_DEAD links) that this source shape does not create.
- **Pins (scratch only, not authorized).** `register s32 position asm("$8")`
  on the `k2_tp0` and `frame` fix gives 149/165 with `pos`/`kind` exact; the
  camera (v1 vs a2) and delta (a2 vs a3) remain. Pinning the camera removes
  its separate rounding temporary, and pinning the delta changes control flow
  (`abssi2` on a hard reg). So pins are not a simple fallback either.

## Allocator model (2026-10-23, continued)

- **Diagnostic compiler.** The recipe in
  `tools/vendor/old-gcc/gcc-2.95.2-psx.Dockerfile` rebuilds a cc1 that is
  bit-identical to the pinned one (SHA-256 `2aa85925...`). A second build
  adds `fprintf` calls to `global.c` that print, into the `.greg` dump:
  - each allocno's preferences before and after `expand_preferences`;
  - the pruned preferences and `regs_someone_prefers`;
  - find_reg's pass-0 candidate set and final choice;
  - each block's live-in allocnos.
  Its assembly is identical to the pinned cc1's. The build context and
  scripts are in this session's scratchpad (`gccdiag/`).
- **Simulator.** A JavaScript model of find_reg (pass 0 and pass 1,
  `regs_used_so_far`, the copy- and preference overrides), fed by that dump.
  It agrees with cc1 on all 17 allocnos of the best candidate.
- **What the target needs**, from searches over conflict edges, hard
  conflicts, preferences and priority order on the fully scheduled candidate
  (`[0,0,0,6]`, separate `pos`/camera/delta, the `frame` fix, trailing
  `position = 0; cam = 0;`):
  - The camera reaches a2 only if it is allocated after `kind` and also loses
    v1, through a conflict with `test` or a v1 preference.
  - The delta reaches a3 only if a2 is excluded for it.
  - `pos` reaches t0 only if a1 to a3 are all excluded.
  No combination of four or fewer changes gives all three. Reordering alone
  never does. The one coherent single story: `pos` and the camera still live
  into bb6 (past the delta and the `HWD0` load) at global-alloc time, which
  also lowers the camera below `distance`. The model then reproduces the
  target exactly.
- **Why no source reaches it.**
  - The target never reads t0 or a2 after bb3 and bb5.
  - Real liveness into bb6 would require uses that vanish after
    allocation. None of reload, jump2 or reorg deletes anything in this
    function.
  - Liveness past bb7 would make `pos` call-crossing (an s-register).
  - In the target no a1 to a3 values sit in `pos`'s blocks.
  - The REG_DEAD links that could carry call-argument preferences are all
    blocked by conflicts from cross-loop variable reuse.
  - Block-scoped per-loop variables, inline helpers, `-g`, every cc1 flag,
    other PSX compilers, and a 400-variant randomized source sweep (variable
    roles, rounding form, camera placement, comparison form, dead bindings,
    redundant reload) never put `pos` above a2.
- **Pins, scratch only and not authorized.** With the camera rounding
  spelled explicitly (`roundedCamera = cam; if (cam < 0) roundedCamera =
  cam + 4095; delta = distance - (roundedCamera >> 12);`) plus
  `register s32 position asm("$8")` and `register s32 cam asm("$6")`, the
  result is 157/165, key `[0,0,0,2]`; only the delta (a2 vs a3) remains.
  Pinning the delta as well pulls the shift result and `distance` into its
  register and changes the delay-slot fill. Scratch: `b954c/pin/rc_poscam.c`,
  `b954c/pin/sx2_poscam.c`.

## Open question

In the target, `pos` avoids v0 to a3 although it really conflicts only with v0,
v1 and its a0 temporary. The camera likewise avoids v1. Under find_reg (global.c
around line 925), that is pass 0 skipping registers in `regs_someone_prefers`,
the preferences of lower-priority allocnos that conflict with it. Note that
`regs_used_so_far` counts every call-clobbered register as already used. So the
question is which lower-priority, non-call-crossing allocnos conflict with `pos`
and the camera and prefer a1 to a3 and v1.

Read `w1`'s `.greg` "preferences" lines against the target before writing
more spellings. Scratch sources are in this session's scratchpad (`b954c/v/`).
