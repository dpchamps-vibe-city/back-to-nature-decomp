# Per-file compiler flag overrides
#
# Format:  CC1FLAGS_<stem> := <extra flags>
#   where <stem> is the source filename without path or .c extension
#
# These flags are APPENDED to the base CC1FLAGS in the FlagsSwitch macro.
#
# POLICY (softened 2026-08-06, owner-approved): flag overrides are a
# legitimate, evidence-gated matching tool — original TUs really were built
# with per-file flags, and a flag can be the TU's true state rather than a
# workaround (proof: -mno-split-addresses on func_80016C08/func_800165D8).
# An agent may add an entry itself when the psx_flag_probe escalation bar is
# met: a target fingerprint, a flag column that dominates baseline, and no
# contrary witness in the same region. Every entry must carry a comment
# stating that evidence, and the matching allowlist entry in
# .pi/autoloop.json (sourcePolicy.allowlist, kind "flag-override") must be
# added in the same change — the allowlist is the audit trail, and the gate
# enforces that it exists. Speculative flag-shopping without a fingerprint
# remains forbidden. The former two UI-counter scheduling exceptions are
# retired; their complete module now matches baseline (see below).





# func_80011370: NO override. The -mno-split-addresses entry that used to sit
# here was withdrawn 2026-08-08 — it was fitted to the 21 self-clobber lui/lw
# pairs, but the target proves split addresses were ON for this TU:
#   0x8001143C / 0x80011444  lui $s0,%hi(D_8005E5E8) and addiu $s0,$s0,%lo()
#                            occupy the delay slots of two *different* jal
#   0x800116B0               lui $v0,%hi(jtbl_80010008) sits in a beqz slot
#   0x8001189C               sw $zero,%lo(D_80070CC4)($v1) sits in a j slot
# A single assembler macro cannot straddle a delay slot, so those lui/%lo
# halves are separate RTL insns. Removing the override took the exact-index
# match from 63/566 to 224/557. The self-clobber pairs are an assembler
# question (see the D_80010098 comment in src/func_80011370.c), not a
# compiler-flag question.

# func_80016C08 / func_800165D8: the -mno-split-addresses overrides that used
# to sit here were withdrawn 2026-08-08. They existed to force the unsplit
# assembler macro for D_8005E3C0 (lui $v1,%hi / lw $v1,%lo($v1), one
# register). That was the wrong lever: the symbol is a 4-byte scalar, so cc1
# already leaves it unsplit — the pair was being broken by GNU as resolving it
# GP-relatively, because this TU does not own it. Ownership is now stated in C:
# a TU that owns a global defines it tentatively, and one that does not leaves
# it extern and gets absolute addressing (ADR-0001 §2.4). Both functions match
# under baseline flags.
# `make check` passes with no per-file compiler flag overrides in the project.

# Complete pad TU, exe:[0x800139FC,0x80014554), reviewed migration of
# exe:0x80014494's existing -fno-cse-skip-blocks setting. The operator
# approved applying this one setting to all 17 members. GP-relative gate/
# mode-ID ownership and internal calls support membership independently.
# Target 0x800144F4 re-forms the buffer's low address after the inner guard
# while sharing the earlier shift; the natural source loses that formation
# at baseline. This is not an impossibility proof for other baseline C.
# Whole merged-source matrix: baseline 16/17 exact (only 0x80014494 differs),
# no-skip 17/17 exact; no compiler diagnostics. The 16 unchanged members are
# neutral witnesses, not proof of the original compiler command line.
# Whole-source matrix: build/tuFinal/pad-whole/matrix.json.
# Exact approved scope/options are in
# configs/tu_flag_reviews.json; the checked fold remeasures every baseline
# member before linked verification. No other construct grant is broadened.
CC1FLAGS_modules__pad := -fno-cse-skip-blocks

# func_800231E8 / func_80023060: NO override. The -mno-split-addresses entries
# that used to sit here were withdrawn 2026-10-21. They existed to force the
# unsplit self-clobbering lui/lh loads of D_800A3FB0/D_800A3FB4, which the
# generated globals.h declared as >8-byte s16[5] arrays. Declared as the s16
# scalars they are (include/globals_override.h), cc1 leaves them unsplit and
# both functions match byte-for-byte under baseline flags; the flag was the
# same wrong lever as the withdrawn func_80016C08/func_800165D8 entries.

