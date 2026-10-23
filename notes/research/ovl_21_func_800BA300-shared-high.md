# ovl_21_func_800BA300: the shared D_800C0448 HIGH and the unmerged flag address

## Resolution (2026-10-23)

**Matched byte-exact in plain C (112/112, residual `[0,0,0,0]`).** There are
no pins, no asm and no flag override. The source and its doc comment are in
`src/overlays/ovl_21/ovl_21_func_800BA300.c`.

Each step below was measured on the staged residual. The key went from
`[0,1,8,7]` (88/110) to an exact match:

| Step | Source change | Key | Words |
|---|---|---|---|
| start | best ledger source | `[0,1,8,7]` | 88/110 |
| 1 | `raw` assigned inside the outer loop, plus a dead `base = raw;` at the end of the outer body | `[0,1,7,5]` | 91/110 |
| 2 | flag load written `offset + flags` (offset first); store through `record->unk34` as a probe | `[0,3,7,3]` | 94/109 |
| 3 | inner counter reset at the top of the outer body, not at its end and in the prologue | `[0,3,5,2]` | 96/111 |
| 4 | `unk4A` read at each use instead of held in a temporary | `[0,3,1,2]` | 102/111 |
| 5 | store back to `*(offset + flags) = 0`, with `flags = (s32 *)((s32)flags \| zero);` between load and store | `[0,0,1,0]` | 108/111 |
| 6 | inner tail `offset += 0x4C; state += 0x4C; } while (++slot < count)` | `[0,0,0,0]` | 112/112 |

### Mechanisms

1. **`raw`'s LO_SUM needs a long in-loop lifetime.** With `raw` assigned at
   the top of the outer loop, the prologue `lui s6` is the target's, because
   GCSE's PRE insertion precedes every use. But loop.c rated the LO_SUM
   "not desirable": 29 x savings 1 x lifetime 1 = 29 against about 100
   insns. A dead `base = raw;` at the end of the outer body moves
   `REGNO_LAST_UID (raw)` to the end of the loop, so the lifetime spans the
   body and `move_movables` hoists the LO_SUM into s7. Flow deletes the dead
   set later. This is the same `move_movables` desirability lever as in
   ovl_17_func_800B9940, here moved through lifetime rather than insn count.
   Without the binding: 96/112.

