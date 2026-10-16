# ovl_11_func_800FC054: state-control run extension

## Verified handoff

The handoff reports `ovl_11_func_800FC054` byte-exact and through the full
finalize gate, with source/evidence identity
`04a64a9213c157f3779e688503defc91fb1abbbadde9c7c7b9f376c9749d1396`.
The verified source is `src/overlays/ovl_11/ovl_11_func_800FC054.c`.
This notes-only pass did not repeat the matching gate.

## Existing evidence and new extension

The ledger already groups `800FBE4C` / `800FC320` at low confidence: both
write the overlay-local s16 `D_8012720C` and call
`func_800132F0(10, 0, 2)`. Their sources are
`src/overlays/ovl_11/ovl_11_func_800FBE4C.c` and
`src/overlays/ovl_11/ovl_11_func_800FC320.c`; the latter also calls
`func_8001FABC(3)`. The earlier pair alone did not establish link adjacency.

The verified controller supplies the missing call/link bridge:

- `build/ovl_11/functions.csv` gives the gapless run
  `800FBE4C` (0x3C) → `800FBE88` (0x1CC) → `800FC054` (0x158) →
  `800FC1AC` (0x174) → `800FC320` (0x38).
  `configs/splat/ovl_11.yaml` corroborates the consecutive entries at offsets
  `0x4402C`, `0x44068`, `0x44234`, `0x4438C` and `0x44500`.
- The target's original assembly,
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800FC054/ovl_11_func_800FC054.s`,
  calls its immediate predecessor `800FBE88` at `800FC100` and its immediate
  successor `800FC1AC` at `800FC138`. Its `D_8012720C` reads and writes
  implement states 1–6, including a reset to zero on exit.
- `src/overlays/ovl_11/ovl_11_func_800FBE88.c` sets that same state to 1
  after calling the display-state initializer `800FB908`, then loads and
  initializes sprite resources. The target calls this setup in state 2 and
  subsequently sets state 3.
- `src/overlays/ovl_11/ovl_11_func_800FC1AC.c` calls its own immediate
  successor `800FC320` on an input mask. That successor sets
  `D_8012720C = 5`, which the target handles as the exit-transition phase.
  Thus the frame driver participates in the same state machine, rather than
  being included solely for adjacency.

## Grouping conclusion and limits

Extend the existing **low-confidence candidate same-TU group** to include
`800FBE88`, `800FC054` and `800FC1AC`. The new evidence is the controller's
calls to both gapless neighbours, corroborated by their shared state-control
paths. This does not establish the original TU boundaries or a shared static,
declaration-order effect or original register-variable declaration.

`800FB908` already appears with `800FBE4C` / `800FBE88` in the ledger's
text-draw family, and `800FC1AC` already appears in the `D_80127328` dispatch
family. These are candidate bridges, not proof that those entire families
belong to one TU. Do not extend membership to the target's distant caller
`800F65B8` or exit callee `800BCF28` merely because of their call edges.
The switch-table extraction in the handoff's splat change identifies the
target's rodata, not an original source-file boundary.
