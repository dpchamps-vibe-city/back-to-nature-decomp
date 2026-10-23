# Case study: 800B954C, an allocator model, and stale liveness

**Function:** `ovl_23_func_800B954C` (`ovl_23`, 0x294 bytes, 165 words).

**Study date:** 2026-10-23.

**Outcome:** byte-exact plain C, 165/165. `make check-all` passed (13
overlays and the EXE), and so did the function finalization gate (exact diff,
full build, scope, clean source). No assembly, register pin, compiler patch or
flag override is in the integrated source.

**Scope:** what the problem was, how it was solved, and what the harness
should learn from it. The fix is described in the source comment and in
`notes/research/ovl_23_func_800B954C-variable-partition.md`. This document is
about method.

## Executive summary

The function had been parked after 218 ledgered measurements, with a best key
of `[0,0,0,4]` (148/165). In that best source, control flow, population and
schedule all matched; three values sat in the wrong registers.

The breakthrough was not a better spelling. It was:

1. **Instrumenting the real compiler.** A diagnostic `cc1` was rebuilt from the
   pinned recipe and is bit-identical to the project's. It prints global-alloc's
   internal decision state: preferences before and after merging,
   `regs_someone_prefers`, find_reg's pass-0 candidate set and choice, and
   each block's live-in allocnos.
2. **Modelling the deciding pass exactly.** A small simulator of `global.c`
   find_reg, fed by that dump, reproduced cc1's choice for all 17 allocnos.
3. **Searching the compiler's inputs, not the source.** Systematic changes to
   the conflict graph, preferences and priority order turned "registers are
   wrong" into a precise requirement: `pos` and `cam` must still be live
   across the delta and `HWD0 / 2` computation at global-alloc time. That is
   true nowhere in the target's emitted code.
4. **Enumerating the pass pipeline** for a mechanism that creates allocation-time
   liveness without emitting code. global-alloc reads block live-in sets
   computed by flow, which runs before combine, and nothing recomputes them. A
   use that combine deletes therefore still counts. A provably-zero term in
   `x` that mentions `position` and `cam` reproduced the target exactly.

About 1,000 source variants had been tried before step 1, including a
400-variant randomized sweep and a 301-variant role sweep. None of them put
`pos` above a2. After step 3, the solution took about 15 targeted compiles.

**The lesson for the harness:** when a residual is purely allocation, and
spelling sweeps stop moving the deciding allocno, stop sampling source. Read
the deciding pass's actual inputs, model it, derive the requirement, then look
for a mechanism that meets it. The harness should make each of those steps a
tool.

## 1. The problem as it stood

### 1.1 Target shape

Two near-identical loops (12 entries at `D_800BF87C+0x5CC`, 54 at `+0x418`).
Each loop:

- loads a record's s32 position (`pos`);
- computes a sprite frame from the record and the column (`i % 12 >= 6`);
- computes `delta = pos/4096 - camera/4096`, using GCC's branchy signed
  power-of-two division for both;
- skips entries with `abs(delta) > HWD0 / 2`;
- otherwise calls three drawing functions with `x = HWD0 / 2 - delta`.

### 1.2 The residual

In the best candidate, every instruction was right except:

| Value | Target | Best candidate |
|---|---|---|
| `pos` (loaded record position) | **t0** | a2 |
| camera `*(s32 *)(base + 0x10)` | **a2** | a2 |
| `delta` | **a3** | a2 |

The best source reused one variable, `position`, for all three values.

### 1.3 Why it resisted

t0 is an unusual register choice: it means find_reg skipped v0 to a3. In the
target, nothing in a1 to a3 is live while `pos` is live. Ordinary
reasoning about the final code therefore predicts a1 at worst. Every earlier
session (and this one, for a long while) varied spellings and got a0, a1 or a2.

## 2. What happened, in order

The numbers below are measured. Scratch artifacts are listed in §7.

### 2.1 Structural corrections (useful, but not the answer)

1. **The variable partition.** The target keeps one register per source
   variable in both loops: `kind` a0, `distance` a1, camera a2, delta a3, `pos`
   t0, `roundedCamera` v0, `test` v1. So `pos`, the camera and the delta are
   three variables. Splitting them first made things worse, because `pos`
   coalesced with its rounding temporary.
