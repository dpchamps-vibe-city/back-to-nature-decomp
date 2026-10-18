# ovl_11_func_800D9108: main-grid threshold search grouping

## Verified anchor

The supplied handoff reports byte identity and a passed full finalize gate for
source/evidence identity
`2b4ec64e2547ec687e6cc89cc89bf0294648d67c3539de07a9788b855058026f`.
Its changed-file list contains only
`src/overlays/ovl_11/ovl_11_func_800D9108.c`. This review changes notes only
and relies on that verification rather than rerunning matching.

## New membership evidence

- **Same concrete grid and record-state helper pattern.** The verified source
  searches `D_80071DFC[25][45]`, testing tag +0 against 0x36 and unsigned
  byte +4 against the signed-halfword result of `800D5C3C(0x36)`, then
  calls `800DACD4(row, col, 0)` on the first qualifying entry. Existing
  cluster member `src/overlays/ovl_11/ovl_11_func_800D85F8.c` likewise
  compares byte +4 with `800D5C3C(entry tag)` as part of its state-update
  pipeline. The original target assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D9108/ovl_11_func_800D9108.s`
  corroborates the threshold lookup at 800D9120, record tests at
  800D9200–800D921C and 800D9284–800D92A0, update call at 800D92B4,
  eight-byte column stride and 0x168-byte row stride. This ties the search
  to the existing entry-state operations, not merely a reconstructed type.
- **Immediate predecessor is already a walker over that grid.**
  `src/overlays/ovl_11/ovl_11_func_800D9064.c` applies the existing
  record-tag update trio (`800DA49C`, `800DA518`, `800DA454`) to every
  entry of the same 25×45 table. Its original assembly under
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800D9064/` has size 0xA4,
  ending exactly at 800D9108. `configs/splat/ovl_11.yaml` agrees:
  offsets 0x21244 → 0x212E8. Link adjacency therefore corroborates the
  shared grid/state work; no direct call between these neighbours is claimed.

## Inference and limits

Add 800D9108 to the existing Ovl11D124Entry table cluster, retaining its
**low-confidence candidate-TU** status. The conditional premise is that the
neighbouring main-grid walkers and shared record-state helper pattern reflect
one source unit. The table and helpers are not proven TU-private, so the
calls can cross translation-unit boundaries and adjacency is not conclusive.
No shared static, declaration-order effect, register-variable quirk or exact
TU boundary is established. The target ends at 800D92FC, but that adjacency
alone does not merge the following bounds/cell-sprite run. Indirect access
through 800DACD4 does not add the target to the callback-table cluster.
