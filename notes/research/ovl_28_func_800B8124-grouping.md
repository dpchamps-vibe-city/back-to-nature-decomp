# ovl_28_func_800B8124 — shared-state and callback-consumer evidence

## Provenance

The documentation handoff reports byte-exact source and a passed full finalize
gate, with source/evidence identity
`e42139e8cf9c10b671989527de29d4f8d6a49ec66d12e7e95b8a98ae8e800e18`.
This note reads that verified body; it does not rerun matching or change build
inputs.

## New grouping evidence

- `src/overlays/ovl_28/ovl_28_func_800B8124.c` updates `D_800B9620` and
  dispatches on `D_800B9624`. Both globals are reset by
  `src/overlays/ovl_28/ovl_28_func_800B7E80.c`, providing a setup/consumer
  link within its adjacent state cluster. The ledger previously recorded the
  reset but not this consumer.
- In phase 2, the verified dispatcher calls through both `D_800B9614` and
  `D_800B9618`. `ovl_28_func_800B7E80.c` installs `ovl_28_func_800B8414` and
  `ovl_28_func_800B865C` into those slots; their corresponding source files
  replace them with `ovl_28_func_800B8478` and `ovl_28_func_800B86D8`.
  This identifies the counter-run dispatcher as a consumer of the existing
  handler-pointer-table group, rather than merely another table entry.
- The same phase reads `D_800B93BC`, reset by both
  `ovl_28_func_800B7E80.c` and `ovl_28_func_800B8414.c`, adding another
  shared-state link to those initialisers.

## Membership conclusion and limits

`configs/splat/ovl_28.yaml` places the dispatcher gaplessly between
`ovl_28_func_800B7FD4` and `ovl_28_func_800B8304`. Combined with the new
setup/consumer and indirect-call links above, this reinforces its existing
membership in the counter run and the connected handler-table/initialiser
cluster recorded in `notes/file-groupings.md`.

No new member or translation-unit boundary is established; confidence remains
medium. Shared globals and callbacks can cross translation units, so these
links are corroboration of the existing adjacency-based hypothesis, not proof
on their own. The shared `D_8006C838+0x4488` counter access was already recorded
and is not counted as new evidence.
