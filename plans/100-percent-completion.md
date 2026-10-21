# 100% completion: what remains after the unparked run

Written 2026-10-21. Assumes the agent loop matches the 39 functions unparked
after the 2026-10-07 provider credit outage (HTTP 402, 14:52–15:19Z), whose
parks were not attempts.

## Where things stand

- `run_output/autoloop/state.json` holds 56 park records.
- 13 of them are stale: the functions match in clean C and should be removed
  from `parked`:
  `func_8001BBD8`, `ovl_11_func_800BD3C4`, `ovl_11_func_800D0F20`,
  `ovl_11_func_800D806C`, `ovl_11_func_800DAC08`, `ovl_11_func_800F1540`,
  `ovl_11_func_801031E4`, `ovl_11_func_8010C4D4`, `ovl_11_func_80113208`,
  `ovl_11_func_80121204`, `ovl_15_func_8013345C`, `ovl_21_func_800B90C4`,
  `ovl_31_func_800B83B8`.
- That leaves **43 unmatched functions**: the four oldest parks, plus 39 others.

## The four oldest parks

`func_80018B98`, `func_800183E0`, `func_8001E4C0`, `func_8002206C`.

`func_8002206C`'s open residual has the same signature as
`ovl_11_func_8010B05C`, one of the 39 being re-attempted. After a struct copy,
the stack arguments are loaded and stored in a different order around the
call, giving `sw …,20(…)`. If the loop solves `8010B05C`, try its lever on
`8002206C`.

## The other 39, by what blocks them

### 1. A source lever is named but was never properly tried (15)

These are the next targets. Each has a specific rewrite identified, but the
session ended before measuring it, or only measured it once.

**Nested comparator defined inside the parent** (the `800D1CFC` mechanism: a
nested definition leaves `cse_not_expected` set, which splits `%hi`/`%lo`):

| Function | Evidence |
|---|---|
| `ovl_11_func_800D062C` | 27/27 in scratch on 2026-10-08 (`build/d1cfc_claude/nest/s.c`), never integrated |
| `ovl_11_func_800DD4D8` | Nested definition reaches 3 words off; the callee is already matched separately with `CAPTURE_PREV_RET`, so integration needs the parent-owns-child arrangement |
| `ovl_19_func_800BB108` | The static chain is visible; a definition was tried 5 times early and dropped on score alone |

**Typed struct or array views instead of byte-offset arithmetic.** The address
expression's shape at RTL expansion decides the register ties seen as
allocation residuals:

| Function | Evidence |
|---|---|
| `ovl_11_func_800D08FC` | The same `pairs_99C8` slots view `800D0F20` matched with; loop 2 is exact in scratch, loop 1 about 2 instructions off |
| `ovl_11_func_800F82FC` | The target's `(arg1*8-9+arg0)*6` is `slots[arg1-1][arg0-1]` over the 6-byte records added to `globals_override.h` on 2026-10-18 |
| `ovl_11_func_800DB9A4` | 66/67; one `lui %hi(D_8006C838)` is misplaced; `ovl_11_func_800EF870` closed the same shape with a typed view |
| `ovl_17_func_800BA704` | `D_800BD848` 0x50-byte records are written as raw `*(s16*)((u8*)p+off)` casts |
| `ovl_17_func_800B9324` | Same `D_800BD848` base; the table read looks like an `s16 tbl[][6]` index |
| `ovl_17_func_800B9628` | Same base; mixes typed and raw `u8*` views of it |
| `ovl_21_func_800BA300` | The nested array layout (0x108 outer stride, 0x4C inner records) was never typed |
| `ovl_17_func_800B9940` | Raw offsets (`work + 0x6520 + off`); the target's association is `(global + 0x6520) + off` |

**Natural loops instead of m2c machine-form loops:**

| Function | Evidence |
|---|---|
| `ovl_11_func_800EBF18` | `x += 0x10000; i = x >> 16` was kept throughout; an `s16` counter was never tried |
| `ovl_11_func_800FE3A0` | Same accumulator kept through all 50 measurements; every closed record is conditional on it |

**Premises found but not applied:**

| Function | Evidence |
|---|---|
| `ovl_19_func_800B95D4` | `func_80012A34` is the matched `u32 Rand(s32)`; the source still declares `s32` |
| `ovl_23_func_800B954C` | The same allocation-only rotation appears in both near-identical loops. That points to a wrong operation form or web, not an allocator tie |

### 2. Blocked by configuration (1)

| Function | Fix |
|---|---|
| `ovl_15_func_80135C20` | `jtbl_8012E08C` (rodata +0x2A4) has no `.rodata` attribution in `configs/splat/ovl_15.yaml`. The oracle reports those words UNDETERMINED and the stop rule parks the function on its first return. Its programs at 156/159 with an all-zero residual (ledger source `d8455f74c4ca5be7`) may be the answer once the split exists |

