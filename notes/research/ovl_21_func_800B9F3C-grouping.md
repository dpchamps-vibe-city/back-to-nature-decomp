# ovl_21_func_800B9F3C — projectile-slot initializer

## Provenance

The handoff reports byte-exact source and a passed full finalize gate, with
source/evidence identity
`40e01170c3954c49ac74331233dc421959acc35edbadc6e993eca0124de02dc5`.
Its changed files are `include/game_types.h`, `include/globals_override.h`,
`include/overlays/ovl_21.h`, and
`src/overlays/ovl_21/ovl_21_func_800B9F3C.c`. This documentation does not rerun
matching or change build inputs.

## New grouping evidence

- The verified source initializes free projectile slots within the same six
  `D_800C0448` records used by existing cluster members: outer stride 0x108,
  inner stride 0x4C, active word at record +0x34, and inner bound from
  `D_800C0448 + 0x64A`. It writes active = 1 where
  `src/overlays/ovl_21/ovl_21_func_800B9844.c` counts active slots;
  `src/overlays/ovl_21/ovl_21_func_800B836C.c` clears those slots during
  initialization. This establishes a producer/initializer/consumer link over
  the specific sub-records, not merely an overlay-wide global reference.
- Its launch gate uses record +0x18 == 2 and bit 0x100 in the halfword at
  `record->unk30 + 2`. Existing member
  `src/overlays/ovl_21/ovl_21_func_800BA510.c` uses that same state/animation
  gate and the same record position triple for projection.
- `configs/splat/ovl_21.yaml` places it immediately after the existing member
  `ovl_21_func_800B9CA4` (0x800B9CA4 + 0x298 = 0x800B9F3C), and before
  `ovl_21_func_800BA300` (0x800B9F3C + 0x3C4 = 0x800BA300). The extracted
  target at
  `build/ovl_21/asm/nonmatchings/ovl_21_func_800B8B3C/ovl_21_func_800B8B3C.s`
  conditionally calls 800B9F3C at 0x800B8E44, then calls 800BA300 at
  0x800B8E4C and existing member 800B990C at 0x800B8E54. The consecutive
  launch/update calls corroborate the launcher's local link-order placement.

## Membership conclusion and limits

Add only `ovl_21_func_800B9F3C` to the existing `ovl_21` D_800C0448 cluster,
with the role projectile-slot initializer. Under that cluster's same-TU
hypothesis, the shared slot lifecycle and animation gate, backed by local
link/call order, support membership at the existing medium confidence.
Shared storage and calls can cross TUs: this establishes no new TU boundary,
data ownership, or additional neighbour membership.
