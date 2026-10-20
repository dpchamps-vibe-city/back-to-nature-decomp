# ovl_11_func_8011BD6C — candidate interaction-handler run extension

## Provenance

Notes-only review of the verified source
`src/overlays/ovl_11/ovl_11_func_8011BD6C.c`. The handoff reports a byte-exact
function and passed full finalize gate, with source/evidence identity
`68b66bbf1ea6c1002ebf5bacb28e35fc768858f599f3be3396720500c9a67996`
(origin: agent); its changed-file list names `configs/splat/ovl_11.yaml` and
the target source. No new matching or build-gate claim is made here.

## New grouping evidence

A **low-confidence candidate extension** of the existing B840/BAFC pair is
supported by combined local ordering and caller/data-flow evidence:

- **Text and jump-table order agree.** Original assembly sizes give
  `8011B840` (0x2BC) → `8011BAFC` (0x270) → `8011BD6C` (0x52C), ending
  at `8011C298`, with no intervening text. Their jump tables are likewise
  consecutive: `jtbl_800BB4E8` (six words) → `jtbl_800BB500` (six words)
  → `jtbl_800BB518` (15 words). Original assemblies are under
  `build/ovl_11/asm/nonmatchings/<function>/<function>.s`;
  `configs/splat/ovl_11.yaml` corroborates text offsets
  0x63A20/0x63CDC/0x63F4C and rodata offsets 0x36C8/0x36E0/0x36F8.
  The reconstruction's per-function splits do not prove original ownership.
- **A shared caller connects the selected-record API.** In the original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80119DF8/ovl_11_func_80119DF8.s`,
  the descriptor at caller stack +0x60 has `D_8012D548` as its first word
  (store at 0x80119E80). Calls to B840 at 0x80119FA8 and 0x8011A094 pass
  that descriptor or its companion by value. The BD6C call at 0x8011A170
  passes the address of the first descriptor in `$a2` (delay slot
  0x8011A174). The verified `StructD548 **arg2` and `8011D98C(**arg2)`
  read the same selected record, corroborating a local handler family rather
  than unrelated adjacent routines. `build/ovl_11/functions.csv` also
  records the shared caller edges.
- **The verified source fits the offset-state handler idiom.** Like B840
  and BAFC, BD6C dispatches on `*arg0 - arg1`, rejects out-of-range states
  with -1, waits for resident message-status values 2/5, advances via
  `8011D098`/`8011D0B4`, and returns 1/3 for completion/cancellation.
  Its by-value `8011D98C` record accessor is also used by BAFC.
  Comparison sources are the B840 and BAFC `.c` files in
  `src/overlays/ovl_11/`.

The original pair's independent shared-caller evidence remains in
[the B840 note](ovl_11_func_8011B840-grouping.md).

## Limits

The extension is conditional on combined text/rodata order, shared caller
record flow and handler idiom reflecting source organization, rather than
neighbouring TUs using the same API. BD6C has 15 states and confirmation/scene
transition work, not the pair's six-state selection template; it is a candidate
extension, not another identical variant. No shared private static, TU-owned
global, declaration-order effect or register-variable quirk is established.
`D_8012D52C` is already known to be container-wide and adds no independent
ownership evidence.

Neither outer boundary is proven. The caller's record flow does not establish
ownership by the handler or justify adding the distant caller/record-helper
groups. BD6C's adjacency and economy calls alone do not justify merging the
separately recorded C298/C6CC transaction pair.
