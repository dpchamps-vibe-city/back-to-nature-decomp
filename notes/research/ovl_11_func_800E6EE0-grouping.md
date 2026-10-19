# ovl_11_func_800E6EE0 grouping evidence

Verified handoff identity:
`33f2e50ed09b99ba587ef978d9eef626534776718b9f25f43cc6baaa319e1e27`.
The supplied handoff reports byte identity and a passed full finalize gate;
its changed-file list contains only
`src/overlays/ovl_11/ovl_11_func_800E6EE0.c`. This assessment changes notes
only and does not rerun matching.

## Candidate membership

Add `ovl_11_func_800E6EE0` to the existing `D_80129560` accessor family,
with a low-confidence local same-TU pair with `ovl_11_func_800E6EAC`:

- `src/overlays/ovl_11/ovl_11_func_800E6EE0.c` optionally resolves an item
  ID through the low signed halfword of a `D_80129560` slot, calls
  `ovl_11_func_800EEC1C`, and returns whether its result is nonzero.
  `include/globals_override.h` records the shared overlay-private 0x50-byte
  s32 table and its sibling accessors.
- `build/ovl_11/functions/ovl_11_func_800E6EE0.s` confirms the same absolute
  table base, signed-halfword slot index with four-byte stride, and `lh`
  at 0x800E6F10. This is another reader of the existing global cluster,
  not a new table or a claim that the entire accessor band is one TU.
- `src/overlays/ovl_11/ovl_11_func_800E6EAC.c` forwards
  `(0, arg0, arg1, 0)` to the target. Its extracted assembly at
  `build/ovl_11/functions/ovl_11_func_800E6EAC.s` confirms the direct call
  at 0x800E6EC8 and the 0x34-byte extent ending exactly at 0x800E6EE0.
- `configs/splat/ovl_11.yaml` places the wrapper and target consecutively
  at file offsets 0x2F08C and 0x2F0C0. Thus call graph and gapless link
  order agree on the local pair; the wrapper does not itself read the table.

## Limits

The table is overlay-private, not proven TU-private, and the local call/link
pair supplies no shared-static, GP-ownership or declaration-order witness.
Candidate membership does not establish an original TU boundary. The distant
`800EEC1C` callee and `800FDFA8` caller are dependencies, not grounds to merge
their recorded runs with this pair. No shared compiler quirk is inferred
from the reconstructed prototypes.
