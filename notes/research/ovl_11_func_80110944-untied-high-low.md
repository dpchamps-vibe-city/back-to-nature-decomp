# ovl_11_func_80110944: the untied HIGH/LOW pair in the large-animal arm

**Status (2026-10-23).** Unmatched and parked. Every other part of the function
can be reached in C. The last two words are the address of `largeAnimals[i]`.
The target builds it with its two halves in different registers. Our cc1
always puts both halves in one register.

Neither we nor the original compiler produce that split for an ordinary
local address. What our compiler needs to produce it is a long-lived HIGH,
normally created by a second reference to the same symbol. No such reference
has been found.

The question still open is whether the original source had a second reference
we haven't reconstructed, or the original compiler differs from ours in one
loop-optimizer detail. The evidence leans slightly towards the source (about
60/40).

## Mismatch signature

Large arm prefix. The stride is `i * 0xB8` in `v0`:

```
target                                ours (every clean spelling)
lui   a0,%hi(D_800749F4)              lui   v1,%hi(D_800749F4)
addiu v1,a0,%lo(D_800749F4)           addiu v1,v1,%lo(D_800749F4)
addu  a1,v0,v1                        addu  a1,v0,v1
lh    a0,0x30(a1)    ; area           lh    a0,0x30(a1)
```

Only the HIGH destination and the LOW source differ. The record pointer `a1`
and the `area` load into `a0` already match under the right spellings (see
below).

## Best measured sources

These scratch sources live under `build/ovl_11_func_80110944/` and are not
tracked.

| Source | Key | Words | Notes |
|---|---|---|---|
| `common-animal-scoped-mask.c` | `[0,0,0,2]` | 229/233 | One `animal` pointer shared by both arms. `flagSet = animal->flags & 4` sits inside five nested `do { } while (0)`. Only the four prefix words above differ. |
| `common-animal.c` | `[0,0,0,4]` | 215/233 | Same, without the nested scopes. The record pointer and the flags copy also swap (`a0`/`a1`). |
| `barriers/record-area-a0-binding.c` | `[0,0,0,1]` | 231/233 | Adds `register s32 recordArea asm("$4")`. The HIGH/LOW tie is still there, so a C register pin cannot reach it. |
| preserved attempt in `src/overlays/ovl_11/ovl_11_func_80110944.c` | `[0,0,0,2]` | 226/233 | The parked baseline. |

The ledger holds about 1,000 measurements across sessions:
`build/experimentLedger/ovl_11_func_80110944.jsonl`.

## Why our cc1 always ties the pair

All references are to the vendored GCC 2.95.2 under
`tools/vendor/gcc/2.95.2/src/gcc`.

- **local-alloc ties them.**
  - Under `-msplit-addresses`, a symbolic address is expanded into a HIGH
    pseudo (`lui`) and a LO_SUM pseudo (`addiu`).
  - When both are local to one block and the HIGH dies in the LO_SUM insn,
    `block_alloc` calls `combine_regs`, which puts them in one quantity
    (`local-alloc.c:1093`, `:1609`). No flag gates this.
  - A pseudo is local when it is used in one block and dies once
    (`local-alloc.c:369`).
- **The register order is numeric.** `REG_ALLOC_ORDER` in `config/mips/mips.h`
  runs v0, v1, a0, … With the stride in `v0`, an untied local HIGH would still
  get `v1`, never `a0`.
- **Reload rebuilds a spilled constant in one register.** The `movsi`
  expander (`mips.md:5200`) uses the destination as the HIGH temporary while
  reload is running. So a hoisted constant that gets no hard register comes
  back as `lui rX / addiu rX,rX`. This is what the `common-animal` sources
  produce: the large base is hoisted out of the loop, spilled, and rebuilt in
  full.

## The state that produces the target

The target fits exactly one compiler state:

1. A **HIGH-only pseudo** carrying `REG_EQUIV (high D_800749F4)` lives across
   the helper call `ovl_11_func_800F44B4`, either because it was hoisted or
   because it is shared.
