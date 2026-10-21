# Flag-override cleanup: which per-file flags the groupings contradict

Written 2026-10-21. Audits the 28 `CC1FLAGS_<stem>` entries in
`configs/flag_overrides.mk` against the suspected source-file groups in
`notes/file-groupings.md`. Nothing has been changed yet; this is the plan.

## Method

A compiler flag is a property of an original source file, so every function
that file contained was built with it. For each override, every matched
member of the function's group was recompiled with the override's flags and
its assembled instructions (mnemonic, operands, relocation) compared with the
baseline build. Where the group lists no members, the three nearest matched
functions on each side by address stood in.

- A matched sibling whose output **changes** under the flag is a contrary
  witness: one file cannot have been built both ways.
- A sibling that is **unchanged** proves nothing.
- A sibling that **needs the same flag** independently is a supporting witness.

Limits:

- "Changes" means the sibling's *current* source no longer matches. A
  different spelling might match under the flag, so a 1–2 word change is weak
  evidence; 15+ words across a whole span is close to decisive.
- Groups are hypotheses with stated confidence. Address neighbours used as a
  fallback can cross a file boundary.

## Inventory

- 28 overrides at the time of the audit (25 after step 1 below). 23 were live
  in matched C. 5 sit on `INCLUDE_ASM` stubs with
  the attempt under `#if 0`, so they currently affect nothing:
  `func_80018B98`, `ovl_11_func_800F1678`, `ovl_11_func_80113FF0`,
  `ovl_11_func_800F0E00`, `ovl_25_func_800B95B0`.
- Overrides with no `flag-override` allowlist entry in `.pi/autoloop.json`:
  `ovl_15_func_8013345C`, `ovl_11_func_800F8224`, `ovl_11_func_800FE068`
  (all live), `ovl_11_func_800F0E00`, `ovl_25_func_800B95B0` (parked). The
  gate that is supposed to enforce this did not catch them; not yet checked
  why.
- Allowlist rows with no override: `func_80019610`. Duplicate key:
  `ovl_11_func_80113ff0` appears twice in the same object.
- `func_8002495C`'s evidence comment names only `-fno-schedule-insns2`; the
  entry also sets `-fno-schedule-insns`.

## Tier 1: contradicted by the groupings

| Override | Evidence |
|---|---|
| `func_800231E8`, `func_80023060` (`-mno-split-addresses`) | **Confirmed crutch.** Generated `globals.h` declares `D_800A3FB0`/`D_800A3FB4` as `s16[5]` read through a scalar cast. Declared as plain `extern s16`, both functions match byte-for-byte under baseline flags (11 words differ with the array declaration, 0 with the scalar). Same declaration-size mechanism as the withdrawn `func_80016C08`/`func_800165D8` overrides. |
| `func_8002470C`, `func_8002495C` (`-fno-schedule-insns -fno-schedule-insns2`) | Both sit with zero gaps inside the high-confidence sprite-wrapper span 0x800245F4–0x80024AD4, so they share one object with their neighbours. All 6 matched span-mates change under the flag: `800247C0`, `80024770`, `80024810`, `80024860`, `800249C0` 15/20 words each, core `80024A4C` 24/38. Four of them are mod-30 counters using the same `0x88888889` idiom, matched at baseline. |
| `func_80022794` (`-fno-rerun-cse-after-loop`, plus register pin and empty asm) | `80022B98` (75/118) and `80022DF8` (36/43) change. Both are proved group members by the `800225C4` dispatcher's function-pointer table. |
| `ovl_21_func_800B98CC` (`-fno-schedule-insns`) | 28 of 41 matched group members change. |
| `ovl_21_func_800BA698` (`-fno-gcse`) | 19 of 41 matched group members change. It also changes 21/86 words under `800B98CC`'s flag, so the two overrides cannot share a file either (`800B98CC` is unchanged under `-fno-gcse`). |
| `ovl_11_func_800F00E4` (`-fno-gcse`) | Gapless predecessor `800F00AC` changes 13/14. |
| `ovl_17_func_800B9158` (`-fno-cse-skip-blocks`) | 7 of 29 matched members change (group confidence low). `800B8294`'s single changed word is `addu a2,v0,v1 → addu a2,v1,v0`: the exact operand order this flag was adopted to flip. |
| `ovl_11_func_800E7660` (`-fno-cse-skip-blocks`), weaker | 5 of 58 members of a very broad family change: `800EB328`, `800EC490`, `800EC354`, `800E9D28`, `800E9778`. |
| `ovl_25_func_800B95B0` (`-fno-cse-follow-jumps`, parked), weaker | Low-confidence sibling `800B97E4` changes 66/137. |

## Tier 2: supported by the groupings

- `ovl_11_func_8011AA64` / `8011AC30` / `8011ADF4` (plus parked
  `80113FF0`), `-fno-cse-follow-jumps -fno-cse-skip-blocks`: three zero-gap
  matched neighbours independently need the same pair for the same
  pointer-walk fingerprint; 9 other family members are unchanged. Not clean:
  `8011B2B0` changes 2/257 and `80114398` 1/155, both on stores to the same
  `D_8012D548`/`D_8012D5A8` record.