# func_800183E0: NO override. An inverted halfword status and staged negative
# coordinate calculations keep the branch-local computations distinct until
# combine, without changing their results. The complete text-TU control matches
# all 491 words at baseline, with PRE 0 substitutions / 0 insertions.
# The former -fno-gcse permission is retired.

# func_80018B98: NO override. A two-byte local command record preserves the
# raw HImode read separately from the word-sized decode/return scratch.
# In the complete text-TU control PRE makes zero substitutions/insertions and
# all 294 words match baseline. The old load-hoist/impossibility claim concerned
# earlier promoted-scalar inputs, not every clean-C representation. The former
# -fno-gcse entry and its permission are retired.


# AnimateMessageWindowRect (80022794): NO override. The current clean C
# and the complete message_window TU match at baseline. The retired
# -fno-rerun-cse-after-loop entry described a superseded reconstruction;
# see notes/file-groupings.md.

# StepUiSpriteFourFramePhase / StepUiSpriteTwoFramePhase (8002470C/8002495C):
# NO override. Both scheduling-off entries are retired. The complete seventeen-
# member ui_sprites TU matches baseline with byte increment, byte quotient and
# subtract operations. Earlier impossibility claims described the old local-
# accumulator/sentinel reconstruction, not the clean-C domain. The checked
# fold proves both counters, all consumers and the linked EXE at baseline;
# see notes/file-groupings.md, sprite render dispatch wrappers.


# ovl_11_func_800F3D40: -fno-gcse. The target materializes D_8006C838+0x4AD5
# and +0x4ADF as two independent lui %hi(D_8006C838) / addiu %lo / addiu-offset
# preheaders. Under baseline gcse+rerun-cse-after-loop the two offsets collapse
# (loop2 re-derived as (D+0x4AD5)+10 through the loop-1 biv final-value
# REG_EQUAL) or the two identical %hi(D_8006C838) halves merge into one register
# live across loop1 — both provably unreachable from any natural C spelling
# (residual-source-space search exhausted all 6 candidates; ~15 hand variants
# across pointer/index/count-up/countdown forms failed). Flag-probe matrix on the
# matching source: baseline 3/18, every other flag <= 11/18, -fno-gcse 18/18
# (18 instructions = target 18); diffFunc VERDICT MATCH. Precedent: func_80018B98
# already carries -fno-gcse in this project's exe; per-TU flags exist in ovl_11
# (ovl_11_func_8011FF74: -fno-schedule-insns).
# RETIRED: an in-loop byte-offset temporary (index + bank offset), then
# base + offset, gives both independent reverse pointer scans at baseline.
# The complete seven-entry control is exact with GCSE/loop passes enabled.
# The impossibility claim above covered earlier preheader/pointer inputs only.

# RETIRED ovl_11_func_800F00E4 -fno-gcse: signed-halfword offset records
# initialized before each outer loop keep the two offset additions distinct
# through GCSE. The base expressions are inside the outer loops; loop.c
# hoists them independently and combine then folds the halfword constants.
# Baseline reproduces all 78 words, including both three-word preheaders.
# Evidence: build/tuFlagReview/ovl_11/collection_halfword_offsets/baseline.
# Earlier claims of baseline impossibility covered literal-offset inputs.

# ovl_11_func_80103B24: the original keeps separate positive/negative
# read-modify-write arms on the same work-area field, including a jump after
# the positive store and a delayed a2=a3 pointer copy. Baseline CSE folds
# these arms into one store (49 instructions); -fno-cse-skip-blocks retains
# them and produces all 55 original words from clean C. The complete fifteen-
# member quantity-editor source is 14/15 exact at baseline and 15/15 with
# this setting after retaining the renderer's upper work-area base. Scope and
# original exception identity: configs/tu_flag_reviews.json.
CC1FLAGS_overlays__ovl_11__modules__quantity_editor := -fno-cse-skip-blocks

