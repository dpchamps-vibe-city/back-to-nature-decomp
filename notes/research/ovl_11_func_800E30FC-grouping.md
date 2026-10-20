# ovl_11_func_800E30FC: local keyframe-state handler membership

## Verified premise

The agent handoff records byte identity and a passing full finalize gate for
source/evidence identity
`4e791463cd1f37b358f0fc142f47bff96b3a733ce6b353c7ef3f35d1b3ab287b`.
The supplied changed-file list is `include/globals_override.h` and
`src/overlays/ovl_11/ovl_11_func_800E30FC.c`; the latter's SHA-256 is
`a0e12c280bf77ea105e548ec13ee0f672ea87354a4baf80b352d638059425220`.
This documentation does not rerun matching or change build inputs.

## New membership evidence

The ledger previously mentioned the target only as `800E3020`'s successor.
The verified handler supports a candidate same-TU run with `800E3020` and
`800E3200` (medium confidence for original TU membership):

- **Code and table order agree on both sides.** In
  `configs/splat/ovl_11.yaml`, `800E3020` occupies offsets
  `0x2B200–0x2B2DC`, the target `0x2B2DC–0x2B3E0` (0x104 bytes), and
  `800E3200` starts at `0x2B3E0`. In
  `build/ovl_11/asm/data/69960.data.s`, `800E3020`'s last selected table
  `D_801240A8` ends at the target's `D_801240B0`; that three-word table
  (`0x003C0004, 0x00000001, 0x0000FFFF`) ends at `800E3200`'s `D_801240BC`.
- **Dispatch order corroborates the local run.**
  `build/ovl_11/asm/data/1740.rodata.s` places `800E3020`, the target and
  `800E3200` consecutively at `D_800B970C[2]`, `[3]` and `[4]`.
  `src/overlays/ovl_11/ovl_11_func_800E276C.c` dispatches through this table
  using the object's +0x26 selector.
- **Shared secondary-update idiom.** The verified target and
  `src/overlays/ovl_11/ovl_11_func_800E3200.c` both preserve s16@+0x2A across
  `800D12B8(obj, 0x12, 0x31)` when `800D04D4` returns zero, in the same
  +0x28 state-switch family. The target's extracted assembly at
  `build/ovl_11/asm/nonmatchings/ovl_11_func_800E30FC/ovl_11_func_800E30FC.s`
  confirms the calls and save/restore. Unlike `800E3200`, the target passes
  +0x2C, not +0x2A, as `800D04D4`'s fifth argument.

## Limits

Shared helpers and object layout alone do not establish an original source
file. The candidate membership depends on code, data and dispatch ordering
retaining original layout; no shared-static ownership, declaration-order
effect or original register-variable declaration is established. This evidence
does not merge the distant family members or the shared helpers into this TU.
