# Plan: route stalled residuals to the owning pass, and spend less context

**Status: proposed 2026-10-10.** This plan follows the analysis of the
autoloop run started 2026-10-10 06:20, the first full run after `fdedbe9f`
("better steering docs"). The session is
`~/.pi/agent/sessions/--home-dave-Documents-code-btn-decompilation--/2026-10-10T06-20-01-625Z_01a12477-ecd8-74a6-a015-002f352c4539.jsonl`.

## The theme

The run was a clear improvement. The 10-09 04:55 run used the same model
(deepseek-v4-1-flash) at the same thinking level, on functions of similar
size.

| | 10-09 run | 10-10 run |
|---|---|---|
| Functions decompiled | 36 in 11.2 h | 66 in 9.9 h |
| Median minutes per function | 10.5 | 4.3 |
| 90th-percentile minutes | 38.9 | 17.2 |
| Minutes per 100 target instructions | 23.0 | 9.9 |
| Median turns | 81 | 36 |
| Input tokens per function | 20.5M | 9.3M |
| Functions over 10 min | 19 | 12 |

The function sets differ, so the comparison is consistent with the doc
changes helping but does not prove it.

The remaining cost is concentrated in a few episodes. Twelve decompile
episodes (18%) took 49% of decompile time (283 of 501 min) and 68% of input
tokens (415M of 613M). One of them, func_8001BBD8, was parked.

**Eleven of the twelve stalled the same way.** The agent modelled a compiler
pass that did not own the residual. A reference sheet it never opened, or
opened late, already named the source change. The exception, 80109594, lost
its time to the switch structure and to finalize bookkeeping. The harness
contributes directly: the pipeline reversal pointed at the wrong sheet, and
the checkpoint redirected the agent to a generic one.

| Function | Minutes | What the agent worked on | What fixed it | Where the fix was written |
|---|---|---|---|---|
| ovl_11_func_80117F70 | 65 | scheduler and allocator; about 12,600 permutation variants | birth order of store-block constants | population, "Store-block initializers"; `psx_analyze_store_block` |
| ovl_11_func_800CB730 | 48 | cross-jump and allocator source; 44 identical outputs | m2c join temporaries replaced by one store per arm | stuck sheet, web-population premise (read 3 times, not applied) |
| ovl_19_func_800BBB48 | 23 | allocation, then sched.c | `__asm__ volatile("" ::: "memory")` | schedule §6; the skill forbids it |
| ovl_19_func_800B82B8 | 21 | scheduler model; 28 compiler-source reads | loop-preheader position, `*(D+0xB0+i)` | loop §1; triage `loop-preheader-order` |
| ovl_11_func_80109594 | 17.6 | type and cast spellings, then finalize bookkeeping | switch fall-through structure; jump-table attribution | — |
| ovl_23_func_800BA494 | 17 | allocation | commutative `addu` operand order, fixed with a typed array view | schedule, CSE "constant-second" rule |
| ovl_11_func_800E9104 | 17 | allocation tools while cfg stayed 1 for 14 min | missing return block, `goto ret1` | population, opening rule |
| ovl_11_func_800CC6A8 | 16.6 | allocation while pop 3 was open | return/join tail restructure | population, "Epilogue return/join" |
| ovl_11_func_800BF720 | 16.5 | tree folding | early exit proving a later arm dead | population §1 |
| ovl_11_func_800D7EF8 | 16.4 | one axis at a time | two interacting levers | allocation §4, "Test these as interactions" |
| ovl_11_func_800E8F78 | 13.3 | wrongly concluded there is no cross-jumping | symmetric arms so jump2 cross-jumps them | population (read late) |
| func_8001BBD8 | 10.5, parked | — | exact at 5 min, but needs GTE inline-asm defines | park was correct under policy (see Phase 4) |

Context fills mostly from the model's own reasoning. Pi's openai-completions
provider replays thinking in the `reasoning` field, and the provider counts it
as context.
- From one turn to the next, context grows by about the previous turn's
  output, even when the tool results are small.
