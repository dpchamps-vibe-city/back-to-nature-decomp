# ovl_11_func_800E537C: candidate adjacent setup/reset pair

## Status and scope

`ovl_11_func_800E537C` is byte-exact and passed the full finalize gate per
handoff, with verified source/evidence identity
`cac44260a97016ef85975616d96ff6a9f876e25b387f39a5c072d6a8151ab577`.
This is a documentation-only assessment, not another matching attempt.
`ovl_11_func_800E5294` remains an assembly stub; its behaviour below is read
from the original instructions, not inferred from a reconstructed C body.

## New grouping evidence

The existing D_8006C838 flags/state-buffer entry already names both functions
as callers of `ovl_11_func_800E63C8`. The additional evidence is their
corroborated link adjacency and complementary use of the same state:

- **Gapless link order:** `configs/splat/ovl_11.yaml` places 800E5294 at
  file offset 0x2D474 and 800E537C at 0x2D55C. The original predecessor's
  length is 0xE8, so 0x2D474 + 0xE8 = 0x2D55C, equivalently
  0x800E5294 + 0xE8 = 0x800E537C.
- **Shared state cells:** both access the s32 flag word at
  `D_8006C838 + 0x5234` and clear bits of the s32 at `+0x4450`.
  800E5294 tests the former with mask 0x20000010 and clears 0x2000 in
  the latter; 800E537C clears 0x100000 in the former (with a further
  conditional update) and clears bit 1 in the latter.
- **Shared call pattern:** the first call is `func_8001AF70(3, 1)` in
  800E5294 and `func_8001AF70(3, 0)` in 800E537C. Both also call
  `ovl_11_func_800E63C8`: conditionally with `(0, 0, 0, 0)` in the
  predecessor and unconditionally with `(1, 0, 0, 0)` in the target.
  These are observed argument-register values, not a claim about the
  callee's number of meaningful parameters.

This supports a **low-confidence candidate same-TU setup/reset pair** within
this local span. The ledger records the pair under its existing
D_8006C838 flags/state-buffer cluster rather than creating a competing
section.

## Roles

- `ovl_11_func_800E5294` — candidate setup sibling: selects `(3, 1)`,
  conditionally updates flags through 800E63C8, and updates work-area state.
- `ovl_11_func_800E537C` — candidate reset sibling: selects `(3, 0)`, resets
  shared status/flag fields, and conditionally dispatches follow-up calls.

## Limits

D_8006C838 is shared main-binary storage, not a demonstrated private static.
There is no direct call between the pair, and their shared callee is not
link-adjacent. Neither shared calls nor adjacency alone proves original
translation-unit boundaries. This evidence does not place every member of
the scattered flags/state-buffer cluster in one TU, establish ownership of
D_8006C838, or extend the pair to 800E5230 or 800E549C. The new
`Ovl11ResetState` view documents accessed fields; it does not establish an
original shared C struct declaration.

## Evidence paths

- Verified C: `src/overlays/ovl_11/ovl_11_func_800E537C.c`.
- Target original instructions:
  `build/ovl_11/functions/ovl_11_func_800E537C.s`.
- Predecessor original instructions:
  `build/ovl_11/functions/ovl_11_func_800E5294.s`.
- Link-order configuration: `configs/splat/ovl_11.yaml`, entries at
  0x2D474 / 0x2D55C / 0x2D67C.
- Verified field view: `struct Ovl11ResetState` in
  `include/globals_override.h`.