2. **cse's canonical register.** `make_regs_eqv` in `cse.c` makes the newer
   register of a copy canonical when its `REGNO_LAST_UID` comes later in insn
   order. That register order is fixed by `reg_scan` on the unoptimized RTL.
   So a variable stays canonical, and is not coalesced into the division
   temporary, only if it is mentioned later than the temporary. A trailing
   dead `position = 0;` after loop 2 achieved that; this is the case-study
   lever from 80110944. Result: the structurally correct `k2_tp0`, `[0,0,1,7]`.
3. **The `frame` operand order.** The target's `addu v1,s1,v1` is
   `frame + test` into `test`. `expand_binop` (`optabs.c:812`) swaps
   commutative operands whenever the target is the second operand, so
   `test = frame + test` can never produce it. Writing `test = rc * 8;` then
   `test = frame + rc * 8;` expands without the swap, and cse substitutes
   `test` back in. The schedule key then reached 0 (`tc1`, `[0,0,0,6]`,
   141/165).
4. **The camera in loop 2** also needed a later mention to stay canonical (a
   trailing `cam = 0;`). The final source no longer needs either trailing
   binding (§2.6).

### 2.2 Dead ends (all measured)

| Probe | Result |
|---|---|
| m2c's raw output (`i % 12`, a single delta expression) written literally | 169–173 instructions. The target truncates the `i/12` quotient, not the remainder, and its `pos` address is not strength-reduced, so the m2c form is wrong. |
| Sibling idioms (800B8C18 `diff = a/4096 - b/4096; abs(diff)`) | Partition right, registers shifted. |
| Ledger census: `pos`'s register across all 245 earlier ledger sources | a0 119, a1 82, a2 24, a3 5, v1 3, never t0. |
| Partition, division-order and comparison sweep (90) | Camera reaches a2 only by moving its load into bb2, which is wrong code. |
| Randomized sweep (400): variable roles, rounding form, camera placement, comparison form, dead bindings, redundant reload | `pos`: a0 220, a1 123, a2 54, a3 3 (a3 only where `pos` and the delta share a variable). |
| Role-assignment sweep (301): which variables carry the `i*8` and field roles in each loop | Best 141/165; `pos` never above a2. |
| Block-scoped per-loop variables (15) | `pos` a0. |
| Inline helper (`worldToPixel`) (14) | `pos` a0; the helper moves the camera load. |
| `-g`, the full cc1 flag matrix, plain `register` | No effect, or worse. |
| Other vendored PSX compilers (2.95.1, 2.91.66, 2.8.x, 2.7.2) | Different code; the toolchain note confirms 2.95.2. |
| Register pins (scratch only) | `pos` alone: 149/165. `pos` + camera with explicit rounding: 157/165. Pinning the delta gives control-flow changes, through local-alloc ties onto the hard register. Pins are not a clean fallback. |

The sweeps were not wasted. They ruled out the space of "different spelling
of the same RTL shape", which is what made the model-based step credible.

### 2.3 The instrumented compiler

- **Build.** `tools/vendor/old-gcc/gcc-2.95.2-psx.Dockerfile` reproduced the
  pinned cc1 bit for bit (SHA-256 `2aa85925...`). Copying the vendored
  `src/` alone failed: top-level configure support files are excluded from
  the vendored tree, so the recipe's upstream tarball and patches were used.
- **Diagnostic patch.** A patch to `global.c` adds only `fprintf` calls into
  the `.greg` dump:
  - each allocno's hard-reg, copy and full preferences before and after
    `expand_preferences`, plus its hard conflicts and calls crossed;
  - the pruned full preferences and `regs_someone_prefers`;
  - per find_reg call: `used1`, the pass-0 used set, the preference sets, the
    scan result and the final choice;
  - per block: the allocnos live at its start.
- **Validation.** Its assembly for the candidate equalled the pinned cc1's,
  ignoring `-dp` annotations.

### 2.4 The model and what it said

`galloc.mjs` implements find_reg's pass 0 and pass 1, starting from
`regs_used_so_far` (call-used registers count as already used). It adds the
preference overrides and the conflict propagation done after each assignment.
It matched cc1 on 17 of 17 allocnos for `tc1`.