- Across the decompile episodes, 5.2M of 10.8M tokens of context growth were
  output tokens.
- The stalled episodes carried 470–865k characters of reasoning each, in
  single blocks of 20–40k characters.
- The agent re-derived the same fact up to 9 times.

Several episodes reached the 300k checkpoint after about 10 minutes.

## Design principle

The tool that knows the owning pass must name the sheet for that pass. Every
later step must use the same answer: the checkpoint, the skill's stall
procedure, triage. A diagnostic must measure the candidate it was asked about,
never the live stub. Doctrine and configuration must not contradict each other
about what is permitted.

## Phase 1: the reversal names the owning pass's sheet

Defects, verified in code:

1. `tools/agent/pipeline-reversal/render-text.ts:70` resolves the sheet from
   `firstDivergence.stage`, which is the waypoint's name.
   - The `lreg` waypoint measures instruction population under register
     masking, and `OWNER_TO_SHEET` (`tools/agent/reference.ts:27`) maps `lreg`
     to `allocation`.
   - So a population-owned residual prints `mechanism sheet: psx_reference
     allocation`, while the owner line two lines above says expand / cse /
     gcse / loop / combine.
2. The `greg` waypoint (`tools/agent/pipeline-reversal/reverse.ts:271`) agrees
   only when `orderDifferences === 0` as well as `allocationDifferences === 0`.
   - A residual that differs only in order, such as 80117F70 (sched 5, alloc 0),
     is therefore owned by "local-alloc / global-alloc — the same values,
     allocated to different hard registers".
   - That is false when no web is allocated differently.

Effects in the run:
- All 65 sheet pointers the reversal printed said `allocation`.
- Agents loaded `allocation` 14 times, `stuck` 15, `population` 2, and
  `schedule`, `loop` and `sdk` once each.

Work:

- Derive the sheet from the residual owner, not the waypoint name:
  - population owners (expand, cse, gcse, combine) → `population`;
  - loop → `loop`;
  - sched1 or sched2 → `schedule`;
  - local-alloc or global-alloc → `allocation`.

  Keep `OWNER_TO_SHEET` for the pass names a caller types. Waypoint names do
  not go through it.
- Split the `greg` disagreement:
  - Order differences with zero allocation differences are an order residual.
    Name sched1/sched2 and the schedule sheet.
  - When both are present, say which is older. Mark the allocation reading
    provisional until order agrees, the same way population already gates the
    later stages.
- A residual of order and allocation only, with order in a loop preheader,
  carries triage's `loop-preheader-order` finding inline and names `loop`.

Acceptance:
- Unit tests cover each ladder case: population, order only, allocation only,
  order + allocation, dbr, assembler. Each asserts the owner text and the
  sheet.
- Re-running the reversal on candidates preserved from this run gives:
  - population sheet for 800CB730 at pop 48, 800BF720, 800E9104 and 801110CC;
  - schedule sheet for 80117F70 at sched 5 / alloc 0.

  None of these may print `allocation`.

## Phase 2: the checkpoint loads the owning sheet, and keeps the best candidate

Defects:
- `.pi/extensions/psx-decomp/autoloop/checkpoint.ts:28` tells every checkpoint
  to load `psx_reference stuck`. In each stalled episode that was the sheet
  read at the checkpoint, and its premise audit found nothing. The owning
  sheet stayed unread.
- The checkpoint summary names the best candidate from the agent's memory. In
  80117F70 it named y0 (93/94, alloc 1) instead of t1 (95/95, alloc 0). That
  cost about 5 minutes chasing an allocation residual that existed only in y0.

Work:
- The checkpoint message names the owning pass and sheet from the latest
  reversal or objective reading of the ledger's best candidate. It tells the
  agent to load that sheet if it has not read it in this episode. It keeps
  `stuck` as the second step, for when the owning sheet has been read and the
  residual still holds.
- The checkpoint message quotes the ledger's best candidate: path, verdict,
  stage counts. The agent's summary must start from it.