### 3. Waiting on a flag decision (5)

Each makes progress only under a per-file `-fno-cse-*` or
`-fno-strength-reduce` override. Some of those overrides are already committed
without allowlist entries. One investigation at translation-unit level, asking
whether these regions really were built with those flags, settles the group.
Otherwise the source shapes are wrong in a way that lets CSE fold what the
target keeps.

| Function | State |
|---|---|
| `ovl_11_func_80113FF0` | `-fno-cse-follow-jumps -fno-cse-skip-blocks` committed and allowlisted; still 55/101 |
| `ovl_25_func_800B95B0` | `-fno-cse-follow-jumps` committed, not allowlisted; parked as "asm-needs-human-approval" although the construct is a flag |
| `ovl_11_func_80110E98` | Needs `-fno-cse-follow-jumps` for its fourth `slti`; one store is still merged away by cross-jumping |
| `ovl_11_func_800F0E00` | `-fno-strength-reduce` committed at `configs/flag_overrides.mk`; still needs a loop that loop.c declines to optimize but that keeps its `LOOP_BEG` note |
| `ovl_11_func_800F1678` | `-fno-cse-skip-blocks` committed with an evidence comment; the guard is still hoisted |

`ovl_11_func_80104EC8` (bucket 4) depends on the same question: its
neighbours `8011AC30`, `8011AA64` and `8011ADF4` are allowlisted for
`-fno-cse-skip-blocks`.

### 4. Large and structurally wrong (3)

| Function | Size | Problem |
|---|---|---|
| `ovl_11_func_800C580C` | 985 words | The m2c shared `goto` tails do not match the original's duplicated call tails. Callee premises are unresolved (`800D622C`'s fourth argument, `800C70CC`'s argument width, `800CD08C`'s arity). Both jump tables lack a `.rodata` split. Got 70 minutes |
| `ovl_11_func_80104EC8` | 1985 words | Progress depends on the `-fno-cse-skip-blocks` question; the jump tables lack a `.rodata` split. Got 39 minutes |
| `ovl_11_func_800F7230` | 350 words | Ended by an abort after the run broke a shared header. Spill choice: the target keeps the loop counter in `s8` while the candidate spills it |

These need their rodata splits added and a run long enough for their size.
There is no compiler mystery yet.

### 5. Small residuals that look like compiler decisions (15)

All are 1–5 terms from exact. "Genuine" is provisional: most were measured
only in the m2c shape. Eleven of these had local-allocation solver verdicts
computed under the oracle's `-G8` bug, now re-marked OPEN (see below).

| Function | Residual |
|---|---|
| `ovl_11_func_801096FC` | Global-alloc priority, 2×2 register transpose |
| `ovl_11_func_800E0E38` | Global-alloc priority, `s0`/`s2` swap (priorities 3589 vs 2439) |
| `ovl_11_func_800D3D60` | Global-alloc priority; matches only with register pins |
| `ovl_11_func_800D3E40` | Global-alloc priority; likely a local-caching premise (fields of a by-value struct) |
| `ovl_11_func_800D7144` | sched2 tie among incoming stack-argument loads, [0,0,2,0] |
| `ovl_11_func_800DACD4` | local-alloc `combine_regs` tie plus sched1 |
| `ovl_11_func_800FE14C` | sched birth order of one `lui`/`move` group; cut off mid-progress |
| `ovl_11_func_80120CC4` | sched1 tie plus a lifetime fact; matches only with operand-tied asm barriers |
| `ovl_23_func_800B8354` | Order in which the loop pass emits preheader setup; two source families each satisfy half |
| `ovl_25_func_800B8A30` | Copy coalescing; best only with a `void` callee declaration that contradicts the matched `s32` callee |
| `ovl_11_func_800D0FB4` | Hard register for a short-lived constant at the second call site |
| `ovl_11_func_80110944` | Short-lived temporaries' registers plus `addu` operand order; 927 measurements without improvement |
| `ovl_11_func_800C2044` | One reused variable carries both the index and the table base, [0,0,0,1] |
| `ovl_11_func_8010B8DC` | GCSE/PRE reload placement at a join |
| `ovl_11_func_800FB674` | Local vs global allocation of three short temporaries; still improving when the run aborted |

## Recommended order

1. Remove the 13 stale park records.
2. Bucket 1 (15 functions): each has a concrete, named rewrite.
3. Bucket 2: add the `.rodata` split for `ovl_15_func_80135C20`.
4. The flag investigation for bucket 3 and `80104EC8`.
5. Bucket 4 with rodata splits and long runs.
6. Bucket 5, first re-checking each against an idiomatic rewrite, then with
   the corrected oracle tools.