Searches over modifications of that state (`gsearch*.mjs`, `hsearch.mjs`,
`permsearch.mjs`):

- **All three targets at once:** 0 solutions with up to four changes (conflict
  edges, `regs_someone_prefers` additions, priority moves), and 0 of 362,880
  priority permutations of the nine relevant allocnos. Reordering alone can
  never do it.
- **Per target:**
  - The camera reaches a2 only if it is allocated after `kind` and loses v1.
  - The delta reaches a3 only if a2 is excluded for it.
  - `pos` reaches t0 only if a1 to a3 are all excluded.
- **Conflict-edge search:** 64 solutions, minimal set: `pos` conflicting with
  `distance`, the camera and the delta; the camera conflicting with the delta
  and `test`; the camera ranked below `distance`.
- **Coherent hypothesis** (`hyp.mjs`): `pos` and the camera live into the
  delta / `HWD0` block. That alone reproduces the target exactly, provided the
  camera's longer life puts it below `distance`.

The requirement is precise and surprising. Two values must be live where the
target never reads them, and must not cross the calls.

### 2.5 From requirement to mechanism

Each pass between flow and global-alloc, and after it, was checked against
the vendored source and our own dumps:

| Pass | Can it extend allocation-time liveness without code? | Evidence |
|---|---|---|
| reload, jump2 (`delete_noop_moves`), reorg | It would need deleted uses; none are deleted in this function | Instruction-set diff of lreg→greg and jump2→dbr |
| `update_equiv_regs` init motion | No: needs `depth == 0` and updates flow info | `local-alloc.c` 897–990 |
| regmove `optimize_reg_copy_*` | Within a block only | `regmove.c:580` |
| `reg_may_share` | Partial zero-extend movables only | `loop.c:1866` |
| Clobbers in mult/abs patterns | HI/LO only | `mips.md` |
| **combine** | **Yes.** It deletes uses, and global-alloc reads flow's `global_live_at_start`, which nothing recomputes | `global.c` `global_conflicts`; DIAG block live-in output |

The probe sequence that proved it:

1. `distance = abs(delta) + ((position < 0) >> 1)` had no effect: **tree
   fold** removed it before RTL. The `.rtl` dump had no use.
2. Routing the flag through a variable (`pf = position < 0; ... (pf >> 1)`)
   kept it until flow: four uses at flow, zero after combine. The DIAG
   live-in sets then showed `pos` and the camera live into the delta block.
   But combine's `distribute_notes` moved their REG_DEAD to the top of that
   block, so they still missed the delta.
3. Placing the phantom term in the if-body (`x = HWD0/2 - delta + ...`),
   after the needed range and before the calls, gave **165/165**.

### 2.6 Minimization and ablation

| Variant | Result |
|---|---|
| Final (carrier `distance`, no trailing bindings, unused declarations removed) | 165/165 |
| Without the zero term | 141/165 |
| Zero term on `position` only | no match |
| A fresh `pf` variable as carrier | 149/165 (population changes) |
| Carrier `kind` | needs the trailing `position = 0;` again (cse canonicality) |
| Carrier `distance`, `test` or `roundedCamera` | 165/165 without trailing bindings |
| Without the `frame` recompute | 164/165 |
| Phantom placed before the comparison (in the delta block) | REG_DEAD moved to the block top; no match |

## 3. Methodology that worked

1. **Check the residual's axis first.** Population and schedule were 0, so
   every remaining word was a register-choice question. That licenses
   modelling the allocator rather than the scheduler or the expander.
2. **Make the compiler explain itself.** Dumps show outcomes; the decisive
   quantities (`regs_someone_prefers`, the pass-0 set, live-in sets at
   global time) are internal. A diagnostic rebuild from the pinned recipe
   costs one Docker build and is verifiable (identical assembly).
3. **Validate the model before trusting it.** 17 of 17 agreement on a real
   candidate. Without that, every search result would have been suspect.
4. **Search the decision's inputs to get a requirement, not a spelling.** The
   search space is small (allocnos × registers × edges), fast (milliseconds
   per evaluation), and its answer is a statement about RTL, not text.