Acceptance:
- Tests assert that the checkpoint text names the sheet derived from a
  fixture ledger and reversal.
- They also assert that the checkpoint text quotes the ledger's best row
  rather than asking for one.

## Phase 3: every diagnostic measures the candidate, never the stub

Verified: `psx_explain_diff`, `psx_compiler_trace`, `psx_fuzz_variants` and
`psx_search_scheduler_state` have no `source` parameter. The transcripts also
report stub-only behaviour from `psx_near_miss_repair`,
`psx_synthesize_source_shapes`, `psx_analyze_target_schedule`,
`psx_inspect_local_allocation_variant` and `psx_allocator_counterfactual`.
Confirm each tool's parameters before changing it.

Effects:
- `psx_explain_diff` reported a stub as "exact" and misled the agent twice
  (800BA494).
- Agents copied candidates into `src/` and back to make tools work: 8 times
  in 80117F70, 13 cycles across three episodes.
- About 330 bash calls compiled by hand or ran objdump.
- Agents wrote private compile scripts into `tools/agent/`. In 800BBB48 one
  left behind failed the finalize gate.
- With the live file a stub, the objective's baseline row is `STUB`. Verdicts
  then read "scored" instead of better / identical / worse, so the skill's
  "identical, change axis" rule never fires.

Work:
- Give each of these tools a `source` argument. When it is omitted, default to
  the ledger's best candidate, and print which source was used.
- Refuse, with a message naming the `source` argument, when the resolved
  source is an `INCLUDE_ASM` stub. Never report a stub as exact.
- When the live file is a stub, `psx_residual_objective` compares verdicts
  against the ledger's best candidate. It also prints the candidate's `.s`
  path and a side-by-side listing of only the differing words. Agents built
  that listing with objdump and python in at least four episodes.

Acceptance:
- Per-tool tests run against a stubbed live file and a candidate path.
- `explain_diff` on a stub exits non-zero with the refusal text.

## Phase 4: remove doctrine and configuration contradictions

1. **The empty memory barrier.**
   - The source policy allows it by default
     (`.pi/extensions/shared/config.ts:39`, `allowEmptyMemoryBarrier: true`),
     and `prompts/reference/schedule.md` §6 documents when to use it.
   - The decompile skill (`.pi/skills/psx-decompile-function/SKILL.md:356`)
     says embedded assembly needs an allowlist entry, with no exception. In
     800BBB48 the agent called the barrier "forbidden" and lost about
     18 minutes.
   - Add one sentence naming the exception the active policy grants, and point
     to the schedule sheet.
2. **GTE placeholder words (func_8001BBD8).**
   - `evidence.md` demands the production `rtir`/`rtv0tr` encoding and forbids
     the raw `.word` placeholder.
   - No header supplies that encoding, so the only match uses inline-asm
     defines, which the gate refuses. The park was correct; the contradiction
     is in the harness.
   - Owner decision: either supply a DMPSX-equivalent mapping from placeholder
     words to mnemonics in the GTE header, or stop the evidence from demanding
     an encoding that no permitted construct produces.
   - In either case, the finalize embedded-asm failure should say "file the
     allowlist request and return". The agent spent 27 turns probing the gate,
     including reading `source-policy.ts` 4 times.
   - On park, prefer the integrated on-disk source over an earlier ledger
     candidate with the same words.

Acceptance:
- The skill text and the policy agree: a test greps the skill for the
  barrier exception whenever `allowEmptyMemoryBarrier` is true.
- The GTE item has an owner decision recorded.

## Phase 5: context budget

Measured costs:
- Startup is a median 62k tokens before the first measurement (21 tool calls).
- The preparation packet is 38% of that, mostly `evidence.md`, read at 25–51k
  characters. For some functions the file is 148–205 KB, mostly
  "(conditional)" entries about other functions: 78 for 800BF720.
- The skill file is 19% (21k characters). Header searches are 12%.
- Some single results hit the 51k-character cap:
  - `ls` and `find` over `build/`;
  - `cat include/overlays/ovl_11.h`;
  - `psx_experiment_ledger`;
  - `psx_verify_build` after a finalize that already verified.
