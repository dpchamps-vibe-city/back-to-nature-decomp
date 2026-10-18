# ovl_11_func_80104A90 — grouping evidence

Verified-source/evidence identity supplied by the handoff:
`cbee2e2eeb49c526086385f5c3b0994ca2a80380f21eaa0edad108b4f30d0330`.
The handoff reports byte identity and a passed full finalize gate; this is
notes-only documentation, not a new matching or verification claim.

## New membership evidence

The verified `src/overlays/ovl_11/ovl_11_func_80104A90.c` supplies another
initializer in the existing `D_801287F4` counter-state family:

- Like documented candidate `ovl_11_func_801048A0`, it sets
  `D_801287F4 = 10`, `D_8006C838+0x4488 = 1`, and
  `D_8007AFF0+0x2548C = 0`. Both reset the far-state words at
  +0x25498/+0x2549C to -1 and +0x25390 to zero, the halfwords at
  +0x2479C/+0x247A6/+0x247A8 to -1, and the words at
  +0x25484/+0x25488 to 0xFFFF. Both clear flag 0x800000 at
  `D_8006C838+0xC` and reset its +0x4438 halfword. These are a shared
  initialization cluster, not merely use of the ubiquitous base pointers.
- `build/ovl_11/asm/data/69960.data.s` records the original
  `D_80121780` table: entries 0/1 are the existing cluster's
  `800BC3AC`/`800BC3E0`, entry 4 is `801048A0`, and entry 5 is
  `80104A90`. Entry 7 is the wrapper `80104EA8`, whose matched source
  calls `80104A90` directly. This connects the new initializer to the
  same dispatch family and its local wrapper.
- `configs/splat/ovl_11.yaml` places `80104A90` (0x228 bytes) immediately
  before `80104CB8` (0x1F0), then wrapper `80104EA8`. Table entries
  4–7 follow the local function address order. `801048A0` is nearby,
  but three functions (`80104A58`, `80104A80`, `80104A88`) intervene;
  the two initializers are not immediate link neighbours.

## Limit

This supports candidate membership in the existing counter-state family,
particularly with initializer `801048A0`. The counter's main run at
0x800BC3AC–0x800BCD28 is address-apart. Shared dispatch membership and
reset state do not prove private linkage or a common original translation
unit; no TU boundary, global ownership, or merger of the intervening code
is established. The ledger therefore records only a candidate member and
its area-entry initialization role.
