# ovl_11_func_80113818: constant-table cluster membership

## Verified anchor

The handoff reports byte identity and the full finalize gate passed for
source/evidence identity
`8a66702b0415c07a10a9ce8e3c52d616b55d3f3ce9a4d7b9b1866da2ebf1448e`.
Its changed files are `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_80113818.c`. This notes-only review accepts
that gate; it does not rerun matching or change build inputs.

## New grouping evidence

- **Code adjacency and initialized-data adjacency agree.** The original
  `build/ovl_11/asm/nonmatchings/ovl_11_func_80113818/ovl_11_func_80113818.s`
  gives a size of 0x208: `0x80113818 + 0x208 = 0x80113A20`, the existing
  constant-table cluster member. `configs/splat/ovl_11.yaml` likewise places
  the functions consecutively at offsets 0x5B9F8 and 0x5BC00. In
  `build/ovl_11/asm/data/69960.data.s`, that neighbour's eight-halfword
  `D_80128138` table ends at 0x80128148, exactly the start of this function's
  `D_80128148` data (ending at 0x801281D0).
- **The new slice has a local, exclusive reader.** An exact-symbol search of
  `build/ovl_11/functions/ovl_11_func_*.s` finds `D_80128148` only in
  80113818, at 80113878/8011387C. The verified source selects that base or
  base +0x34 for `func_8001AC10`; it therefore supplies a newly witnessed
  trailing slice of the ledger's existing D_80127F88–D_80128138 cluster.
- **Their state roles are complementary.** At 801138E4/801138F8 the target
  increments the initial halfword of `D_8012D110` on a successful 0x8000
  trigger. The immediate successor reads that halfword as its weighted-table
  row index (see `src/overlays/ovl_11/ovl_11_func_80113A20.c` and the original
  `build/ovl_11/functions/ovl_11_func_80113A20.s`). This supports the local
  connection, but the shared absolute state buffer alone is not TU evidence.

## Membership inference and limits

Candidate membership in the existing constant-table cluster, **medium
confidence**, based on the combined gapless code/data adjacency and exclusive
slice reader. The premise is that these aligned local code and data runs
reflect one original source unit; separate adjacent linked units remain
possible. No TU boundary, static linkage, declaration-order effect or
register-variable quirk is established.

There is no direct call between 80113818 and 80113A20. Original callers are
800C162C at 800C1690 for the target and 80113310 at 8011363C for its successor
(`build/ovl_11/functions/ovl_11_func_800C162C.s` and
`build/ovl_11/functions/ovl_11_func_80113310.s`). The target's call to
80112D10 and its broader D_8012D110 relationship were already recorded in the
ledger; they do not newly merge the initializer, grid-trigger pair or
record-feed/clear pair into this candidate TU.