- Directory-mode `psx_residual_objective` gets truncated to its provenance
  lines, losing the ranking table. There were 412 python calls, 106 of them
  parsing tool JSON reports, 43 of those the reversal's `report.json`.

Work:
- `evidence.md` keeps only rows about the target and its direct callees and
  callers. The rest goes into a linked file the handoff names but does not
  ask to read.
- Directory-mode `psx_residual_objective` prints one provenance summary, then
  a deduplicated top-N ranking table last, so truncation that keeps the tail
  keeps the table. `psx_experiment_ledger` defaults to a capped, newest-first
  page.
- `psx_finalize_function` prints the failing step's stderr. In 80117F70 a bare
  "exit code 2" took 5 minutes to trace to the agent's own extern. In
  80109594, 31% of the episode went to discovering `deriveRodataSplits --write`
  and `split-<container>`. Investigate whether finalize should run those
  itself.
- `psx_compiler_source` prints repo-relative paths. There were 13 failed reads
  of `tools/vendor/gcc/2.95.2/gcc/*.c`; the files are under `.../src/gcc/`.

Acceptance:
- On this run's preparation packets, median `evidence.md` is under 15 KB.
- A 50-variant directory score keeps the ranking table under the tool output
  cap.

## Phase 6: failing tools

- `psx_solve_local_allocation` failed 11 of 22 calls with "compiler-oracle
  report lacks baseline/local counterfactual variants", plus one exit 137.
  Fix the oracle report or remove the tool from the agent's tool list until it
  works.
- `psx_control_shape_sweep` failed 13 of 15 calls on its preconditions: "no
  constant/result-valued return tail" and "no side-effect-free decision
  suffix". Agents also passed it source text instead of a path. State the
  preconditions in the tool description and accept only a path.
- `psx_family_transfer` failed 10 times ("no family member with clean matched
  C", "different shapes"). The description should say when it applies.
- `psx_search_scheduler_state` failed 7 times: no `.sched2` stage, "cannot
  derive a unique target-order assertion", MODEL-REPLAY-FAILED. Check each
  against its fixture.

## Phase 7: stall doctrine, general imperatives only

The model behaviours behind the longest plateaus get one sentence each in the
decompile skill's stall section. They contain no function names.

- A change measured together with another change is not refuted. Before
  dropping a lever that raised the matched word count, measure it alone and
  measure it combined with the best candidate.
- When levers interact, test them in pairs. Optionally, the objective
  automatically measures pairwise combinations of the top two improving
  edits.
- A permutation sweep belongs to the axis the residual's NEXT line names.
  Do not sweep an axis whose stage is already zero.
- While cfg or population is open, allocation and schedule readings are
  provisional. `psx_residual_objective` should de-emphasize those cells until
  the earlier stages agree.

Acceptance: the skill text gains these lines, and the existing skill tests
pass.

## Sequencing

1. Phases 1 and 2 first. They are small, verified defects, and they would
   have redirected most of the twelve stalls.
2. Phase 4, item 1, the barrier sentence, alongside them.
3. Phase 3 next. It removes the stub workarounds that drove hand compiles and
   staging into `src/`.
4. Phase 5 and Phase 6 in any order.
5. The Phase 4 GTE owner decision runs on its own schedule.

## Overall acceptance

- `npm test` and `make check-all` pass.
- The next full autoloop run is compared with this one by the same
  measurements:
  - median, 90th-percentile and over-10-minute counts;
  - tokens per function;
  - sheets loaded, against the owner the reversal named.

  Across the run, every sheet pointer matches the owner line. The over-10-minute
  share does not rise.

## Out of scope

- Changing models, thinking levels or the 300k checkpoint threshold.
- Dropping or truncating the reasoning pi replays. The finding in the theme
  explains why context fills; this plan reduces what fills it through better
  routing, not by discarding reasoning.
- Changing compiler flags or any live source.
- The documentation role, which cost 4% of tokens and 13% of time in this run.