# ovl_11_func_80104EC8: repeated full D_8006C838 address materializations
# after actor/date guard diamonds (e.g. 0x80105260 and 0x80105674).
# Baseline CSE skips those diamonds and carries the address through them;
# the final-source matrix emits 1793 words versus the original's 1982.
# Disabling only skip-blocks reproduces all 1982 relocated words, including
# all three jump tables' references, with residual [0, 0, 0, 0]. Disabling
# follow-jumps too emits 1988 words: it destroys sharing the target retains
# within case 0x15. Scheduling, GCSE and CSE jump following remain enabled.
# Same-overlay precedent: 80103B24 above. See the function's research note.
CC1FLAGS_overlays__ovl_11__modules__item_condition_dispatch := -fno-cse-skip-blocks

# Retired 80104148's no-follow setting: a local signed-halfword label/column
# pair retains the actual label ID and the 61-pixel starting column through
# CSE, then folds without synthetic USE spills. Both fields are consumed.
# Baseline preserves all 147 words, the 56-byte frame and every later narrow
# column update. build/tuFlagReview/ovl_11/dialog_row_readable/baseline.

# Retired ovl_21:800B98CC's -fno-schedule-insns: a shared result return
# outside the inverted two-arm condition preserves both store/return tails
# at baseline (all 16 words exact in the complete scene-fold experiment).
# The old universal claim applied to separate per-arm returns.

# Retired ovl_21:800BA698's -fno-gcse in modules/scene_simulation.
# Initialize the count-load base AFTER the window-entry guard, before the
# outer do-loop: PRE's high-half copy no longer overlaps the first bound.
# Reuse the active-word address scratch for the loaded per-slot active flag;
# both assignments are consumed. That changes the web from one isolated local
# quantity to an ordinary multi-set global web, reproducing both occurrences
# without extra instructions. All 46 members match at baseline in the complete
# control and checked fold; no module override or permission replaces this.
# Prior impossibility/flag claims assumed the split address/flag variables.

# Retired ovl_17:800B9158's -fno-cse-skip-blocks in modules/record_scene.
# Existing Ovl17QueueView entry-member lvalues retain the word/halfword
# address-expression distinction lost by raw byte-pointer arithmetic. The
# checked whole-61 baseline fold proves all members and linked ovl_17 exact.
# The earlier universal CSE-impossibility claim applied only to those inputs.

# Retired ovl_11:800E4BA4's -fno-rerun-loop-opt during reconstruction of
# modules/object_program_install. Indexing the archive's halfword tag array
# directly (instead of carrying a second pointer induction variable) preserves
# the target's one-hoist loop at baseline. The checked whole-eleven fold proved
# every member and the linked overlay byte-identical; the former universal
# baseline-impossibility claim covered only the pointer-walk reconstruction.


# Retired ovl_11:800F8224's -fno-strength-reduce in modules/record_slot_menu.
# Explicit scan backedges retain the target's recomputed six-byte addresses;
# a C loop construct instead introduces LOOP_BEG and a strength-reduced walker.
# The checked whole-twelve baseline fold proves every member and linked ovl_11
# exact. The old flag column described the do-while reconstruction, not a TU.

# ovl_15_func_8013345C: NO override. The -fno-gcse entry that used to sit here
# was withdrawn 2026-10-21. It was fitted to the m2c goto-form reconstruction
# (54 instructions at baseline). The idiomatic Ovl15RecordPools scan that
# replaced it compiles to identical words with or without -fno-gcse and is
# byte-exact under baseline flags.

# Retired ovl_15:801335F8's -fno-gcse in modules/save_slot_selection.
# A single next-column assignment at the base-selection join preserves the
# target's latch narrowing; the compiler duplicates its add into both arms.
# Cache the complete mark-table address before the outer loop and form its
# bank byte offset first. This avoids the old HIGH-only PRE/rematerialization
# web and reproduces the preheader's registers/operand order at baseline.
# All thirteen members and the linked overlay pass the checked whole-unit
# fold. The prior flag requirement described the split source, not the TU;
# no module override or replacement permission is introduced.

# ovl_11_func_800FE068: NO override. The -fno-strength-reduce entry was
# withdrawn after remeasuring the actual matched source under overlay -G0.
# Its do-while with var_s0 = var_a2 + 1 is byte-exact 57/57 under both
# baseline and -fno-strength-reduce. The earlier walker fingerprint applied
# to another source shape, not this one. A shared flag therefore cannot
# support grouping it with ovl_11_func_800F8224.

