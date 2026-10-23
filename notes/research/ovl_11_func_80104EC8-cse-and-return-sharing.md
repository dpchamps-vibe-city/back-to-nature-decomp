# ovl_11_func_80104EC8 — CSE skipping and return sharing

## Verified result

The integrated C reproduces all **1982 relocated instruction words**, with
residual `[0, 0, 0, 0]` and no undetermined relocations. `make -j8 check-all`
passes for the PS-X EXE and all 13 configured overlays. The source-policy gate
passes; there is no embedded assembly or register binding in this function.

The signature is `s32 ovl_11_func_80104EC8(s16 arg0)`. Its byte-selector caller,
`ovl_11_func_800F1AA8`, still matches all 14 words after reconciling its local
prototype to the published signature. The eleven direct callee declarations
are corroborated against their matched definitions.

## The flag evidence

The original rematerializes the complete `D_8006C838` address after guard
diamonds: for example, the actor selector at `0x80105260` is followed by a new
`lui` at `0x80105288`; the corresponding pair in case 0x15 is `0x80105674` /
`0x80105694`. Baseline CSE carries an address across these guards instead.
The relevant compiler decision is `cse_end_of_basic_block` in the vendored
GCC 2.95.2 `cse.c`, including its skip-block and jump-following paths.

The final complete-source matrix, before installing the override:

| Additional flags | Emitted words | Evidence |
| --- | ---: | --- |
| none | 1793 | loses repeated address materializations and guarded regions |
| `-fno-cse-follow-jumps` | 1794 | does not repair the skip-block defect |
| `-fno-cse-skip-blocks` | 1982 | relocated-byte EXACT, including all table references |
| both of the above | 1988 | contrary witness: loses sharing retained inside case 0x15 |

Only `-fno-cse-skip-blocks` is installed, with its policy audit entry. GCSE,
scheduling and CSE jump following remain enabled. The earlier parked attempt's
two-flag setting was not adopted. Nearby matched `ovl_11_func_80103B24` provides
same-overlay precedent for this flag, not proof of a shared original TU.

The automated probe's generic loop fingerprint is misleading here: backward
branches to shared return blocks look like nested loops to its interval scan.
Its `not-supported-current-source` conclusion concerns the automatically
suggested no-GCSE columns, not the manually witnessed skip-block fingerprint.
The complete-source matrix and relocated-byte oracle settle this experiment.

## Source mechanisms that mattered

- Partition both the affection amount and threshold variables by switch arm.
  Splitting only one family changes the allocator's conflicts without recovering
  the target. Reusing m2c's function-wide temporary obscured this partition.
- Preserve real coordinate-output storage: the two four-halfword buffers each
  serve two coordinate pairs, and the final buffer holds one pair. Splitting
  these into five independent arrays adds stack alignment that the target lacks.
- Reuse the actual accumulation variables in cases 0x1F, 0x25 and 0x53. Fresh
  temporaries alter scheduler promotion and the placement of signed truncation.
- Case 0x25 uses an indexed five-element table loop. Its initialization belongs
  to loop.c's induction initialization, not an explicitly initialized cursor.
- Case 0x23 walks a byte cursor for the first animal scan and reuses its index
  for the second scan. This preserves the first scan's large field offsets and
  the shared counter allocation; an indexed field-address GIV chooses a
  different base.
- Undo m2c's explicit cross-case sharing of calls and decision tails. Let GCC
  share the repeated date checks and the final zero tests itself.
- A function-wide `success` label is not equivalent compiler input to local
  return joins. `jump.c:find_cross_jump` reduces its minimum when it reaches a
  label, so that spelling can move the common true-return block out of case
  0xA. Natural returns plus a local join in case 4 retain the original anchor.
- The final case-0x72 residual was `[0, 1, 0, 2]`: a local true-return join
  separates the comparison temporaries from the returned constant, recovering
  the original registers and delay slot. Local false joins likewise preserve
  the two guarded tails in case 0x71 instead of folding their Boolean results.

These are measured sufficient source forms, not a claim to recover the
original author's exact spelling.

## Integration and diagnostic limits

All three jump tables occupy the original `0x26F0..0x2930` rodata window.
`deriveRodataSplits.ts --container ovl_11 --write` assigns that window to the
compiled function; `make split-ovl_11` refreshes the linker script. Before that
attribution, six instruction words were legitimately undetermined, not wrong.
The final linked-overlay check also verifies the table contents.

The split regenerated `include/gte_macros.inc` without its existing `rtir`
alias. That unrelated generated change was restored to its known-good tracked
state before verification; no GTE tooling or unrelated source was changed.

Scratch evidence is under `build/ovl_11_func_80104EC8/inspection/`:
`final-context.c`, `final-flag-probe.json`,
`flag-experiments/final-context--fno-cse-skip-blocks/measurement.json`,
`integrated-triage.json`, and `check-all.log`. The integrated triage has no
blockers. Early block-number objectives were degraded when CFGs did not align;
case-region comparisons helped localize work but normalized branch destinations
and were never treated as a byte oracle.

No new original-TU boundary is established. The existing string-library note
already records this function's two calls to `func_80017F30`; those remain
cross-container usage evidence rather than same-TU membership.
