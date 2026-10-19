# ovl_11_func_8011B6C4 — candidate two-state helper pair

## Provenance

Notes-only review of the verified
[source](../../src/overlays/ovl_11/ovl_11_func_8011B6C4.c).
Supplied source/evidence identity:
`283dec39bdb147c03438fb9bf723fc861d435019f413355db215fb64a4610574`
(origin: agent; byte-exact and full finalize gate reported passed).
The supplied changed-file list names only that source. No matching or build
verification was rerun for this documentation review.

## Candidate membership evidence

A low-confidence local same-TU pair is supported with the immediate successor
[8011B784](../../src/overlays/ovl_11/ovl_11_func_8011B784.c):

- **Shared state/status idiom:** both take `(s32 *state, s32 base, s16 id)`,
  reject unsigned `*state - base >= 2`, perform an ID-dependent start operation
  in case 0, advance to case 1, and poll `D_80071A6C` for completion. Both
  restore `base + local_state` and return the completion flag. B6C4 starts
  item insertion through `800E6F34`; B784 constructs a three-halfword record
  through `800D7338` and submits it through `800CB540`. Their completion
  masks differ. The original helper assemblies corroborate the common shape
  and status reads at 0x8011B748 and 0x8011B804.
- **Gapless helper order:** original assembly extents give
  `8011B6C4 (0xC0) → 8011B784 (0xBC) → 8011B840`.
  [The split configuration](../../configs/splat/ovl_11.yaml) records the
  corresponding offsets 0x638A4, 0x63964 and 0x63A20.
- **Call/link order corroboration:** consecutive clients
  `80118CE4 (0x324) → 80119008` call B6C4 at 0x80118D80 and B784 at
  0x801192E0 respectively. Thus the adjacent helper variants are used by
  adjacent interaction handlers in the same order, rather than linked solely
  by a common status-word read. The config places those clients at offsets
  0x60EC4 and 0x611E8; original assemblies confirm the calls.

Original assembly evidence is under
`build/ovl_11/asm/nonmatchings/<function>/<function>.s` for both helpers and
both clients. `build/ovl_11/functions.csv` corroborates their sizes and edges.

## Limits

The hypothesis is conditional on the combined local ordering, shared
start/wait template and client ordering reflecting source organization rather
than neighboring TUs. `D_80071A6C` aliases the widely used
`D_8006C838 + 0x5234` status word, as already recorded in the grouping ledger;
it is not private-static or data-ownership evidence. No declaration-order,
register-variable or SDK fingerprint is established.

Neither outer TU boundary is proven. Do not merge this pair with the preceding
`D_8012D52C` reset-stub family, the following six-state B840/BAFC pair, the
clients, or the distant start-operation callees merely from these links.