2. It gets no callee-saved register. All nine are busy across the loop:
   s0 `i`, s1 `area`, s2 `eligible`, s3 `count`, s4 `mode`, s5 `choices`,
   s6 `mode & 1`, s7 `limit`, fp `small`. So it is spilled.
3. Reload rebuilds only the `lui`, just before the LO_SUM. `v0` holds the
   stride and `v1` is the LO_SUM's output, so the first usable register is
   `a0`.
4. The LO_SUM stays in the arm as an ordinary local pseudo in `v1`. `area`
   is loaded into `a0` afterwards, because the rebuilt HIGH is dead by then.

### How our compiler creates a long-lived HIGH, and why neither route applies

- **GCSE sharing between two references.** Matched
  `ovl_11_func_80116980` indexes `D_8012823C[...]` twice: once straight-line,
  once in a later loop.
  - GCSE runs once (`gcse.c:162`, `MAX_PASSES 1`). Its PRE finds the second
    `(high D_8012823C)` redundant and inserts a single shared pseudo near the
    top (pseudo 441: 5 uses across 450 insns, crossing 13 calls,
    `REG_EQUIV (high …)`).
  - Each LO_SUM stays separate, because their HIGH operands were different
    registers when GCSE compared them.
  - Both target sites come out as `lui X / addiu Y,X`.
  - `ovl_11_func_800EC52C` shows the same thing with many references to
    `D_80129560`.
  - 80110944 references `D_800749F4` exactly once in the target.
- **loop.c hoisting the HIGH alone.** `force_movables` (`loop.c:1405`) ties a
  movable LO_SUM to its HIGH whenever the LO_SUM is the HIGH's last use.
  `move_movables` then moves the LO_SUM as soon as the HIGH moves
  (`loop.c:1840`). So a HIGH is never hoisted alone unless something else
  uses it after the LO_SUM, which again means a second reference.
- **A theoretical gap.** `toplev.c` runs `reg_scan` before GCSE (`:3891`) and
  not again before the first loop pass. `force_movables` pairs on
  `REGNO_LAST_UID`, and its own comment says that can be stale
  (`loop.c:1415`). If a later use of the HIGH existed when `reg_scan` ran and
  GCSE then removed it, the LO_SUM would not be forced and the HIGH could move
  alone. No source shape that does this without leaving extra code has been
  found.

## Constraints the rest of the target puts on the source

- **The large arm must mention the bare root `D_8006C838`.** The target has
  `lui t0,%hi(D_8006C838)` in the delay slot of each arm's first branch. That
  is GCSE PRE of `(high D_8006C838)` for the selection tail
  (`addiu a0,t0,%lo(D_8006C838)` before `index0`/`index1`).
  - It only lands in those slots when the large arm itself computes
    `state = Ovl11_SelectionState`.
  - Without it, PRE spreads `lui t0` into other blocks and reuses `t0` for the
    inner-scan root, costing one extra word.
- **`available` is a control case.** `D_800712F8` is also hoisted and
  spilled. The target rebuilds it the ordinary way (`lui v1 / addiu v1,v1`),
  and the following `addu v0,s0,v1` is untied. That matches a pre-loop
  `available = D_800712F8` under our compiler.
  - So the original compiler treats a hoisted full constant exactly as ours
    does.
  - Two similar constants in one function came out differently, which points
    at how the source spelled them.
- **Small arm.** It addresses `smallAnimals[i]` and `smallAvailable[i]`
  through the `D_8006C838` struct view. That is why cse relates them
  (`lbu -0x2FDE(base + i)`) and the small base LO_SUM is global (`a0`).
- **Inner scan.** It walks root + `0x8000` in 0xF8 steps through a root
  register (`li 0x8000; addu`). A constant-folded entries base does not
  reproduce this.

## Corpus evidence

The scan covered all 2,674 functions in every container, for
`lui A,%hi(S); addiu B,A,%lo(S)` with A ≠ B. There are about 1,180 such pairs.