- `ovl_15_func_8013345C` / `801335F8`, `-fno-gcse`: **resolved for
  `8013345C`, likely the same for `801335F8`.** No split or boundary problem
  was found (gapless, `configs/splat/ovl_15.yaml` unchanged). The override on
  `8013345C` was fitted on 2026-10-07 to the m2c goto-form source; the
  2026-10-20 idiomatic rewrite compiles to identical words with or without
  the flag, so the override was dead and is withdrawn. `801335F8` is still
  m2c-shaped: its source spells GCC's own strength-reduced `y` accumulator
  (`(y << 16) + 0x180000`) and the early `x + 0x88` as C, which is what hands
  PRE the shift it hoists. A natural rewrite (nested `for` loops, `s16 y`
  reset to 0x22 at the bottom of the outer loop, `x += 0x88`) reproduces the
  hoisted `0x180000` register and the runtime `sll` at baseline, and makes
  the flag *worse*. Its best spelling so far is 132/132 words with 23
  positions differing: the order of the inner-loop setup block and register
  choice, not a flag.

## Tier 3: no organisational signal

Every neighbour is unchanged and no second function needs the flag:
`func_80014494`, `ovl_11_func_80103B24`, `ovl_11_func_80104148`,
`ovl_11_func_800E4BA4`, `ovl_11_func_800EF8BC`, `ovl_11_func_800F3D40`,
`ovl_11_func_800F8224`, `ovl_11_func_800FE068`, and parked `func_80018B98`,
`ovl_11_func_800F1678`, `ovl_11_func_800F0E00`. These rest only on the
argument that no C spelling reaches the target at baseline, which is the
argument that failed for the split-address pair.

## Why the bad ones got through

- **The neighbour test was defined away.** 13 evidence comments argue there
  is no contrary witness because each `.c` file is its own translation unit.
  That is true of today's layout, not of the original files.
- **Weak precedent.** Comments cite overrides in other containers
  (`8013345C` cites ovl_11) or chain to each other (`800F0E00` cites
  `800F8224` and `800FE068`).
- **Cherry-picked witnesses.** `800B9158` lists four unchanged neighbours;
  seven others in its group change.
- **Stale claims.** `8002495C`'s comment says its siblings are not matched
  yet. They now match at baseline and contradict the flag; the comment was
  never revisited. The sprite-span grouping note records that `800249C0`
  matches with default flags without flagging the conflict.
- **The probe recommends a withdrawn flag.** `tools/agent/flagProbe.ts:24`
  still signals `-mno-split-addresses` for symbolic self-clobbering
  `lui`/`lw` pairs, citing the withdrawn `func_800165D8`/`func_80016C08`
  precedent. That is the likely origin of the `800231E8` pair.

## Plan

1. **Split-address pair. Done 2026-10-21.** `D_800A3FB0`/`D_800A3FB4` are
   now `extern s16` in `include/globals_override.h`; both overrides and their
   allowlist entries are gone, and the `ovl_15_func_8013345C` override was
   withdrawn alongside them. `make check-all` passes (exe and all 13 overlays).
   Root cause to keep in mind: `tools/build/classifyGlobals.ts` turns every
   global outside the gp window into a fake >8-byte array to force split
   addressing, so any small out-of-window scalar whose target shows the
   unsplit self-clobbering load needs a real scalar declaration in the
   override header.
1a. **`ovl_15_func_801335F8`.** Rematch from the natural nested-loop shape at
   baseline and drop its `-fno-gcse` override.
2. **Probe detector.** Retarget the `flagProbe.ts` self-clobber fingerprint to
   the declaration-size and ownership causes instead of `-mno-split-addresses`.
3. **Neighbour test as a tool.** Make the method above a repository check
   (extend `flagProbe`'s regional context to recompile matched group siblings
   under the candidate flag), and require its output in every override's
   evidence comment in place of the own-translation-unit argument.
4. **Reopen tier 1.** Remove the overrides on `func_8002470C`,
   `func_8002495C`, `func_80022794`, `ovl_21_func_800B98CC`,
   `ovl_21_func_800BA698`, `ovl_11_func_800F00E4` and
   `ovl_17_func_800B9158`, and rematch each at baseline. Start with the
   sprite-span pair, where the contradiction is strongest. Then weigh
   `800E7660` and `800B95B0`.
5. **Tier 2.** Inspect the 1–2 word changes in `8011B2B0` and `80114398`
   under the CSE pair. Look for file-boundary evidence at 0x8013345C and
   0x80133808 in ovl_15.
6. **Tier 3.** Re-examine each against a declaration or ownership premise
   before accepting the flag, as step 1 did.
7. **Bookkeeping.** Fix the allowlist gaps, the stale `func_80019610` row,
   the duplicate `ovl_11_func_80113ff0` key and the `8002495C` comment.
   Update bucket 3 of `plans/100-percent-completion.md`, which depends on the
   same `-fno-cse-*` question.
