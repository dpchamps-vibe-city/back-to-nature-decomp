# Asm-exception evidence

This file records the evidence behind source-policy exceptions (register
bindings, inline asm, scheduling barriers) that the user has authorized, so a
later session can tell a deliberate, justified exception from a workaround that
should be removed. Each entry says what the exception does, why plain C could
not reach the target under our toolchain, and what would retire it.

## ovl_19_func_800BB108 — `register s32 column asm("$16")` (authorized 2026-10-23)

**Exception.** The table loop's column counter is declared
`register s32 column asm("$16")` in `src/overlays/ovl_19/ovl_19_func_800BB108.c`.
The allowlist entry is `ovl_19_func_800bb108` (`register-asm`, `embedded-asm`) in
`.pi/autoloop.json`. The rest of the body is C.

**What the target needs.** The original's first `column = 0`
(`move $s0,$zero`) sits in the delay slot of `jal func_80017A64`, below the
`jal func_80015EE8` that ends the object's first use of `$s0`. The reset at the
end of each outer pass sits in the outer back-branch's delay slot, with
`move $s1,$s5` left before the branch. reorg's backward fill takes the insn
directly before a branch, so that tail reset must be the last insn of the outer
body. A reset at the top of the outer body loses the slot to `move $s1,$s5`.

**Why plain C does not reach it under our cc1.** The scheduler that runs here
is the old backward list scheduler (`sched.c`, not `haifa-sched.c`).
- It computes priority from predecessors, so a constant write has priority 1.
- The `D_8005E3C0->field_D8` double load gives `jal func_80015EE8`, and every
  later call, priority 3.
- A constant write to a pseudo that already crosses calls has no dependency on
  those calls, so it floats above them. `column` crosses the inner loop's calls,
  so its first reset lands at the top of the block.
- That overlaps the object pointer's lifetime in `$s0`. `column` is pushed to
  `$s1`, and `row`, `column` and `y` rotate (best clean C: `[0,0,1,3]`).
- Writes this scheduler does keep below the preceding call are:
  - sets of pseudos that cross no call (`REG_N_CALLS_CROSSED == 0`);
  - every hard-register set. `sched.c:1176` tests `call_used_regs[i]`, the loop
    index, not `regno + i`, so every hard-register set gets an anti-dependence
    on the last call.

**Measurements** (scratch under `build/bb108/`, ledgered).
- About 460 plain-C variants failed between two sessions. They covered
  statement order, natural `for` loops, variable reuse and the nested-definition
  position. Closed-directions entry: "Where must the initial column reset sit".
- A loop-note barrier (`do { } while (0)`, 308 placements) pins the reset into
  the right delay slot but shifts other allocation. Best `[0,0,4,3]`.
- An empty `__asm__ volatile ("")` barrier gives `[0,6,3,7]` or worse. A full
  barrier is the wrong model: only one write needs holding.
- Binding `column` to `$16` makes that one write a hard-register set. With the
  row reset after the `func_80015EE8` call and `offset + (s32)text`, it gives
  132/132 byte-exact (`build/bb108/asm/q2.c`, cleaned to `build/bb108/final.c`).

**Corpus scan** (`build/bb108/scan/heldWrites.mts`, 2674 functions read from
the original image). It looked for a constant write to a callee-saved register,
below a call, with no reference to that register in between.
- **All blocks:** 151 matched writes in 110 functions. Almost all are in blocks
  where every priority is 1, and there this scheduler keeps source order. They
  are not informative.
- **Blocks where a load feeds the calls (so the scheduler reorders) and the
  write has no consumer in its block:**

  | Function | Status | What keeps the write below the call |
  |---|---|---|
  | `ovl_19_func_800BB108` | unmatched (now pinned) | nothing in plain C |
  | `ovl_11_func_800F7230` | unmatched | not yet examined |
  | `ovl_11_func_801024D8` | matched | false positive: in RTL the loads are in an earlier block |
  | `ovl_15_func_801320A8` | matched | a loop.c constant that crosses no call |
  | `func_8001E4C0` | matched | stacked `do { } while (0)` barriers |

No matched function gets this shape from ordinary C under our toolchain. The
reading taken: the original compiler keeps such a write below the call and ours
does not. This is a narrow scheduling difference, not a register pin in the
original source. The sample is small, so this is a judgement, not a proof.

**What would retire it.**
- If `ovl_11_func_800F7230` turns out to need the same single-write hold, two
  cases of one toolchain difference would justify a small `cc1` scheduling
  change in place of per-function pins. Any such change must keep all matched
  functions byte-exact.
- A plain-C construct that holds that write below the call would replace the
  binding outright.

Related: `notes/file-groupings.md` (`ovl_19` nested-callee pair) and the
precedent `ovl_11_func_800FC8C8`, also a user-authorized callee-saved binding.