2. **The target never merges the flag load's and store's address.** The target
   computes `addu v0,s1,s4` at the load and again at the store. Any source in
   which both read `offset + flags` with `flags` loop-invariant goes wrong
   twice:
   - GCSE's PRE deletes the store's add as fully redundant (`PRE: redundant
     insn … reaching reg …` in the `.gcse` dump).
   - loop.c then combines the two DEST_ADDR givs and strength-reduces them
     into a pointer stepped by 0x4C (`giv at … combined … reduced to`).

   In `combine_givs` a single-use DEST_REG giv cannot be the root of a
   combination. So two unmerged single-use address givs each keep benefit
   `4 - add_cost = 0` and are never reduced, which is the target's shape.
   The fix keeps PRE from merging them without leaving code behind:
   - `zero = 0;` in the prologue, and `flags = (s32 *)((s32)flags | zero);`
     on the path between the load and the store.
   - cse1 does not know `zero` there, because the loop top starts a new
     extended basic block.
   - GCSE's cprop substitutes 0 but leaves `(ior flags 0)`, because
     `validate_replace_rtx_1` simplifies only PLUS and MINUS. It is still a
     set of `flags`, so the expression is killed between the uses and PRE
     keeps the store's add. LCM cannot insert it elsewhere, because the
     store is not anticipated on the other paths.
   - In loop.c `flags` is set inside the loop, so it is neither invariant
     nor a biv, and neither address is a giv.
   - cse2 folds the IOR to `flags = flags` and deletes it.

   Without the IOR: 97/113, the address reduced to a stepped pointer. The
   allocation swap of s4 (`flags`) and s5 (PRE's `group + 1`) resolves with
   this step, because the store's reference raises `flags`'s priority.

3. **The reorg delay-slot copy places the counter reset.** The target's outer
   back-branch goes to 0x800BA338, one word past the prologue's
   `move s2,zero` at 0x800BA334, and the delay slot holds `move s2,zero`.
   That is `fill_slots_from_thread` taking the loop's first insn from the
   branch target. So the reset is the first statement of the outer body.

4. **Re-reading `unk4A`.** Holding `unk4A` in a temporary schedules the unk44
   update ahead of the unk42 one and reverses the add's operands. Reading
   the field at each use, which cse merges into one load, gives the target's
   order.

### Ablations

Each row was measured from the matching source:

| Removed or changed | Words |
|---|---|
| the dead `base = raw;` | 96/112 |
| `raw` hoisted to the prologue by hand | 109/113 |
| record base spelled `D_800C0448` instead of `D_800C0458 - 0x10` | 99/112 |
| the `flags \| zero` kill | 97/113 |
| `+ zero` instead of `\| zero` (cprop simplifies it away before loop.c) | 97/113 |

These variants also match:
- the kill on the offset (`offset \|= zero`);
- XOR instead of IOR;
- the kill before the `if` or inside the cleared branch;
- `zero = 0` in the outer body.

### Closed directions

- **`-fno-gcse`.** The same-cluster neighbour `ovl_21_func_800BA698` has the
  same `raw + 0x64A` guard and `D_800C0448 + 0x34` flag idiom and needs
  `-fno-gcse`. Here the flag makes the outer `group * 0x108` strength-reduce,
  which the target does not. The target's `addiu s5,v1,1 / move v1,s5` outer
  increment is a PRE product, so GCSE was on. flagProbe's "in-place bottom
  increment" fingerprint is a false positive for this function.
- **Block-local copies** of the offset or base before the store. cse1
  substitutes them before GCSE runs, so PRE still merges.
- **Separate base variables joined by loop.c movable matching**
  (`temp_s4` at the loop top, another copy before the store). PRE no longer
  merges, but `simplify_giv_expr` looks through the matched movables, the
  DEST_ADDR givs combine, and the address is reduced.

## Earlier status (2026-10-23)

Before the resolution above, the function was unmatched with best key
`[0,1,8,7]` (88/110). The rest of this note records which compiler state the
target needs and what had been measured towards it, following the method of
`harness-improvement-case-studies/ovl_11_func_80110944-dead-bindings-and-compiler-history.md`.

## What the target shows

All three address sites relocate against `D_800C0448`:

```
800BA30C  lui   s6,%hi(D_800C0448)      prologue: shared HIGH, kept in s6
800BA314  addiu s7,s6,%lo(D_800C0448)   raw (count block), loop-invariant in s7
800BA354  lui   v1,%hi(D_800C0448)      outer body: a separate, local HIGH
800BA358  addiu s3,v1,%lo(D_800C0448)   temp_s3
800BA420  addiu v1,s6,%lo(D_800C0448)   inner body: reuses the prologue HIGH
```

GCSE shares a HIGH only between textually equal expressions. So the original
very likely spelled `raw` and the inner record address the same way, and
`temp_s3` differently, for example `D_800C0458 - 0x10`. The best ledger source
has it the other way round. Its outer and inner sites share the HIGH, through
`move s6,v1`.

## Measured (scratch, 2026-10-23)

- **Swapping the spellings** (`raw = D_800C0448`,
  `temp_s3 = D_800C0458 - 0x10`): the prologue and inner sites now share. But
  GCSE's PRE inserts a fresh `(set R (high X))` at the end of the entry block,
  after `raw`'s LO_SUM. So `raw` keeps the original HIGH, and cse2 turns the
  insertion into a copy: `lui v0; addiu s7,v0; move s6,v0`. Same key.
- **Overwritten bindings of `D_800C0448` before `raw`, in the entry block:**
  no change. cse1 merges them with `raw`'s HIGH within the block.
- **Trailing dead bindings,** after the outer loop or at the end of its body:
  no change.
- **`raw` assigned at the top of the outer loop:** the prologue becomes the
  target's exact `lui s6,0x800c`, because the PRE insertion now precedes every
  use. But `raw`'s LO_SUM stays in the loop. loop.c rates it "not desirable":
  29 x savings 1 x lifetime 1 = 29, against 101 insns. The target has it
  hoisted into s7.
- **Reading the count through the global inside the loop:** cse folds 0x64A
  into the `%lo` (`lh v0,2706(s6)`), but the target keeps a full `raw` in s7.

## Open question

What source state leaves the shared HIGH as the entry block's own `lui`, with
`raw`'s LO_SUM using it in the prologue, while the inner site still recomputes
`%lo` from it? The candidates still open are loop.c making `raw`'s in-loop
LO_SUM desirable (savings x lifetime >= 4), or a block boundary between the
PRE insertion point and `raw`. The remaining schedule terms (the inner-loop
`unk42`/`unk44` update order) are separate from this question.

Scratch sources: the session scratchpad's `scan/ovl_21_func_800BA300/v/`.
They are not tracked; `swap.c` and `inloop_raw.c` are the two informative
ones.