# Retired ovl_11:800F0E00's -fno-strength-reduce in condition_evaluation.
# Reconstructing the target's 36-byte offset in its three arithmetic steps
# retains the indexed scan at baseline. The integer-address addition retains
# its operand order. The checked complete-28 fold proves every member and
# linked ovl_11 exact. The earlier universal claim covered multiply-shaped
# loop inputs, not this address representation; no module setting replaces it.


# ovl_11_func_800E7660: -fno-cse-skip-blocks.
#
# Target fingerprint (decoded from the original bytes, no source needed): the
# dominating block at 0x800E7664 materializes D_8006C838 in $a3
# (`lui $v1,%hi(D_8006C838)` / `addiu $a3,$v1,%lo(D_8006C838)`), and the join
# block at 0x800E7744 re-forms only the low half from the CSE-shared %hi held
# in $t1 — `addiu $v1,$t1,%lo(D_8006C838)` — even though no call or clobber
# sits on the fall-through path between them. Under baseline -fcse-skip-blocks
# cse.c follows the branch around the one-block `if` in the arg0 == -1 arm and
# carries the base pseudo into the join, so the lo_sum there is folded away and
# the whole base stays in one register; the target's re-materialised lo_sum is
# unreachable from any C shape at baseline. Same mechanism as func_80014494 and
# ovl_17_func_800B9158 (both already carry this flag).
#
# Flag column: on the final source, baseline is 59/78 words (15 words differ);
# -fno-cse-skip-blocks is 78/78 byte-identical (`diffFunc` VERDICT MATCH). Every
# other measured column is far below: the flagProbe matrix over the candidate
# family scores -fno-gcse 1/78, -mno-split-addresses 1/78, -fno-cse-follow-jumps
# 21/78, -fno-schedule-insns{,2} 26/78, -fno-rerun-cse-after-loop 41/78, and
# -fno-cse-skip-blocks 72/78 masked on the pre-final spelling.
#
# No contrary regional witness: this src file is its own TU (one function per
# file), so the override cannot disturb the matched ovl_11 neighbours.
CC1FLAGS_overlays__ovl_11__modules__clock_threshold_command := -fno-cse-skip-blocks


# Retired ovl_11:800EF8BC's -fno-cse-skip-blocks in condition_date_records.
# Structured rejection returns, case breaks and a shared success return keep
# the target's rematerialized low halves at baseline. The checked whole-seven
# fold preserves every member and the linked overlay. The old unreachable
# claim was conditional on shared-failure-tail CFGs, not all clean C. No TU
# setting replaces it; the canonical record view also removes local conflicts.

# Retired ovl_11:80113FF0's two CSE overrides in price_option_rows.
# Complete its final gated row append: advance q at the join, then advance
# each parallel cursor on its store. This preserves the 101-word pointer-walk
# target at baseline, as for the complete prompt_option_rows family.
# The checked whole-five fold preserves every member and linked overlay;
# no module setting replaces it. Earlier 88-word forms stopped at the last
# row and did not establish baseline unreachability for a complete append.


# Retired ovl_25:800B95B0's no-follow override. Transfer the initial signed
# 16-bit selector through its bit-field before Rand, retaining that width
# boundary until combine instead of teaching CSE its value across the cascade.
# Baseline then retains both zero assignments and the original branch sense:
# all 106 words match. Evidence: build/tuFlagReview/ovl_25/
# choice_initial_bitfield/baseline/report.json and the checked complete
# scene_state_machine fold. No replacement flag or permission is introduced.

# Retired ovl_11:8011AA64/8011AC30/8011ADF4's two CSE overrides.
# Complete the unrolled append of the gated last row: advance the text cursor
# after the gate, then post-increment each parallel row-field cursor. This
# retains all target walks at baseline; the earlier reconstruction stopped
# its cursors on the final row and folded those walks into displacements.
# The checked eleven-member prompt_option_rows fold proves the whole unit
# and linked overlay. No module flag or new exception replaces the entries.