5. **Then search the pipeline for mechanisms, not the source for spellings.**
   The requirement ("live with no use") immediately ruled out every
   real-use explanation and pointed to passes that delete uses after liveness
   is computed.
6. **Verify each mechanism step on its own dump.** The tree fold, the flow
   uses, the combine deletion, the DIAG live-in sets and the REG_DEAD
   placement were each confirmed separately before the final placement.
7. **Ablate after the match.** Every construct in the final source is shown to
   be load-bearing, and the minimal carrier choice removed two other
   constructs.
8. **Score is not distance.** The productive path ran through candidates
   scoring 123–141. The best-scoring source (148) merged three variables,
   which is structurally wrong.

### Mistakes worth designing out

- **An overwritten dump invalidated searches.** A `-da` run into the same
  scratch directory overwrote the diagnostic `.greg`, and four model searches
  silently read empty preference data and undefined actual assignments. Only
  a nonsensical result ("assigned cam=2") exposed it.
- **Probe scripts resolved relative paths after `cd`** and wrote `.diff` and
  `.trace` files into the repository root (found by the finalizer's scope
  gate).
- **Shell-quoted comment text** with an apostrophe and backticks broke a
  `node -e` write, and bash executed the backticked fragments. That was
  harmless here, but it is a hazard.
- **Concurrent compiles share `build/compilerTrace/<fn>`**, so background
  sweeps and foreground traces can collide. This session avoided it by
  waiting.

## 4. Mechanism catalogue: eliminated-operation levers

This session added two entries to the class identified in the 80110944 study.

| Lever | Pass that sees the operation | Pass that removes it | Effect | Case |
|---|---|---|---|---|
| Dead binding to an existing local | cse, gcse, loop (it survives `delete_trivially_dead_insns`, which uses function-wide use counts) | flow | HIGH sharing, address history | 80110944 |
| Dead sets padding a loop | loop's second pass `insn_count` | flow | Keeps an invariant local (`threshold × savings × lifetime < insn_count`) | 800B9940 |
| Later mention of a variable | `reg_scan`'s `REGNO_LAST_UID` (before cse1) | cse or flow | Keeps the variable canonical over a copy temporary | 800B954C |
| Recomputed operand | expand (no operand swap) | cse | Fixes commutative operand order | 800B954C |
| **Combine-eliminated use** | **flow's live-in sets** | **combine** | **Extends allocation-time conflicts without code** | **800B954C** |

Each lever has a phase window (the pass that must still see the operation,
and the pass that must remove it) and a placement rule. They belong in one
reference sheet.

## 5. Harness proposals

### P0: a diagnostic compiler as a first-class tool

- Check in the `global.c` (and later `local-alloc.c`) diagnostic patch and a
  build target that reproduces it from the pinned recipe. Check in no
  binaries.
- Verify on build that the unpatched cc1 matches the pinned SHA-256, and that
  the diagnostic cc1's assembly is identical.
- Expose a `compilerTrace --diag` mode that writes the DIAG dump into its own
  run directory.

### P0: allocator model and requirement search

- `tools/agent/analyzeAllocatorCounterfactual.ts` exists, but it read the live
  stub instead of a candidate and needed prepared inputs. Replace or extend it:
  `--src <candidate>`, run the diagnostic compiler, simulate, and report agreement.
- Requirement search: given target hard registers for named roles, report the
  minimal changes to conflicts, preferences or order, and the single coherent
  hypothesis if there is one ("these values must be live in these blocks").
- Map pseudos to source variables automatically (declaration order to pseudo
  numbers, or `.lreg` "user var" lines), so reports name variables.

### P1: a mechanism finder for "live without a use"

When the requirement is extra liveness with no visible use, report the
candidate levers in pipeline order, with their phase windows and placement
rules (§4). For combine-stale liveness: an existing-local carrier, routed
through a variable to defeat tree fold, placed after the range that needs
the conflicts and before any call.

### P1: requirement-based fitness for sweeps

