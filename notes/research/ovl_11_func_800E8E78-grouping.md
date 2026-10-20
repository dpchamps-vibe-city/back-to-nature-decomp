# ovl_11_func_800E8E78 grouping evidence

Verified handoff source/evidence identity:
`cbdabc710070067397bc96c5011ba6181bd00bd429c49bd0daa6d08553ced9b1`.
The supplied handoff reports byte identity and a passed full finalize gate;
this is a notes-only assessment, not a new verification run.

## Candidate membership

Add the target to the existing `D_80129560` accessor family, with a
low-confidence local same-TU tie to `ovl_11_func_800E8F78`:

- `src/overlays/ovl_11/ovl_11_func_800E8E78.c` reads three `D_80129560`
  slots in mode 1 and writes the position triple `D_80071B00[0..2]`.
  Mode 0 supplies raw coordinates; mode 2 passes that same triple to
  `ovl_11_func_800CD0C0` and calls `ovl_11_func_800E9284`.
- `src/overlays/ovl_11/ovl_11_func_800E8F78.c` likewise resolves arguments
  through `D_80129560` and updates `D_80071B00[0]`/`[2]`. Thus the target
  shares both the overlay-private selector table and the exact position
  storage with its immediate successor, not merely the broad state-buffer
  base. Both declarations are in `include/globals_override.h`.
- `configs/splat/ovl_11.yaml` places the target at file offset `0x31058`
  and its successor at `0x31158`. The target's original `0x100`-byte extent
  in `build/ovl_11/asm/nonmatchings/ovl_11_func_800E8E78/ovl_11_func_800E8E78.s`
  ends exactly at `0x800E8F78`, corroborating the shared-global cluster.
- The immediate predecessor, `ovl_11_func_800E8DC8`, also reads three
  `D_80129560` slots to write a position triple. Its verified source and
  the splat offsets `0x30FA8`/`0x31058` corroborate the target's placement
  within the already-recorded accessor family.

## Limits

This is candidate membership, not proof of an original TU boundary. The
position storage is resident shared state, and overlay-private data need
not have C static linkage. Neither neighbour calls the target, and there
is no shared-static, GP-ownership or declaration-order witness. The target's
calls establish dependencies, not same-TU membership with the callees;
do not merge the wider accessor band on this evidence alone.
