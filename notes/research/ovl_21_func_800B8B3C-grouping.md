# ovl_21_func_800B8B3C — state-7 update handler

## Provenance

The handoff reports byte-exact source and a passed full finalize gate, with
source/evidence identity
`c3cf03b4a7dd0b11b8556322dbcdded78c8f83f8bbae3688baec940831624949`.
Its changed files are `include/globals_override.h`,
`include/overlays/ovl_21.h`, and
`src/overlays/ovl_21/ovl_21_func_800B8B3C.c`. This documentation does not rerun
matching or change build inputs.

## New grouping evidence

- The verified source and its immediate link successor,
  `src/overlays/ovl_21/ovl_21_func_800B8F98.c`, share the same update sequence:
  calls to 800BA300 and established member 800B990C, a six-record cooldown
  loop, then a call to established member 800BA4C0. Both loops operate on
  `D_800C0448` with stride 0x108: state 7 at record +0x18 increments the
  halfword at +0x2E, then resets that counter and selects state 9 at 30.
  `src/overlays/ovl_21/ovl_21_func_800B990C.c` supplies the corresponding
  state-7 write and counter reset. This is a shared producer/consumer
  lifecycle and repeated local update idiom, not just a reference to an
  overlay-wide global.
- The link predecessor,
  `src/overlays/ovl_21/ovl_21_func_800B8A80.c`, seeds the six record states,
  initializes the countdown at `D_800C0448 +0x8`, and selects outer state 7.
  The target consumes that countdown and the periods at +0x648/+0x64E/+0x650
  initialized by `src/overlays/ovl_21/ovl_21_func_800B8228.c`; its completion
  path selects outer state 8, handled by 800B8F98.
- `configs/splat/ovl_21.yaml` places these handlers gaplessly:
  800B8A80 + 0xBC = 800B8B3C, and 800B8B3C + 0x45C = 800B8F98.
  `build/ovl_21/asm/data/3C64.data.s` places them in consecutive slots 6/7/8
  of `D_800BCB6C`, which the established dispatcher
  `src/overlays/ovl_21/ovl_21_func_800B806C.c` indexes with `D_800C0448[0]`.
  Thus state transitions, dispatcher order, and local link order corroborate
  the shared update block.

## Membership conclusion and limits

Add only `ovl_21_func_800B8B3C` to the existing `ovl_21` D_800C0448 cluster,
with the role state-7 update handler coordinating input, periodic record
updates, and transition to state 8. The already-recorded call to 800B98CC
through the +0x10 sub-view is corroboration, not a new discovery here.
Under the existing same-TU hypothesis, the shared lifecycle/update idiom and
ordered neighbouring handlers support membership at medium confidence.
Shared storage and calls can cross TUs; no TU boundary, static ownership, or
additional neighbour membership is established.