Final registers and word scores are flat across the whole spelling space here.
A fitness of "required conflict edges present / required live-in present",
read from the diagnostic dump, would have flagged the right family of sources.
It would also have stopped the random sweeps early ("none of 400 variants adds
a `pos`–delta conflict").

### P1: a declarative variant generator

This session hand-wrote five generators: partition, randomized knobs, roles,
block scope, phantom placement. A shared tool taking a base source, named
anchors and knob tables (exact-occurrence replacements, as the variant lab's
transform spec already requires) would make these reproducible and
ledgered. Today they are scratch-only.

### P1: a register-role census over the ledger

"Which register did value X get in every earlier source?" was a one-off script
(245 sources, about 12 minutes). It answered a key question: nobody had ever
produced t0. It should be a ledger query.

### P2: artifact isolation and probe hygiene

- Per-run output directories for every compiler invocation. Refuse to
  overwrite a diagnostic dump with a non-diagnostic one.
- Probe helpers should resolve paths before changing directory, and never
  write inside the repository root.
- A run lock or unique directory for `build/compilerTrace/<fn>`, so
  background sweeps cannot collide with foreground traces.

### P2: residual-triage routing

When the key is `[0,0,0,n]` and spelling sweeps stop moving the deciding
allocno's register, triage should route to the allocator-model workflow
instead of recommending more spellings. The `STALLED` message the parked
handoff carried already says to "solve for the compiler state"; the tools to do
so did not exist for global-alloc.

## 6. Honest limitations

- The zero term `(distance >> 1)` is a construction that reproduces the
  original's compiler state. It is not a claim about the original source.
  The original probably had some other use that combine eliminated; it was
  not identified.
- The model omits caller-saves, `losers` from reload retries and multi-word
  allocnos. None were active here; others may need them.
- The levers' generality is untested beyond the three functions above.
- The diagnostic `global.c` patch and the simulator are reproduced in
  Appendices A and B. The source generators and searches exist only in the
  scratchpad (§7) until §5's proposals are implemented.

## 7. Artifacts (scratch, not tracked)

All paths are under this session's scratchpad (`/tmp/claude-1000/...`).

| Path | Contents |
|---|---|
| `gccdiag/Dockerfile`, `global.diag.c`, `out*/cc1.{orig,diag}` | Diagnostic compiler build |
| `gccdiag/galloc.mjs` | find_reg simulator |
| `gccdiag/{gsearch,gsearch2,gsearch3,hsearch,permsearch,hyp,req,links}.mjs` | Requirement searches, hypothesis test, preference-link listing |
| `gccdiag/diag.sh`, `show.sh` | Run the diagnostic compiler on a candidate; print the allocation table |
| `b954c/{sweep,biggen,rolegen,scopegen,natgen,helper}.mjs` | Source-variant generators |
| `b954c/big.txt`, `roles.txt`, `ledger/scan.txt` | Sweep and census results |
| `b954c/stale/`, `b954c/abl/` | Phantom-use probes and ablations; `abl/final1.c` is the integrated source |

The integrated source, with an explanatory comment, is
`src/overlays/ovl_23/ovl_23_func_800B954C.c`. The function-level note is
`notes/research/ovl_23_func_800B954C-variable-partition.md`.

## Appendix A. Diagnostic patch to `global.c`

Apply to the `gcc/global.c` of the tree the 2.95.2-psx recipe builds. The
diagnostic cc1 is built by copying the patched file over `gcc/global.c` after
the unpatched cc1 is made, deleting `gcc/global.o`, and re-running `make cc1`.
The patch adds `fprintf` calls into the `.greg` dump (`-dg`); code generation
is unchanged.

```diff
--- a/gcc/global.c
+++ b/gcc/global.c
@@ -260,6 +260,11 @@
 static void expand_preferences	PROTO((void));
 static void prune_preferences	PROTO((void));
 static void find_reg		PROTO((int, HARD_REG_SET, int, int, int));
+static FILE *diag_file;
+static void diag_set (name, s) const char *name; HARD_REG_SET s; {
+  int r; if (!diag_file) return; fprintf (diag_file, " %s{", name);
+  for (r = 0; r < FIRST_PSEUDO_REGISTER; r++) if (TEST_HARD_REG_BIT (s, r)) fprintf (diag_file, "%d,", r);
+  fprintf (diag_file, "}"); }
 static void record_one_conflict PROTO((int));
 static void record_conflicts	PROTO((int *, int));
 static void mark_reg_store	PROTO((rtx, rtx));
@@ -285,6 +290,7 @@
      FILE *file;
 {
   int retval;
+  diag_file = file;
 #ifdef ELIMINABLE_REGS
   static struct {int from, to; } eliminables[] = ELIMINABLE_REGS;
 #endif
@@ -519,7 +525,10 @@
 
       /* Try to expand the preferences by merging them between allocnos.  */
 
+      diag_file = file;
+      if (file) { int a; for (a = 0; a < max_allocno; a++) { fprintf (file, ";; DIAG pre-expand r%d", allocno_reg[a]); diag_set ("pref", hard_reg_preferences[a]); diag_set ("copy", hard_reg_copy_preferences[a]); diag_set ("full", hard_reg_full_preferences[a]); diag_set ("hconf", hard_reg_conflicts[a]); fprintf (file, " calls=%d\n", allocno_calls_crossed[a]); } }
       expand_preferences ();
+      if (file) { int a; for (a = 0; a < max_allocno; a++) { fprintf (file, ";; DIAG post-expand r%d", allocno_reg[a]); diag_set ("pref", hard_reg_preferences[a]); diag_set ("copy", hard_reg_copy_preferences[a]); diag_set ("full", hard_reg_full_preferences[a]); fprintf (file, "\n"); } }
 
       /* Determine the order to allocate the remaining pseudo registers.  */
 
@@ -548,6 +557,7 @@
 
       if (file)
 	dump_conflicts (file);
+      if (file) { int a; for (a = 0; a < max_allocno; a++) { fprintf (file, ";; DIAG pruned r%d", allocno_reg[a]); diag_set ("full", hard_reg_full_preferences[a]); diag_set ("someone", regs_someone_prefers[a]); fprintf (file, "\n"); } }
 
       /* Try allocating them, one by one, in that order,
 	 except for parameters marked with reg_live_length[regno] == -2.  */
@@ -669,6 +679,7 @@
 	   allocno now live, and with each hard reg now live.  */
 
 	record_conflicts (block_start_allocnos, ax);
+	if (diag_file) { int q; fprintf (diag_file, ";; DIAG bbstart %d:", b); for (q = 0; q < ax; q++) fprintf (diag_file, " r%d", allocno_reg[block_start_allocnos[q]]); fprintf (diag_file, "\n"); }
 
 #ifdef STACK_REGS
 	{
@@ -974,6 +985,7 @@
   IOR_HARD_REG_SET (used, regs_someone_prefers[allocno]);
   
   best_reg = -1;
+  if (diag_file) { fprintf (diag_file, ";; DIAG find r%d alt=%d ccl=%d retry=%d", allocno_reg[allocno], alt_regs_p, accept_call_clobbered, retrying); diag_set ("used1", used1); diag_set ("pass0used", used); diag_set ("someone", regs_someone_prefers[allocno]); diag_set ("copypref", hard_reg_copy_preferences[allocno]); diag_set ("pref", hard_reg_preferences[allocno]); }
   for (i = FIRST_PSEUDO_REGISTER, pass = 0;
        pass <= 1 && i >= FIRST_PSEUDO_REGISTER;
        pass++)
@@ -1011,6 +1023,7 @@
 	  }
       }
 
+  if (diag_file) fprintf (diag_file, " scan_best=%d pass_end=%d", best_reg, pass);
   /* See if there is a preferred register with the same class as the register
      we allocated above.  Making this restriction prevents register
      preferencing from creating worse register allocation.
@@ -1093,6 +1106,7 @@
 	    }
     }
  no_prefs:
+  if (diag_file) fprintf (diag_file, " final_best=%d\n", best_reg);
 
   /* If we haven't succeeded yet, try with caller-saves. 
      We need not check to see if the current function has nonlocal
```

## Appendix B. find_reg simulator (`galloc.mjs`)

It parses `;; N conflicts:` and the DIAG lines from the diagnostic `.greg`, and
replays find_reg in `regs to allocate` order. Run `node galloc.mjs <greg>` to
report agreement with cc1. `simulate(d, {conf, someone, order})` is the hook
the requirement searches use.

```js
// Model of GCC 2.95.2 global.c find_reg over a diag .greg dump.
// usage: node galloc.mjs <greg> [--json]
import fs from 'fs';
export function parse(text) {
  const order = text.match(/regs to allocate: ([\d ]+)/)[1].trim().split(/\s+/).map(Number);
  const conf = {}, hconf = {}, calls = {}, full = {}, someone = {}, copypref = {}, pref = {}, actual = {};
  for (const m of text.matchAll(/^;; (\d+) conflicts:([\d ]*)$/gm)) {
    const r = +m[1]; const xs = m[2].trim().split(/\s+/).filter(Boolean).map(Number);
    conf[r] = new Set(xs.filter(x => x >= 76 && x !== r)); hconf[r] = new Set(xs.filter(x => x < 76));
  }
  const set = s => new Set((s || '').split(',').filter(Boolean).map(Number));
  for (const m of text.matchAll(/^;; DIAG pre-expand r(\d+).* calls=(\d+)$/gm)) calls[+m[1]] = +m[2];
  for (const m of text.matchAll(/^;; DIAG pruned r(\d+) full\{([^}]*)\} someone\{([^}]*)\}/gm)) { full[+m[1]] = set(m[2]); someone[+m[1]] = set(m[3]); }
  for (const m of text.matchAll(/^;; DIAG find r(\d+) alt=0.*?copypref\{([^}]*)\} pref\{([^}]*)\}.*final_best=(-?\d+)/gm)) { copypref[+m[1]] = set(m[2]); pref[+m[1]] = set(m[3]); if (!(+m[1] in actual)) actual[+m[1]] = +m[4]; }
  // initial regs_used_so_far: call-used + regs used by local alloc (approximate with every hard reg <16 or 24,25,31 plus those seen)
  return { order, conf, hconf, calls, full, someone, copypref, pref, actual };
}
const FIXED = [0, 1, 26, 27, 28, 29, 31];
const CALL_USED = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25];
const GR = new Set(Array.from({ length: 32 }, (_, i) => i));
export function simulate(d, opts = {}) {
  const order = opts.order || d.order;
  const conf = opts.conf || d.conf, someone = opts.someone || d.someone;
  const hconf = {}; for (const r of order) hconf[r] = new Set(d.hconf[r] || []);
  const used = new Set([...CALL_USED, ...FIXED.filter(x => x === 31)]); // regs_used_so_far
  const assign = {};
  for (const r of order) {
    const used1 = new Set(FIXED); for (let i = 32; i < 76; i++) used1.add(i);
    if (d.calls[r]) CALL_USED.forEach(x => used1.add(x));
    hconf[r].forEach(x => used1.add(x));
    let best = -1;
    for (let pass = 0; pass <= 1 && best < 0; pass++) {
      const u = new Set(used1);
      if (pass === 0) { for (let i = 0; i < 76; i++) if (!used.has(i)) u.add(i); (someone[r] || []).forEach(x => u.add(x)); }
      for (let i = 0; i < 76; i++) if (!u.has(i)) { best = i; break; }
      if (best >= 0) {
        // preference overrides use the same 'used' set
        const cp = [...(d.copypref[r] || [])].filter(x => !u.has(x)).sort((a, b) => a - b);
        const pp = [...(d.pref[r] || [])].filter(x => !u.has(x)).sort((a, b) => a - b);
        if (cp.length) best = cp[0]; else if (pp.length) best = pp[0];
      }
    }
    assign[r] = best;
    if (best >= 0) { used.add(best); for (const o of conf[r] || []) if (hconf[o]) hconf[o].add(best); }
  }
  return assign;
}
if (process.argv[1] && process.argv[1].endsWith('galloc.mjs')) {
  const d = parse(fs.readFileSync(process.argv[2], 'utf8'));
  const a = simulate(d);
  let ok = 0, bad = [];
  for (const r of d.order) { if (a[r] === d.actual[r]) ok++; else bad.push(`r${r}: sim ${a[r]} actual ${d.actual[r]}`); }
  console.log(`simulated ${d.order.length} allocnos, ${ok} agree with cc1`); bad.forEach(b => console.log('  ' + b));
}
```
