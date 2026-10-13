# ovl_28_func_800B86D8 — slot 8 shared-state consumer evidence

## Provenance

The agent-origin documentation handoff reports byte-exact source and a passed
full finalize gate, with source/evidence identity
`bd58f4781fef182e8dadb0f44005a86e2abb6aa9083e6121620edd2422eafc29`.
Its changed build inputs are `include/overlays/ovl_28.h` and
`src/overlays/ovl_28/ovl_28_func_800B86D8.c`. This note reads the verified
body and neighbouring evidence; it does not rerun matching or alter build inputs.

## New grouping evidence

- `src/overlays/ovl_28/ovl_28_func_800B86D8.c` gates on `D_800B93C0`,
  consumes and updates `D_800B93C4`/`C6`/`C8`/`CA`/`CE`, and reads
  `D_800B93CC` as the row index for `D_800B93D0`.
  `src/overlays/ovl_28/ovl_28_func_800B865C.c` initializes this entire
  cluster; `src/overlays/ovl_28/ovl_28_func_800B7E80.c` also resets it.
  The ledger recorded the setup, but not this handler's consumption of the
  cluster, supplying a setup/consumer tie to the adjacent slot 8 initializer.
- The handler reads `D_800B93B8` before selecting the next table entry and
  `D_800B93C0` before running its state machine.
  `src/overlays/ovl_28/ovl_28_func_800B8478.c` clears the former on command
  0xF1 and sets/clears the latter on commands 0xF2/0xF3. The former is also
  seeded by `src/overlays/ovl_28/ovl_28_func_800B8414.c` and `800B7E80.c`.
  These are new concrete consumer links to the slot 7 command handler and
  its setup, beyond the previously recorded callback-table relationship.

## Membership conclusion and limits

`configs/splat/ovl_28.yaml` places slot 8 setup at offset 0x83C and this
handler at 0x8B8: the setup's 0x7C-byte body ends exactly at the handler.
The handler occupies 0x284 bytes through offset 0xB3C, where the already
referenced record-table initializer `ovl_28_func_800B895C` begins.
Together with the new shared-state consumer links, this supports adding
`ovl_28_func_800B86D8` to the existing `D_800B95F8` handler-pointer-table
group, conditional on its adjacency-based translation-unit hypothesis.

The installation of this handler into `D_800B9618` by `800B865C.c` was
already recorded and is not counted as new evidence. No new translation-unit
boundary is established; confidence remains medium because shared globals and
callbacks can cross translation units. The table `D_800B93D0` and calls to
other helpers alone do not establish their same-file membership.
