# ovl_11_func_800F4D28: adjacent pool-record transition caller

## Provenance and evidence

The agent handoff reports a byte-exact result and a passed full finalize gate,
with verified source/evidence identity
`138ec947b4201784e0e8354f52398b12e29bab3a7ab528468e34488031a8975c`.
Its changed-file list names `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800F4D28.c`. This documentation pass did not
rerun the gate or change build inputs.

Evidence read:
- `src/overlays/ovl_11/ovl_11_func_800F4D28.c` (SHA-256
  `3c3ef9670032ca6e4f752f3b9cba60a6ffe9652f438e68e5a165e9cc70d86bb5`)
  and `Ovl11SpawnProbeState` / `Ovl11PoolGateState` in
  `include/globals_override.h`.
- `build/ovl_11/asm/nonmatchings/ovl_11_func_800F4D28/ovl_11_func_800F4D28.s`:
  original gate/record accesses and direct calls.
- `src/overlays/ovl_11/ovl_11_func_800F4CAC.c`,
  `src/overlays/ovl_11/ovl_11_func_800F4CEC.c` and their original assemblies
  under `build/ovl_11/functions/`: the two adjacent transition leaves.
- `src/overlays/ovl_11/ovl_11_func_800F4E84.c` and
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800F4E84/ovl_11_func_800F4E84.s`:
  adjacent probe caller; `src/overlays/ovl_11/ovl_11_func_800F4FC8.c`:
  shared completion leaf.
- `configs/splat/ovl_11.yaml`, offsets `0x3CE8C` through `0x3D1FC`:
  corroborating link order, not recovered TU boundaries.
- [Earlier pool-entry grouping evidence](ovl_11_func_800F4E84-grouping.md).

## New grouping evidence

The original sizes give the gapless chain
`800F4CAC + 0x40 = 800F4CEC`,
`800F4CEC + 0x3C = 800F4D28`,
`800F4D28 + 0x15C = 800F4E84`, and
`800F4E84 + 0x144 = 800F4FC8`.
The verified function calls its immediate predecessor `800F4CEC` at
`800F4DE8`, the preceding sibling `800F4CAC` at `800F4E50`, and the existing
completion leaf `800F4FC8` at `800F4E60`. Thus the new membership tie is not
address adjacency alone.

`800F4D28` tests bit `0x800` of `D_8006C838+0x44F8` and follows the pool
pointer at `D_8006C838+0xDDD4` before setting bit 0 of the current record's
halfword at +4. Its immediately following sibling `800F4E84` tests the same
bit and reads the same slot in its current-record probe path. Both call
`800F4CEC` for a current-record transition and `800F4FC8` for completion,
although their probe inputs and conditions differ. The original slot loads
use the same base-plus-`0x8000`, displacement-`0x5DD4` construction.

The two preceding leaves share the more specific output global
`D_80129630`: `800F4CEC` publishes the current record, while `800F4CAC`
publishes the next 0x18-byte record. They initialize the same +0/+8/+C/+10
fields with different position constants. Their shared output, adjacency,
and newly verified common caller support including both in the local run.
Previously `800F4CEC` was only supporting call evidence for the documented
`800F4E84`/`800F4FC8` pair. The verified caller supplies its direct
call/link-adjacency tie and connects the paired transition leaves to that
existing candidate group.

## Limits

A medium-confidence local same-TU hypothesis now spans the five functions
from `800F4CAC` through `800F4FC8`, conditional on this linked transition/
probe/completion sequence sharing an original source file. A TU boundary
could still split the run. No shared static, TU-owned definition,
declaration-order effect, register-variable quirk or original filename was
established. Modern view typedefs are not original-TU evidence.

The earlier `800F4994`/`800F4A58` pair remains connected only by the existing
low-confidence shared-pool tie; this does not establish membership for the
intervening functions. No merger with the separate pool-walk or
spawn-record writer families is claimed.