- Almost all are explained by a global LO_SUM (used later, often in an
  s-register) or by multiple references to S.
- Restricted to one reference to S in the function, with both A and B dead
  immediately afterwards, **the only hit is this function's large arm**.

## Measured and rejected (2026-10-23)

Each probe is one edit on the `common-animal.c` family.

| Spelling | Result |
|---|---|
| `(u8 *)&D_800749F4 + i * 0xB8`, with or without `state` | Record pointer `a1` and `area` `a0` match. The HIGH/LOW stays tied. Without the root mention, `t0` placement costs one word (212/234). |
| `&Ovl11_SelectionState->largeAnimals[i]` | Same as the direct spelling. |
| Large base as a loop-top variable, or as a pre-loop variable | Hoisted and rebuilt in full (`lui v1 / addiu v1,v1`), 197/234. |
| Re-indexing the pregnancy field through `&D_800749F4` | GCSE does share the HIGH (it becomes global, `a1`), but the LO_SUM and stride are recomputed in that block (+8 words). |
| Re-indexing the area field | cse folds it within the path, so there is no change. |
| Destructive pointer (`animal = &D_800749F4; animal += i`) | 203/235. |
| Destructive pointer plus pregnancy re-index | 189/241. |
| Reset at the loop top, then advance in the arm | 149–150/237. |
| Reset just before `if (!small)` | 201/235. |

Flags: the other session's probe (`flag-probe-mask.txt`) has baseline flags
best on the best source. Nothing points to a per-file override.

## Assessment

**Source difference, slightly more likely.**
- The original compiler handles `available` exactly as ours does in the same
  function.
- The only route to a long-lived HIGH that we know works in matched code is a
  second reference.
- So the original most likely referenced the large array a second time in a
  way we haven't reconstructed.

**Compiler difference, possible.**
- The pattern is unique in the corpus, so it could also be a rare trigger: a
  constant address in one conditional arm of a loop that contains a call,
  with every callee-saved register busy.
- A `loop.c` that moved only the HIGH here would produce exactly the target.
- Unlike `ovl_19_func_800BB108` (`notes/asm-exception-evidence.md`), the
  difference cannot yet be named precisely enough to test with a compiler
  patch, and no second unmatched function shows the symptom.

## Open leads

1. **A second reference that leaves no code.**
   - Untested candidate: cross-jumping after reload. `jump2` edits code after
     reload (see `func_8001A284-crossjump-equiv-orphan.md`). If the original
     had two textually identical copies of the large-arm code, each
     rebuilding a shared HIGH, `jump2` could keep only one.
   - Another candidate: a reference that survives cse1 and is removed by GCSE
     before the first loop pass, so the stale `REGNO_LAST_UID` stops the
     LO_SUM being forced.
2. **Compiler-patch test.** Find a `loop.c` change that hoists the HIGH
   without the LO_SUM for this shape. Accept it only if natural C then
   matches 80110944 and all matched functions stay byte-exact.
3. **Asm workaround.** This is a user decision. It would need both
   `embedded-asm` and `register-asm` allowlist entries:
   ```c
   register Ovl30AnimalRecordB8 *large asm("$3");
   state = Ovl11_SelectionState;
   __asm__("lui\t$4,%%hi(D_800749F4)\n\t"
           "addiu\t%0,$4,%%lo(D_800749F4)"
           : "=r"(large) : : "$4");
   animal = (Ovl11AnimalSelection *)&large[i];
   ```
   This is untested and has known problems:
   - `state` becomes dead, so the root reference that places `t0` disappears.
   - The asm is opaque to the scheduler, so the stride/HIGH/LO_SUM order may
     differ.
   - The record/flags swap still needs the nested `do { } while (0)` scopes.

   Prototype it in scratch before any approval, and record the evidence
   alongside the 800BB108 entry in `notes/asm-exception-evidence.md`.

The closed direction, with its premise, is recorded with
`tools/agent/closedDirections.ts ovl_11_func_80110944` (2026-10-23).
