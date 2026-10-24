# Flag-override review

Snapshot of 2026-10-23, at `6cfec884`. It lists every function that has a
`flag-override` entry in `.pi/autoloop.json` (`sourcePolicy.allowlist`) or a
`CC1FLAGS_<stem>` line in `configs/flag_overrides.mk`. The evidence column
points at the override line in `configs/flag_overrides.mk`; each override's
evidence comment sits directly above that line.

| # | Function | Flags | Evidence | Notes |
|---|---|---|---|---|
| 1 | func_80014494 | `-fno-cse-skip-blocks` | `flag_overrides.mk:79` | |
| 2 | func_800183E0 | `-fno-gcse` | `flag_overrides.mk:111` | |
| 3 | func_80018B98 | `-fno-gcse` | `flag_overrides.mk:135` | |
| 4 | func_80019610 | none | none | **Stale:** allowlisted, but has no override |
| 5 | func_80022794 | `-fno-rerun-cse-after-loop` | `flag_overrides.mk:171` | Also has a pin and an empty asm; see `asm-review.md` |
| 6 | func_8002470C | `-fno-schedule-insns -fno-schedule-insns2` | `flag_overrides.mk:237` | The file marks it legacy, pending re-validation |
| 7 | func_8002495C | `-fno-schedule-insns -fno-schedule-insns2` | `flag_overrides.mk:209` | The file marks it legacy, pending re-validation |
| 8 | ovl_11_func_800E4BA4 | `-fno-rerun-loop-opt` | `flag_overrides.mk:408` | |
| 9 | ovl_11_func_800E7660 | `-fno-cse-skip-blocks` | `flag_overrides.mk:538` | |
| 10 | ovl_11_func_800EF8BC | `-fno-cse-skip-blocks` | `flag_overrides.mk:561` | |
| 11 | ovl_11_func_800F00E4 | `-fno-gcse` | `flag_overrides.mk:264` | |
| 12 | ovl_11_func_800F0E00 | `-fno-strength-reduce` | `flag_overrides.mk:512` | |
| 13 | ovl_11_func_800F1678 | `-fno-cse-skip-blocks` | `flag_overrides.mk:434` | |
| 14 | ovl_11_func_800F3D40 | `-fno-gcse` | `flag_overrides.mk:252` | |
| 15 | ovl_11_func_800F8224 | `-fno-strength-reduce` | `flag_overrides.mk:459` | |
| 16 | ovl_11_func_80103B24 | `-fno-cse-skip-blocks` | `flag_overrides.mk:274` | |
| 17 | ovl_11_func_80104148 | `-fno-cse-follow-jumps` | `flag_overrides.mk:296` | |
| 18 | ovl_11_func_80104EC8 | `-fno-cse-skip-blocks` | `flag_overrides.mk:285` | |
| 19 | ovl_11_func_80113FF0 | `-fno-cse-follow-jumps -fno-cse-skip-blocks` | `flag_overrides.mk:584` | |
| 20 | ovl_11_func_8011AA64 | `-fno-cse-follow-jumps -fno-cse-skip-blocks` | `flag_overrides.mk:667` | |
| 21 | ovl_11_func_8011AC30 | `-fno-cse-follow-jumps -fno-cse-skip-blocks` | `flag_overrides.mk:657` | |
| 22 | ovl_11_func_8011ADF4 | `-fno-cse-follow-jumps -fno-cse-skip-blocks` | `flag_overrides.mk:629` | |
| 23 | ovl_15_func_801335F8 | `-fno-gcse` | `flag_overrides.mk:474` | |
| 24 | ovl_17_func_800B9158 | `-fno-cse-skip-blocks` | `flag_overrides.mk:376` | |
| 25 | ovl_21_func_800B98CC | `-fno-schedule-insns` | `flag_overrides.mk:320` | |
| 26 | ovl_21_func_800BA698 | `-fno-gcse` | `flag_overrides.mk:343` | |
| 27 | ovl_25_func_800B95B0 | `-fno-cse-follow-jumps` | `flag_overrides.mk:610` | |

## Summary

26 functions have a live override. Row 4 is an allowlist entry with nothing
behind it.

| Flags | Functions |
|---|---|
| `-fno-cse-skip-blocks` | 7 |
| `-fno-gcse` | 6 |
| `-fno-cse-follow-jumps -fno-cse-skip-blocks` | 4 |
| `-fno-cse-follow-jumps` | 2 |
| `-fno-strength-reduce` | 2 |
| `-fno-schedule-insns -fno-schedule-insns2` | 2 |
| `-fno-schedule-insns` | 1 |
| `-fno-rerun-loop-opt` | 1 |
| `-fno-rerun-cse-after-loop` | 1 |

By container: 15 in ovl_11, 6 in the EXE, and 5 in the other overlays.
