# ovl_11_func_8011C6CC — adjacent transaction-handler candidate

## Provenance

Notes-only review of the verified source
`src/overlays/ovl_11/ovl_11_func_8011C6CC.c`. The handoff reports byte-exact
source and a passed full finalize gate, with source/evidence identity
`1858b08ff99bdeb01698565cdd4c863866a93626c7aea801c78c4179675bc063`
(origin: agent). Its changed-file list names `configs/splat/ovl_11.yaml`,
`include/globals_override.h` and the target source. No matching experiments
or build-input edits were performed in this review.

## Grouping evidence

The verified body corroborates a **low-confidence candidate same-TU pair**
with its immediate predecessor, `ovl_11_func_8011C298`:

- **Consecutive text and jump tables.** Original assembly sizes give
  `8011C298` (0x434 bytes) → `8011C6CC` (0x4FC bytes), ending at
  `8011CBC8`. Their switch bounds and table references identify the
  consecutive tables `jtbl_800BB558` (14 words, 0x38 bytes) and
  `jtbl_800BB590` (17 words, 0x44 bytes). Evidence is in
  `build/ovl_11/asm/nonmatchings/ovl_11_func_8011C298/ovl_11_func_8011C298.s`
  and the corresponding `ovl_11_func_8011C6CC` assembly. Text offsets
  0x64478/0x648AC and rodata offsets 0x3738/0x3770 in
  `configs/splat/ovl_11.yaml` corroborate the order; the reconstruction's
  per-function splits do not establish original TU ownership.
- **Shared state and transaction idiom.** Both C sources clear
  `D_8006C838` offsets +0x522C/+0x5230, set bit 0x800000 at +0x5234,
  dispatch on `D_8012D524`, write `D_8012D52C`, and clear that bit on
  completion. Both advance through `8011D084`/`8011D0B4`, check a cost
  with `8011D06C`, and debit it through `800F2354(-cost, 1)`, with
  resident message/status calls around the transitions. This shared
  source-level pattern and helper-call graph corroborate the local order;
  the C298 source is `src/overlays/ovl_11/ovl_11_func_8011C298.c`.

## Limits

The hypothesis depends on the combined local text/rodata ordering and shared
transaction template reflecting source organization rather than neighbouring
TUs implementing the same API. These are shared overlay globals, not proven
private statics; in particular, `D_8012D52C` is already known to be
container-wide. No TU ownership, register-variable quirk or declaration-order
effect is established, and neither outer boundary is proven.

C6CC's calls to the already documented B840/BAFC pair were recorded in
`notes/research/ovl_11_func_8011B840-grouping.md`; those calls alone are not
new grouping evidence and do not justify merging that pair or the distant
state/record-helper groups into this candidate.
