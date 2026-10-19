# ovl_11_func_800D4314: keyframe-state handler run

## Verified premise

The supplied agent handoff records byte identity and the full finalize gate
passing for evidence identity
`c6858e3f8605ff9cbed373eeb09161948ecb665cca7766b8299105009bba9010`.
The source read at `src/overlays/ovl_11/ovl_11_func_800D4314.c` has SHA-256
`e35e6b1ed7f13c22542fc672aebabfe467c12b1089f82ee98bfee6869617fd69`.
This documentation does not rerun the solver or change build inputs.

## Membership evidence

Candidate same-TU run: `ovl_11_func_800D4314`, `ovl_11_func_800D4558`
and the already-grouped `ovl_11_func_800D4624` (high confidence for the local
run, not a proven original translation-unit boundary).

- **Gapless code order:** `configs/splat/ovl_11.yaml` places the three at
  offsets `0x1C4F4`, `0x1C738` and `0x1C804`: the target's `0x244` bytes end
  at `800D4558`, whose `0xCC` bytes end at `800D4624`.
- **Matching table order:** `build/ovl_11/asm/data/69960.data.s` has the
  target's six-word `D_80123BD4` ending at `D_80123BEC`, followed immediately
  by `D_80123BF8`, the respective tables of those two neighbours.
- **Shared handler/callee pattern:** the verified target and neighbour source
  `src/overlays/ovl_11/ovl_11_func_800D4558.c` share the state/flag object view,
  the `800D04D4(obj, table, &state, -1, &timer)` progression call,
  the `800D3200` terminal-state cleanup and `800D0408` velocity helper.
  This corroborates the code/table adjacency rather than relying on
  proximity alone.

## Target role detail

The target steps `D_80123BD4` and responds to collision probes. Initialization
seeds the timer at +0x2A with `func_80012A34(60)`. States 1/2 and 3 probe the
halfword-aligned collision tag at +0x2E through `800BE208`; direction-selection
loops exclude direction 1. States 4/5 call `800D0408` on the +0x48 velocity
view, with state 4 additionally subtracting 16 from +0x4C.

## Limits

The shared layout and helpers alone establish a handler family, not a source
file. Consecutive code and table order support the local same-TU hypothesis;
no shared static ownership, declaration-order effect or register-variable
quirk is established. No TU membership is inferred for the helpers or for all
of the distant handlers in the ledger's broader family.