Buckets 1–3 are about 21 functions that look tractable without new tooling.

## After 100%: dead code

Dead code is not part of the 100% target. It is a post-100% step.

52 executable functions are still `INCLUDE_ASM` stubs that `build/callGraph.json`
marks `dead`: no `jal` from the executable or any overlay calls them, and no
scanned reference takes their address (`tools/agent/callGraph.ts:246`). They
are probably unused functions that came along with their object files, since
the linker keeps whole objects.

- `nextTarget` in `.pi/extensions/psx-decomp/autoloop/oracles.ts` skips dead
  functions, so the loop never selects them. Working them needs explicit
  targets.
- Their bytes are still in the binary; the stubs supply them.
- 51 are ordinary compiled-C candidates. `__start` is handwritten assembly.
- They total 2,373 instructions: median 30, largest 268. Most are small.
  `func_8001BA18`, for example, copies three words out of `D_80061DE8`.
- With no callers there is no call site to confirm a signature. Infer it from
  the body (argument registers read, `$v0` written) and from file-group
  neighbours in `notes/file-groupings.md`.
- 57 other dead functions already have C. They show these are tractable.

Full list: `__start`, `func_80011DB0`, `func_8001202C`, `func_80012AB0`,
`func_80014748`, `func_80015CA0`, `func_80015D6C`, `func_80017284`,
`func_80017C4C`, `func_80017CF4`, `func_80017EA0`, `func_80017F88`,
`func_8001ACBC`, `func_8001B028`, `func_8001BA18`, `func_8001BA5C`,
`func_8001BAC4`, `func_8001BB28`, `func_8001BD64`, `func_8001BE18`,
`func_8001D678`, `func_8001D86C`, `func_8001D8B8`, `func_8001DBF0`,
`func_8001DCB0`, `func_8001DE4C`, `func_8001E04C`, `func_8001E088`,
`func_8001E26C`, `func_8001E6FC`, `func_8001F084`, `func_8001F100`,
`func_8001F1C0`, `func_8001F35C`, `func_8001F3DC`, `func_8001F42C`,
`func_8001F5B8`, `func_8001F5EC`, `func_800202F8`, `func_8002039C`,
`func_8002047C`, `func_800204E0`, `func_80020570`, `func_8002066C`,
`func_800224F0`, `func_80022528`, `func_800236EC`, `func_80023794`,
`func_800237FC`, `func_80023B5C`, `func_80023C2C`, `func_800243D0`.

## Lessons that apply to every remaining function

- **Rewrite the m2c skeleton before tuning it.** Most parks were wrong
  structural premises, not compiler walls. They came from byte-offset casts,
  hand-written induction variables, goto loops, and declaration-only nested
  functions. 11 of the matched parked functions fell to an idiomatic rewrite.
- **A residual names the pass where a symptom appears, not its cause.**
  Sessions read `lcm.c`, `sched.c` and `local-alloc.c` when the cause was RTL
  shape fixed by the source.
- **Read a closed direction's `conditionalOn` before trusting it.** Most
  closures that blocked a later match were conditioned on exactly the wrong
  premise.
- **Never measure under a flag you haven't committed to.** A mid-session
  override silently conditions every later measurement. `800B90C4`'s exact
  candidate was discarded because it was only ever measured under one.

## Harness defects that affect the remaining work

| Defect | Status |
|---|---|
| The compiler oracle compiled and assembled overlay functions with the executable's flags (`-G8`) | Fixed 2026-10-21 in `tools/agent/compiler-oracle/run.ts`; the oracle tools now refuse a baseline that does not reproduce production. Pre-fix verdicts on 43 overlay functions were re-marked OPEN in `build/experimentLedger/closed/` |
| The loop counts a provider-error turn (402, 429, overload) as a model return, so an outage parks every function | Open (`.pi/extensions/psx-decomp/autoloop/loop.ts`) |
| `tierMinutes` 25 with `returnsPerTier` 1: any turn ending after 25 minutes parks, including aborts and permission questions | Open (`.pi/autoloop.json`) |
| The park preserves the best key under a strict `[cf,pop,sched,alloc]` ordering, often a farther or broken program | Open |
| Ledger rows store only the `.c` file, not header or flag context. A later header edit makes the preserved best uncompilable, and `remeasure` accepts the resulting UNKNOWN | Open |
| Completion records each store a ~400 KB `inputs` map; `state.json` is ~525 MB, close to V8's ~537 MB string limit | Open |
| Matching a function outside the loop does not clear its park record | Open |
